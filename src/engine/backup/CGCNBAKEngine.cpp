#include <dolphin/card.h>
#include <cstring>
#include "engine/backup/CGCNBAKEngine.h"

static U8 CardWorkArea[0xA000];
static int CRCTable[0x100];

void CardDettachCB(s32, s32);

void CRCInit() {
    U32 i;
    U32 j;
    U32 uVar3;

    for (i = 0; i < 0x100; i++) {
        uVar3 = i << 24;
        j = 0;
        for (; j < 8; j++) {
            if (!(uVar3 & 0x80000000)) {
                uVar3 <<= 1;
            } else {
                uVar3 = uVar3 << 1 ^ 0x4C11DB7;
            }
        }
        CRCTable[i] = uVar3;
    }
}

namespace DKBAK {

CGCNBAKEngine::CGCNBAKEngine() {

}

void CGCNBAKEngine::SetGameTitle(std::string title) {
    strncpy(m_game_title, title.c_str(), 32);
    m_game_title[title.size()] = '\n';
}

CGCNBAKEngine::~CGCNBAKEngine() {
    if (m_unk8 > 1) {
        while (CARDUnmount(m_channel) == CARD_RESULT_BUSY);
    }
}

extern "C" extern CGCNBAKEngine* DkBakUpGetEngine(void);
void CardDettachCB(s32, s32) {
    DkBakUpGetEngine()->GetState(); // unused
}

U32 CGCNBAKEngine::CheckState() {
    s32 result;

    while (TRUE) {
        switch (m_unk8) {
            case 0: {
                do {
                    result = CARDProbeEx(m_channel, &m_memory_size, &m_sector_size);
                } while (result == CARD_RESULT_BUSY);

                switch (result) {
                    case CARD_RESULT_READY:
                        m_unk8++;
                        continue;
                    case CARD_RESULT_WRONGDEVICE:
                        return 2;
                    case CARD_RESULT_NOCARD:
                        return 0;
                    case CARD_RESULT_BUSY:
                    default:
                        return 9;
                }
                break;
            }
            case 1: {
                do {
                    result = CARDMount(m_channel, CardWorkArea, CardDettachCB);
                } while (result == CARD_RESULT_BUSY);

                switch (result) {
                    case CARD_RESULT_READY:
                    case CARD_RESULT_ENCODING:
                    case CARD_RESULT_BROKEN:
                        m_unk8++;
                        continue;
                    case CARD_RESULT_WRONGDEVICE:
                        m_unk8 = 0;
                        return 2;
                    case CARD_RESULT_NOCARD:
                        m_unk8 = 0;
                        return 0;
                    case CARD_RESULT_IOERROR:
                    case CARD_RESULT_FATAL_ERROR:
                    case CARD_RESULT_BUSY:
                    default:
                        m_unk8 = 0;
                        return 9;
                }
                break;
            }
            case 2: {
                do {
                    result = CARDCheck(m_channel);
                } while (result == CARD_RESULT_BUSY);

                switch (result) {
                    case CARD_RESULT_READY:
                        return 17;
                    case CARD_RESULT_ENCODING:
                        return 33;
                    case CARD_RESULT_BROKEN:
                        return 5;
                    case CARD_RESULT_NOCARD:
                        m_unk8 = 0;
                        do {
                            result = CARDUnmount(m_channel);
                        } while (result == CARD_RESULT_BUSY);
                        return 0;
                    case CARD_RESULT_IOERROR:
                    case CARD_RESULT_FATAL_ERROR:
                    default:
                        m_unk8 = 0;
                        return 9;

                }
                break;
            }
        }
    }
}

// I have no idea what this function even is
U32 CGCNBAKEngine::GetState() {
    if (CheckState() != 17) {
        return CheckState();
        // I'm assuming they would have checked for another value and did something in here,
        // but I don't know what.
    }
}

U32 CGCNBAKEngine::Format() {
    m_unk8 = 0;
    while (TRUE) {
        s32 result = CARDFormat(m_channel);
        if (result == CARD_RESULT_BUSY) {
            continue;
        } else if (result == CARD_RESULT_READY) {
            break;
        } else {
            return -99;
        }
    }
    return 0;
}

}
