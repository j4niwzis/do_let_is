#include "do_let_is/counter.h"
#if DO_LET_IS_CNT_SEL == 0
#undef DO_LET_IS_CB_0
#if (((DO_LET_IS_COUNTER) + 1) % 10) == 0
#define DO_LET_IS_CB_0 0
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 1
#define DO_LET_IS_CB_0 1
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 2
#define DO_LET_IS_CB_0 2
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 3
#define DO_LET_IS_CB_0 3
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 4
#define DO_LET_IS_CB_0 4
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 5
#define DO_LET_IS_CB_0 5
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 6
#define DO_LET_IS_CB_0 6
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 7
#define DO_LET_IS_CB_0 7
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 8
#define DO_LET_IS_CB_0 8
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 9
#define DO_LET_IS_CB_0 9
#endif
#undef DO_LET_IS_CB_1
#if (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 0
#define DO_LET_IS_CB_1 0
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 1
#define DO_LET_IS_CB_1 1
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 2
#define DO_LET_IS_CB_1 2
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 3
#define DO_LET_IS_CB_1 3
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 4
#define DO_LET_IS_CB_1 4
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 5
#define DO_LET_IS_CB_1 5
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 6
#define DO_LET_IS_CB_1 6
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 7
#define DO_LET_IS_CB_1 7
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 8
#define DO_LET_IS_CB_1 8
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 9
#define DO_LET_IS_CB_1 9
#endif
#undef DO_LET_IS_CB_2
#if (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 0
#define DO_LET_IS_CB_2 0
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 1
#define DO_LET_IS_CB_2 1
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 2
#define DO_LET_IS_CB_2 2
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 3
#define DO_LET_IS_CB_2 3
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 4
#define DO_LET_IS_CB_2 4
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 5
#define DO_LET_IS_CB_2 5
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 6
#define DO_LET_IS_CB_2 6
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 7
#define DO_LET_IS_CB_2 7
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 8
#define DO_LET_IS_CB_2 8
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 9
#define DO_LET_IS_CB_2 9
#endif
#undef DO_LET_IS_CB_3
#if (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 0
#define DO_LET_IS_CB_3 0
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 1
#define DO_LET_IS_CB_3 1
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 2
#define DO_LET_IS_CB_3 2
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 3
#define DO_LET_IS_CB_3 3
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 4
#define DO_LET_IS_CB_3 4
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 5
#define DO_LET_IS_CB_3 5
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 6
#define DO_LET_IS_CB_3 6
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 7
#define DO_LET_IS_CB_3 7
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 8
#define DO_LET_IS_CB_3 8
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 9
#define DO_LET_IS_CB_3 9
#endif
#undef DO_LET_IS_CB_VAL
#if ((DO_LET_IS_COUNTER) + 1) < 10
#define DO_LET_IS_CB_VAL DO_LET_IS_CB_0
#elif ((DO_LET_IS_COUNTER) + 1) < 100
#define DO_LET_IS_CB_VAL DO_LET_IS_CNT_CAT2(DO_LET_IS_CB_1, DO_LET_IS_CB_0)
#elif ((DO_LET_IS_COUNTER) + 1) < 1000
#define DO_LET_IS_CB_VAL DO_LET_IS_CNT_CAT3(DO_LET_IS_CB_2, DO_LET_IS_CB_1, DO_LET_IS_CB_0)
#else
#define DO_LET_IS_CB_VAL DO_LET_IS_CNT_CAT4(DO_LET_IS_CB_3, DO_LET_IS_CB_2, DO_LET_IS_CB_1, DO_LET_IS_CB_0)
#endif
#undef DO_LET_IS_CNT_SEL
#define DO_LET_IS_CNT_SEL 1
#undef DO_LET_IS_COUNTER
#define DO_LET_IS_COUNTER DO_LET_IS_CB_VAL
#else
#undef DO_LET_IS_CA_0
#if (((DO_LET_IS_COUNTER) + 1) % 10) == 0
#define DO_LET_IS_CA_0 0
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 1
#define DO_LET_IS_CA_0 1
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 2
#define DO_LET_IS_CA_0 2
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 3
#define DO_LET_IS_CA_0 3
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 4
#define DO_LET_IS_CA_0 4
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 5
#define DO_LET_IS_CA_0 5
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 6
#define DO_LET_IS_CA_0 6
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 7
#define DO_LET_IS_CA_0 7
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 8
#define DO_LET_IS_CA_0 8
#elif (((DO_LET_IS_COUNTER) + 1) % 10) == 9
#define DO_LET_IS_CA_0 9
#endif
#undef DO_LET_IS_CA_1
#if (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 0
#define DO_LET_IS_CA_1 0
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 1
#define DO_LET_IS_CA_1 1
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 2
#define DO_LET_IS_CA_1 2
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 3
#define DO_LET_IS_CA_1 3
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 4
#define DO_LET_IS_CA_1 4
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 5
#define DO_LET_IS_CA_1 5
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 6
#define DO_LET_IS_CA_1 6
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 7
#define DO_LET_IS_CA_1 7
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 8
#define DO_LET_IS_CA_1 8
#elif (((DO_LET_IS_COUNTER) + 1) / 10 % 10) == 9
#define DO_LET_IS_CA_1 9
#endif
#undef DO_LET_IS_CA_2
#if (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 0
#define DO_LET_IS_CA_2 0
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 1
#define DO_LET_IS_CA_2 1
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 2
#define DO_LET_IS_CA_2 2
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 3
#define DO_LET_IS_CA_2 3
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 4
#define DO_LET_IS_CA_2 4
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 5
#define DO_LET_IS_CA_2 5
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 6
#define DO_LET_IS_CA_2 6
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 7
#define DO_LET_IS_CA_2 7
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 8
#define DO_LET_IS_CA_2 8
#elif (((DO_LET_IS_COUNTER) + 1) / 100 % 10) == 9
#define DO_LET_IS_CA_2 9
#endif
#undef DO_LET_IS_CA_3
#if (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 0
#define DO_LET_IS_CA_3 0
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 1
#define DO_LET_IS_CA_3 1
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 2
#define DO_LET_IS_CA_3 2
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 3
#define DO_LET_IS_CA_3 3
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 4
#define DO_LET_IS_CA_3 4
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 5
#define DO_LET_IS_CA_3 5
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 6
#define DO_LET_IS_CA_3 6
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 7
#define DO_LET_IS_CA_3 7
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 8
#define DO_LET_IS_CA_3 8
#elif (((DO_LET_IS_COUNTER) + 1) / 1000 % 10) == 9
#define DO_LET_IS_CA_3 9
#endif
#undef DO_LET_IS_CA_VAL
#if ((DO_LET_IS_COUNTER) + 1) < 10
#define DO_LET_IS_CA_VAL DO_LET_IS_CA_0
#elif ((DO_LET_IS_COUNTER) + 1) < 100
#define DO_LET_IS_CA_VAL DO_LET_IS_CNT_CAT2(DO_LET_IS_CA_1, DO_LET_IS_CA_0)
#elif ((DO_LET_IS_COUNTER) + 1) < 1000
#define DO_LET_IS_CA_VAL DO_LET_IS_CNT_CAT3(DO_LET_IS_CA_2, DO_LET_IS_CA_1, DO_LET_IS_CA_0)
#else
#define DO_LET_IS_CA_VAL DO_LET_IS_CNT_CAT4(DO_LET_IS_CA_3, DO_LET_IS_CA_2, DO_LET_IS_CA_1, DO_LET_IS_CA_0)
#endif
#undef DO_LET_IS_CNT_SEL
#define DO_LET_IS_CNT_SEL 0
#undef DO_LET_IS_COUNTER
#define DO_LET_IS_COUNTER DO_LET_IS_CA_VAL
#endif
