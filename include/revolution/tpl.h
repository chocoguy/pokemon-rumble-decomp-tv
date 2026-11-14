#ifndef TPL_H
#define TPL_H

#include "revolution/gx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u16 numEntries;
    u8 unpacked;
    u8 _4;
    GXTlutFmt format;
#if defined(PORT)
    PORT_PTR32(char) data; /* 4-byte slots in the file: see port_addr.h */
#else
    char* data;
#endif
} TPLClutHeader, *TPLClutHeaderPtr;

typedef struct {
    u16 height;
    u16 width;
    u32 format;
#if defined(PORT)
    PORT_PTR32(char) data;
#else
    char* data;
#endif
    GXTexWrapMode wrapS;
    GXTexWrapMode wrapT;
    GXTexFilter minFilter;
    GXTexFilter magFilter;
    f32 LODBias;
    u8 edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    u8 unpacked;
} TPLHeader, *TPLHeaderPtr;

typedef struct {
#if defined(PORT)
    PORT_PTR32(TPLHeader) textureHeader;
    PORT_PTR32(TPLClutHeader) CLUTHeader;
#else
    TPLHeaderPtr textureHeader;
    TPLClutHeaderPtr CLUTHeader;
#endif
} TPLDescriptor, *TPLDescriptorPtr;

typedef struct {
    u32 versionNumber;
    u32 numDescriptors;
#if defined(PORT)
    PORT_PTR32(TPLDescriptor) descriptorArray;
#else
    TPLDescriptorPtr descriptorArray;
#endif
} TPLPalette, *TPLPalettePtr;

void TPLBind(TPLPalettePtr);
TPLDescriptorPtr TPLGet(TPLPalettePtr, u32);
void TPLGetGXTexObjFromPalette(TPLPalettePtr, GXTexObj*, u32);

#ifdef __cplusplus
}
#endif

#endif
