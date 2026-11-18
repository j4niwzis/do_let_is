#ifndef DO_LET_IS_DETAIL_STACK_STATE_H
#define DO_LET_IS_DETAIL_STACK_STATE_H

#define DO_LET_IS_HEX16_I(h15, h14, h13, h12, h11, h10, h9, h8, h7, h6, h5, h4, h3, h2, h1, h0) \
  0x##h15##h14##h13##h12##h11##h10##h9##h8##h7##h6##h5##h4##h3##h2##h1##h0##ULL
#define DO_LET_IS_HEX16(h15, h14, h13, h12, h11, h10, h9, h8, h7, h6, h5, h4, h3, h2, h1, h0) \
  DO_LET_IS_HEX16_I(h15, h14, h13, h12, h11, h10, h9, h8, h7, h6, h5, h4, h3, h2, h1, h0)

#define DO_LET_IS_SA_15 0
#define DO_LET_IS_SA_14 0
#define DO_LET_IS_SA_13 0
#define DO_LET_IS_SA_12 0
#define DO_LET_IS_SA_11 0
#define DO_LET_IS_SA_10 0
#define DO_LET_IS_SA_9 0
#define DO_LET_IS_SA_8 0
#define DO_LET_IS_SA_7 0
#define DO_LET_IS_SA_6 0
#define DO_LET_IS_SA_5 0
#define DO_LET_IS_SA_4 0
#define DO_LET_IS_SA_3 0
#define DO_LET_IS_SA_2 0
#define DO_LET_IS_SA_1 0
#define DO_LET_IS_SA_0 0

#define DO_LET_IS_STATE_A                                                                       \
  DO_LET_IS_HEX16(DO_LET_IS_SA_15, DO_LET_IS_SA_14, DO_LET_IS_SA_13, DO_LET_IS_SA_12,           \
                  DO_LET_IS_SA_11, DO_LET_IS_SA_10, DO_LET_IS_SA_9, DO_LET_IS_SA_8,             \
                  DO_LET_IS_SA_7, DO_LET_IS_SA_6, DO_LET_IS_SA_5, DO_LET_IS_SA_4,               \
                  DO_LET_IS_SA_3, DO_LET_IS_SA_2, DO_LET_IS_SA_1, DO_LET_IS_SA_0)

#define DO_LET_IS_STATE_B                                                                       \
  DO_LET_IS_HEX16(DO_LET_IS_SB_15, DO_LET_IS_SB_14, DO_LET_IS_SB_13, DO_LET_IS_SB_12,           \
                  DO_LET_IS_SB_11, DO_LET_IS_SB_10, DO_LET_IS_SB_9, DO_LET_IS_SB_8,             \
                  DO_LET_IS_SB_7, DO_LET_IS_SB_6, DO_LET_IS_SB_5, DO_LET_IS_SB_4,               \
                  DO_LET_IS_SB_3, DO_LET_IS_SB_2, DO_LET_IS_SB_1, DO_LET_IS_SB_0)

#define DO_LET_IS_STATE_SEL 0
#define DO_LET_IS_STATE DO_LET_IS_STATE_A

#endif
