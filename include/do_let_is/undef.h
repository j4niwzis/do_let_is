#include "do_let_is/detail_stack_state.h"
#undef DO_LET_IS_NEXT_SHORT_IS_C
#undef DO_LET_IS_USE_BLOCKS
#define DO_LET_IS_NEXT_STATE \
  ((DO_LET_IS_STATE >> 4) ? (((DO_LET_IS_STATE >> 8) << 4) | (((DO_LET_IS_STATE >> 4) & 0xFULL) - 1ULL)) : 0ULL)
#include "do_let_is/detail_eval_state.h"
#undef DO_LET_IS_NEXT_STATE
#include "do_let_is/detail_apply_mode.h"
