#ifndef DO_LET_IS_COUNTER_H
#define DO_LET_IS_COUNTER_H

#define DO_LET_IS_CNT_CAT2_I(a, b) a##b
#define DO_LET_IS_CNT_CAT2(a, b) DO_LET_IS_CNT_CAT2_I(a, b)
#define DO_LET_IS_CNT_CAT3_I(a, b, c) a##b##c
#define DO_LET_IS_CNT_CAT3(a, b, c) DO_LET_IS_CNT_CAT3_I(a, b, c)
#define DO_LET_IS_CNT_CAT4_I(a, b, c, d) a##b##c##d
#define DO_LET_IS_CNT_CAT4(a, b, c, d) DO_LET_IS_CNT_CAT4_I(a, b, c, d)

#define DO_LET_IS_CNT_SEL 0
#define DO_LET_IS_COUNTER 0

#define DO_LET_IS_UPDATE_COUNTER() "do_let_is/update_counter.h"

#endif
