#include <bertrand/bertrand.hpp>

namespace {
struct Parent {
  struct Nested {
    void failing_func() { Require(false, "Cannot be false"); }
  };
};
} // namespace

int main(int, char **) {
  Parent::Nested().failing_func();
  return 0;
}