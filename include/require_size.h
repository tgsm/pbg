#pragma once on

#if defined(__cplusplus) && defined(__MWERKS__)
struct YourSizeIsWrong {};
#define REQUIRE_SIZE(class_, size) \
    typedef YourSizeIsWrong YouShouldFixIt[(sizeof(class_) == (size)) ? 1 : -1]
#else
#define REQUIRE_SIZE(class_, size)
#endif
