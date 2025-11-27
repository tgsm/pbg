#ifndef RWA_RWASTREAM
#define RWA_RWASTREAM

#ifdef __cplusplus
extern "C" {
#endif

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

// TODO
typedef struct RwaStream {
    char unk;
} RwaStream;

RwaStream* RwaStreamCreate(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // TODO: params

void _rwaStreamSetMedia(RwaStream* stream, RwaUnkMedia* media);
int _rwaStreamGetStatus(RwaStream* stream);

#ifdef __cplusplus
}
#endif

#endif
