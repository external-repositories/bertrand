#include <catch.hpp>

#define __BERTRAND_CONTRACTS_ARE_EXCEPTIONS
#include <bertrand/bertrand.hpp>

TEST_CASE(
    "GIVEN a precondition contract WHEN passed true THEN no assert happens") {

  REQUIRE_NOTHROW(require(true));
}

TEST_CASE(
    "GIVEN a precondition contract WHEN passed false THEN assert happens") {
  REQUIRE_THROWS(require(false));
}

TEST_CASE(
    "GIVEN a postcondition contract WEHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(ensure(true));
}

TEST_CASE(
    "GIVEN a postcondition contract WEHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(ensure(false));
}
TEST_CASE(
    "GIVEN an invariant contract WEHEN passed true THEN no assert happens") {
  REQUIRE_NOTHROW(invariant(true));
}

TEST_CASE(
    "GIVEN a invariant contract WEHEN passed false THEN  assert happens") {
  REQUIRE_THROWS(invariant(false));
}