/**
 * This file is part of "bertrand" version 0.0.12
 * https://github.com/bernedom/bertrand
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

// fail compilation of contracts are force-enabled and force-disabled at the
// same time
#if defined(BERTRAND_DISABLE_CONTRACTS) && defined(BERTRAND_ENABLE_CONTRACTS)
static_assert(false,
              "Cannot force enable and disable contracts at the same time");
#endif

#if !defined(NDEBUG) && !defined(BERTRAND_ENABLE_CONTRACTS) &&                 \
    !defined(BERTRAND_DISABLE_CONTRACTS)
#define BERTRAND_ENABLE_CONTRACTS
#endif

#ifdef BERTRAND_ENABLE_CONTRACTS
#include <iostream>
#include <sstream>
// asserts as exceptions is a workaround for testing purposes, do not use in
// production
#ifdef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <stdexcept>
#else
#include <cstdlib>
#endif

namespace bertrand {

template <typename... Args>
inline void assert_handler(bool expr, const char *expression, const char *file,
                           int line, const char *type, const char *message,
                           const Args &... args) {
  if (!expr) {
    std::stringstream buffer;

    buffer << "Assert at: " << file << ":" << line << ": " << type << " ('"
           << expression << "') " << message;
    if constexpr (sizeof...(args) > 0) {
      (buffer << ... << args);
    }
    buffer << "\n";
    std::cerr << buffer.str();
#ifdef BERTRAND_CONTRACTS_ARE_EXCEPTIONS
    throw std::runtime_error(buffer.str());
#else
    abort();
#endif
  }
}
} // namespace bertrand

/// this forwarding macro is needed to get the correct file and line information
#define bertrand_handle_assert_impl(EXPR, FILE, LINE, CONTRACT_TYPE, ...)      \
  bertrand::assert_handler((EXPR), #EXPR, FILE, LINE, CONTRACT_TYPE,           \
                           __VA_ARGS__)

#define Require(EXPR, ...)                                                     \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Require", __VA_ARGS__)
#define Ensure(EXPR, ...)                                                      \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Ensure", __VA_ARGS__)
#define Invariant(EXPR, ...)                                                   \
  bertrand_handle_assert_impl(EXPR, __FILE__, __LINE__, "Invariant",           \
                              __VA_ARGS__)
#else
#define Require(EXPR, ...) (static_cast<void>(0))
#define Ensure(EXPR, ...) (static_cast<void>(0))
#define Invariant(EXPR, ...) (static_cast<void>(0))
#endif