#if DO_LET_IS_STATE_SEL == 0
#include "do_let_is/detail_write_b.h"
#undef DO_LET_IS_STATE_SEL
#define DO_LET_IS_STATE_SEL 1
#undef DO_LET_IS_STATE
#define DO_LET_IS_STATE DO_LET_IS_STATE_B
#else
#include "do_let_is/detail_write_a.h"
#undef DO_LET_IS_STATE_SEL
#define DO_LET_IS_STATE_SEL 0
#undef DO_LET_IS_STATE
#define DO_LET_IS_STATE DO_LET_IS_STATE_A
#endif
