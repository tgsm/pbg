#include <rwsdk/badevice.h>

static RwGlobals staticGlobals;
RwGlobals* RwEngineInstance;

unsigned int RwEngineGetVersion() {
    return 0x34003; // version 3.4.0.3
}
