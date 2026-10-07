#ifndef RWA_INTERFACES_RWAVIRTUALVOICE_H
#define RWA_INTERFACES_RWAVIRTUALVOICE_H

#include <rwa/core/rwaobjinterface.h>

#ifdef __cplusplus
extern "C" {
#endif

RwaObjInterfaceDef* RwaVirtualVoiceInterfaceRegister(void);
void RwaVirtualVoiceInterfaceUnregister(void);

#ifdef __cplusplus
}
#endif

#endif
