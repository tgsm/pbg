#include <rwa/interfaces/rwavirtualvoice.h>

static RwaObjInterfaceDef _rwaVirtualVoiceIntDef;

const RwaUUID rwaVIRTUALVOICEINTERFACEID = {
    0x94144D2D, 0xC043, 0x4AFE, { 0x95, 0x77, 0xC8, 0x01, 0x82, 0x4B, 0x68, 0xDD }
};

const RwaUUID rwaVIRTUALVOICEIDI_PRIORITY = {
    0x695E4026, 0xBBBB, 0x4A5D, { 0xB9, 0xF9, 0x0B, 0x29, 0xBB, 0x71, 0x28, 0x1F }
};

const RwaUUID rwaVIRTUALVOICEIDO_PRIORITY = {
    0x6DCC5DD6, 0x9239, 0x49D7, { 0xB5, 0x82, 0xFB, 0x40, 0x54, 0x56, 0x7B, 0x6C }
};

const RwaUUID rwaVIRTUALVOICEIDI_BIAS = {
    0x3E3643A0, 0x9A85, 0x4C9B, { 0x96, 0x43, 0x3B, 0xEE, 0xB0, 0xAF, 0x05, 0x37 }
};

const RwaUUID rwaVIRTUALVOICEIDO_BIAS = {
    0x55920110, 0x22E2, 0x495B, { 0x9F, 0xE8, 0x35, 0xA3, 0x74, 0x79, 0xF1, 0x87 }
};

extern const RwaUUID rwaVOICE3DIDI_POS;
extern const RwaUUID rwaVOICE3DIDI_VEL;
extern const RwaUUID rwaVOICE3DIDI_MINDISTANCE;
extern const RwaUUID rwaVOICE3DIDI_MAXDISTANCE;
extern const RwaUUID rwaVOICEIDI_FREQ;
extern const RwaUUID rwaVOICEIDI_GAIN;
extern const RwaUUID rwaVOICEIDI_TRIGGER;
extern const RwaUUID rwaVOICEIDI_WAVEPOS;
extern const RwaUUID rwaVOICEIDI_LOOP;
extern const RwaUUID rwaVOICEIDI_WAVE;
extern const RwaUUID rwaVOICEIDI_PAN;

extern const RwaUUID rwaPARAMTYPE_UINT32;
extern const RwaUUID rwaPARAMTYPE_REAL;
extern const RwaUUID rwaPARAMTYPE_RWV3D;
extern const RwaUUID rwaPARAMTYPE_RWBOOL;
extern const RwaUUID rwaPARAMTYPE_POINTER;

static RwaObjInterfaceDefParam _rwaVirtualVoiceIntInputs[] = {
    { { (RwaUUID*)&rwaVIRTUALVOICEIDI_PRIORITY, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVIRTUALVOICEIDI_BIAS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDI_POS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWV3D, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDI_VEL, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWV3D, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDI_MINDISTANCE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDI_MAXDISTANCE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_FREQ, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_GAIN, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_TRIGGER, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWBOOL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_WAVEPOS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_UINT32, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_LOOP, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWBOOL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_WAVE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_POINTER, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDI_PAN, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
};

extern const RwaUUID rwaVOICE3DIDO_POS;
extern const RwaUUID rwaVOICE3DIDO_VEL;
extern const RwaUUID rwaVOICE3DIDO_MINDISTANCE;
extern const RwaUUID rwaVOICE3DIDO_MAXDISTANCE;
extern const RwaUUID rwaVOICEIDO_FREQ;
extern const RwaUUID rwaVOICEIDO_GAIN;
extern const RwaUUID rwaVOICEIDO_PLAYING;
extern const RwaUUID rwaVOICEIDO_WAVEPOS;
extern const RwaUUID rwaVOICEIDO_LOOP;
extern const RwaUUID rwaVOICEIDO_WAVE;
extern const RwaUUID rwaVOICEIDO_PAN;
extern const RwaUUID rwaVOICEIDO_ISSTEREO;

static RwaObjInterfaceDefParam _rwaVirtualVoiceIntOutputs[] = {
    { { (RwaUUID*)&rwaVIRTUALVOICEIDO_PRIORITY, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVIRTUALVOICEIDO_BIAS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDO_POS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWV3D, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDO_VEL, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWV3D, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDO_MINDISTANCE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICE3DIDO_MAXDISTANCE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_FREQ, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_GAIN, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_PLAYING, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWBOOL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_WAVEPOS, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_UINT32, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_LOOP, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWBOOL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_WAVE, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_POINTER, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_PAN, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_REAL, TRUE } },
    { { (RwaUUID*)&rwaVOICEIDO_ISSTEREO, NULL, 0 }, { (RwaUUID*)&rwaPARAMTYPE_RWBOOL, TRUE } },
};

RwaObjInterfaceDef* RwaVirtualVoiceInterfaceRegister(void) {
    RwaObjInterfaceDef* def = RwaObjInterfaceDefCreate(&_rwaVirtualVoiceIntDef);
    if (def == NULL) {
        return NULL;
    }

    RwaObjInterfaceDefAssignID(&_rwaVirtualVoiceIntDef, &rwaVIRTUALVOICEINTERFACEID, NULL);
    RwaObjInterfaceDefSetup(&_rwaVirtualVoiceIntDef, _rwaVirtualVoiceIntInputs, 13, _rwaVirtualVoiceIntOutputs, 14);

    return &_rwaVirtualVoiceIntDef;
}

void RwaVirtualVoiceInterfaceUnregister(void) {
    RwaObjInterfaceDefDestroy(&_rwaVirtualVoiceIntDef);
}
