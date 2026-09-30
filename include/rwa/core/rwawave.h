#ifndef RWA_CORE_RWAWAVE_H
#define RWA_CORE_RWAWAVE_H

#ifdef __cplusplus
extern "C" {
#endif

struct RwaWaveDef;

// TODO
typedef struct RwaWave {
    char unk0[0xC];
    struct RwaWaveDef* waveDef;
} RwaWave;

#ifdef __cplusplus
}
#endif

#endif
