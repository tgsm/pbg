#ifndef RWA_RWASTREAM_H
#define RWA_RWASTREAM_H

#include <rwa/core/rwaintf.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwaStreamStatus {
    rwaNASTREAMSTATUS = 0,
    rwaSTREAMSTATUS_OPENING = 2,
    rwaSTREAMSTATUS_STOPPED = 3,
    rwaSTREAMSTATUS_PAUSING = 4,
    rwaSTREAMSTATUS_PAUSED = 5,
    rwaSTREAMSTATUS_PLAYING = 6,
    rwaSTREAMSTATUS_STARVED = 7,
    rwaSTREAMSTATUS_STOPPING = 8,
    rwaSTREAMSTATUS_NOMEDIA = 9,
    rwaSTREAMSTATUS_FORCEENUMSIZEINT = 0x7FFFFFFF,
} RwaStreamStatus;

typedef enum RwaStreamPlayState {
    rwaNASTREAMPLAYSTATE = 0,
    rwaSTREAMPLAYSTATE_STOP = 1,
    rwaSTREAMPLAYSTATE_PLAY = 2,
    rwaSTREAMPLAYSTATE_PAUSE = 3,
    rwaSTREAMPLAYSTATE_FORCEENUMSIZEINT = 0x7FFFFFFF,
} RwaStreamPlayState;

typedef struct RwaUnkMedia {
    char* filename;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
} RwaUnkMedia;

typedef struct RwaStreamGainElement {
    RwInt32 inputChannel;
    RwInt32 bussNumber;
    RwReal gain;
} RwaStreamGainElement;

typedef struct RwaStreamGain {
    RwInt32 subStream;
    RwaStreamGainElement element;
} RwaStreamGain;

typedef enum RwaStreamPosSegmentType {
    rwaNASTREAMPOSSEGMENTTYPE = 0,
    rwaSTREAMPOSSEGMENTTYPE_NAME = 1,
    rwaSTREAMPOSSEGMENTTYPE_UUID = 2,
    rwaSTREAMPOSSEGMENTTYPE_INDEX = 3,
    rwaSTREAMPOSSEGMENTTYPE_FORCEENUMSIZEINT = 0x7FFFFFFF,
} RwaStreamPosSegmentType;

typedef enum RwaStreamPosTimeType {
    rwaNASTREAMPOSTIMETYPE = 0,
    rwaSTREAMPOSTIMETYPE_TIME = 1,
    rwaSTREAMPOSTIMETYPE_SAMPLES = 2,
    rwaSTREAMPOSTIMETYPE_FORCEENUMSIZEINT = 0x7FFFFFFF,
} RwaStreamPosTimeType;

typedef struct RwaStreamPos {
    union {
        RwChar* name;
        RwaUUID* uuid;
        RwUInt32 index;
    } segment;
    RwaStreamPosSegmentType segmentType;
    union {
        RwUInt32 samples;
        RwReal seconds;
    } time;
    RwaStreamPosTimeType timeType;
} RwaStreamPos;

typedef struct RwaStreamPosParams {
    RwaStreamPos* pos;
    RwBool flush;
} RwaStreamPosParams;

// TODO
typedef struct RwaStream {
    char unk;
} RwaStream;

RwaStream* RwaStreamCreate(void*, unsigned int, unsigned int, unsigned int, unsigned int); // TODO: params

void _rwaStreamSetMedia(RwaStream* stream, RwaUnkMedia* media);
RwaStreamStatus _rwaStreamGetStatus(RwaStream* stream);

void _rwaStreamSetStreamGain(RwaStream* stream, RwaStreamGain* gain);
void _rwaStreamSetPitch(RwaStream* stream, RwReal pitch);
void _rwaStreamSetPlayState(RwaStream* stream, RwaStreamPlayState playState);
void _rwaStreamSetStreamPos(RwaStream* stream, RwaStreamPosParams* posParams);
void _rwaStreamSetLoopSegment(RwaStream* stream, RwBool loop);

#ifdef __cplusplus
}
#endif

#endif
