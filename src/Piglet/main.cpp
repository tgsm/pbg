#include "CGame.h"
#include <iostream>

extern "C" {
extern void OSSetNumberOfVerySmallBuffers(int);
extern void OSSetQualificationSize(int);
extern void OSSetConfigurator(void(*)(int*), BOOL);
}

void MemoryConfig(int* unk) {
    *unk = 0x340000;
    OSSetNumberOfVerySmallBuffers(0x2000);
    OSSetQualificationSize(0x1000);
}

extern "C" void DkMemoryGCNPreSetup() {
    OSSetConfigurator(MemoryConfig, TRUE);
}

int main() {
    DkMemoryGCNPreSetup();

    CGame* game = new CGame(NULL, 0);
    if (game != NULL) {
        while (game->NextFrame()) {}

        delete game;
    }

    return 0;
}
