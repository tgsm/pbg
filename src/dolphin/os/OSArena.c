#include <dolphin.h>
#include <dolphin/os.h>

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))
#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))

static void* __OSArenaHi;
static void* __OSArenaLo = (void*)-1;

void* OSGetArenaHi(void) {
    ASSERTMSGLINE(55, (u32)__OSArenaLo != -1, "OSGetArenaHi(): OSInit() must be called in advance.");
    ASSERTMSGLINE(57, (u32)__OSArenaLo <= (u32)__OSArenaHi, "OSGetArenaHi(): invalid arena (hi < lo).");
    return __OSArenaHi;
}

void* OSGetArenaLo(void) {
    ASSERTMSGLINE(73, (u32)__OSArenaLo != -1, "OSGetArenaLo(): OSInit() must be called in advance.");
    ASSERTMSGLINE(75, (u32)__OSArenaLo <= (u32)__OSArenaHi, "OSGetArenaLo(): invalid arena (hi < lo).");
    return __OSArenaLo;
}

void OSSetArenaHi(void* newHi) {
    __OSArenaHi = newHi;
}

void OSSetArenaLo(void* newLo) {
    __OSArenaLo = newLo;
}
