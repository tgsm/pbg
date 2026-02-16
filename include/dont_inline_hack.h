#ifndef PIGLET_DONT_INLINE_HACK
#define PIGLET_DONT_INLINE_HACK

#ifdef __MWERKS__
#define DONT_INLINE_HACK() \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0; \
    (void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0;(void)0
#else
#define DONT_INLINE_HACK()
#endif

#endif
