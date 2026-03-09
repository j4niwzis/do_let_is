#include "errored.h"

using namespace doletis;  // NOLINT

#define CALL LET _ IS

constexpr errored<int> foo() { return 1; }

constexpr errored<int, char> bar1() { return error{'a'}; }

constexpr errored<int, int> bar2() { return 1; }

constexpr errored<int, int> bar3() { return 1; }

constexpr errored<void_result, float> bar4() { return error<float>{4}; }

constexpr auto chain() {  // errored<int, char, float, int>
  return DO(
   LET a IS(foo());
   LET b IS(bar1());
   LET c IS(bar2());
   LET d IS(bar3());
   CALL(bar4());
   return errored{a + b + c + d};
 );
}

static_assert(chain().error() == std::variant<char, float, int>{'a'});

int main() {}
