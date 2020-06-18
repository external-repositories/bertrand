#include <bertrand/bertrand.hpp>

namespace {
struct Parent {
  struct Nested {
    void failing_func(int c) { Require(false, "Cannot be false"); }
  };
};

inline void level2(int c) { Parent::Nested().failing_func(c); }

inline void level1(int c) { level2(c); }

} // namespace

int main(int argc, char **) {

  level1(argc);
  return 0;
}