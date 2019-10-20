/**
 * This file is part of "bertrand"
 * A minimalistic, header only implementation of design by contract for C++
 *
 * As a header only library using this in proprietary software is fine as long
 * as it is used unmodified and that there is somewhere a remark about its usage
 * and a link to the lgpl licence
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#pragma once

///@todo add function to message
///@todo add possibility for delivering stack trace

// asserts as exceptions is a workaround for testing purposes, do not use in
// production

#include <cstdio>

#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <stdexcept>
#else
#include <cstdlib>
#endif

namespace bertrand {
inline void assert_handler(bool b, const char *expression, const char *message,
                           const char *file, int line) {
  if (!b) {
    // directly allocating is not so nice, but at that point something went
    // wrong anyway
    char buffer[2048];
    snprintf(buffer, sizeof(buffer), "%s:%d: ('%s') %s", file, line, expression,
             message);
    fprintf(stderr, "%s\n", buffer);
#ifdef __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
    throw(std::runtime_error(buffer));
#else
    abort();
#endif
  }
}
} // namespace bertrand

#define __bertrand_handle_assert(EXPR, MSG, FILE, LINE)                        \
  bertrand::assert_handler((EXPR), #EXPR, MSG, FILE, LINE)

#ifndef NDEBUG
#define require(EXPR, MSG)                                                     \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define ensure(EXPR, MSG)                                                      \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#define invariant(EXPR, MSG)                                                   \
  __bertrand_handle_assert(EXPR, MSG, __FILE__, __LINE__)
#else
#define require(EXPR, MSG)
#define ensure(EXPR, MSG)
#define invariant(EXPR, MSG)
#endif