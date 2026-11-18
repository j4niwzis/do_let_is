#include "do_let_is/detail_stack_state.h"

#undef DO_LET_IS_TARGET_MODE

#if !defined(__cplusplus) || defined(DO_LET_IS_NEXT_SHORT_IS_C) || defined(DO_LET_IS_USE_BLOCKS)
#ifdef DO_LET_IS_USE_BLOCKS
#if defined(DO_LET_IS_HAVE_C_DO) && defined(DO_LET_IS_HAVE_C_GENERATOR)
#define DO_LET_IS_TARGET_MODE 6ULL
#elif defined(DO_LET_IS_HAVE_C_DO)
#define DO_LET_IS_TARGET_MODE 4ULL
#elif defined(DO_LET_IS_HAVE_C_GENERATOR)
#define DO_LET_IS_TARGET_MODE 5ULL
#else
#define DO_LET_IS_TARGET_MODE 0ULL
#endif
#else
#if defined(DO_LET_IS_HAVE_C_DO) && defined(DO_LET_IS_HAVE_C_GENERATOR)
#define DO_LET_IS_TARGET_MODE 3ULL
#elif defined(DO_LET_IS_HAVE_C_DO)
#define DO_LET_IS_TARGET_MODE 1ULL
#elif defined(DO_LET_IS_HAVE_C_GENERATOR)
#define DO_LET_IS_TARGET_MODE 2ULL
#else
#define DO_LET_IS_TARGET_MODE 0ULL
#endif
#endif
#undef DO_LET_IS_USE_BLOCKS
#undef DO_LET_IS_NEXT_SHORT_IS_C
#else
#if defined(DO_LET_IS_HAVE_CXX_DO) && defined(DO_LET_IS_HAVE_CXX_GENERATOR) && \
    defined(DO_LET_IS_HAVE_CXX_GENERATOR_LOOPHOLES)
#define DO_LET_IS_TARGET_MODE 13ULL
#elif defined(DO_LET_IS_HAVE_CXX_GENERATOR) && defined(DO_LET_IS_HAVE_CXX_GENERATOR_LOOPHOLES)
#define DO_LET_IS_TARGET_MODE 12ULL
#elif defined(DO_LET_IS_HAVE_CXX_DO) && defined(DO_LET_IS_HAVE_CXX_GENERATOR_LOOPHOLES)
#define DO_LET_IS_TARGET_MODE 11ULL
#elif defined(DO_LET_IS_HAVE_CXX_GENERATOR_LOOPHOLES)
#define DO_LET_IS_TARGET_MODE 10ULL
#elif defined(DO_LET_IS_HAVE_CXX_DO) && defined(DO_LET_IS_HAVE_CXX_GENERATOR)
#define DO_LET_IS_TARGET_MODE 9ULL
#elif defined(DO_LET_IS_HAVE_CXX_GENERATOR)
#define DO_LET_IS_TARGET_MODE 8ULL
#elif defined(DO_LET_IS_HAVE_CXX_DO)
#define DO_LET_IS_TARGET_MODE 7ULL
#else
#define DO_LET_IS_TARGET_MODE 0ULL
#endif
#endif

#define DO_LET_IS_NEXT_STATE                                                                               \
  ((((DO_LET_IS_STATE - (DO_LET_IS_STATE & 0xFULL)) | ((DO_LET_IS_STATE & 0xFULL) + 1ULL)) << 4) |         \
   (DO_LET_IS_TARGET_MODE))
#include "do_let_is/detail_eval_state.h"
#undef DO_LET_IS_NEXT_STATE
#undef DO_LET_IS_TARGET_MODE
#include "do_let_is/detail_apply_mode.h"
