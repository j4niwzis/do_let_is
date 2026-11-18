// 1) Include C header BEFORE C++ do_let_is: macros must be cleanly undefined afterwards
#include "../c/sample_c_api1.h"

#ifdef DO
#error "DO should be undefined after sample_c_api1.h when C++ short macros were not active"
#endif

// 2) Include 4-level nested chain: C++ -> C -> C -> C++ before outer short.h
#include "nested_cxx_outer.h"

#ifdef DO
#error "DO should be undefined after nested_cxx_outer.h popped back to empty stack"
#endif

// 3) Now activate C++ do_let_is short macros in the .cc file itself
#include <do_let_is/all_short.h>

#include <cstdlib>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

constexpr auto cxx_before_c_header(std::optional<int> a, std::optional<double> b) {
  return DO(
    LET x IS(a);
    LET y IS(b);
    RETURN(static_cast<double>(x) + y);
  );
}

static_assert(cxx_before_c_header(40, 2.0) == 42.0);

// 4) Include C header AFTER C++ DO was active: C DO runs inside header, C++ DO restored after
#include "../c/sample_c_api2.h"

constexpr auto cxx_after_c_header(int base) {
  return DO(
    LET x IS(std::optional{base});
    IF_CONSTEXPR(true)(
      RETURN(x * 2);
    )(
      RETURN(0);
    )
  );
}

static_assert(cxx_after_c_header(21) == 42);

constexpr auto cxx_gen_after_c_header() {
  return GENERATOR((int i), (.i = 0),
    WHILE(i < 3)(
      YIELD(i + 10);
      ++i;
    )
    return end<int, 16>();
  );
}

int main() {
  c_api1_opt r1 = c_api1_add(c_api1_opt_some(19), c_api1_opt_some(23));
  if (!r1.has_value || r1.value != 42) {
    abort();
  }

  int defer_cnt = 0;
  // start=29 -> +1 (L1 cxx) -> +2 (L2 c1) -> +3 (L3 c2) -> +7 (L3.5 cxx-blocks + L4 cxx) = 42!
  auto chain = nested_level1_cxx_after(29, &defer_cnt);
  if (chain != std::optional<int>{42} || defer_cnt != 11) {
    abort();
  }

  auto lh_vals = nested_level1_loophole_gen() | std::ranges::to<std::vector>();
  if (lh_vals != (std::vector<int>{1, 2, 3})) {
    abort();
  }

  int trace_ok = 0;
  c_api2_res r2 = c_api2_txn(false, &trace_ok);
  if (!r2.has_value || r2.value != 42 || trace_ok != 1) {
    abort();
  }

  int trace_err = 0;
  c_api2_res r3 = c_api2_txn(true, &trace_err);
  if (r3.has_value || r3.error != -5 || trace_err != 91) {
    abort();
  }

  auto combined = DO(
    LET a IS(r1.has_value ? std::optional<int>{r1.value} : std::optional<int>{});
    LET b IS(r2.has_value ? std::optional<int>{r2.value} : std::optional<int>{});
    RETURN(a + b);
  );
  if (combined != std::optional<int>{84}) {
    abort();
  }

  auto vals = cxx_gen_after_c_header() | std::ranges::to<std::vector>();
  if (vals != (std::vector<int>{10, 11, 12})) {
    abort();
  }
  return 0;
}
