#include "../../gnu-efi/inc/efi.h"
#include "../../gnu-efi/inc/efilib.h"

#pragma pack(1)
typedef struct {
    UINT16 Signature;
    UINT32 FileSize;
    UINT16 Reserved1;
    UINT16 Reserved2;
    UINT32 PixelDataOffset;
} BMP_FILE_HEADER;

typedef struct {
    UINT32 HeaderSize;
    UINT32 Width;
    UINT32 Height;
    UINT16 Planes;
    UINT16 BitsPerPixel;
    UINT32 Compression;
    UINT32 ImageSize;
    UINT32 XPixelsPerMeter;
    UINT32 YPixelsPerMeter;
    UINT32 ColorsUsed;
    UINT32 ImportantColors;
} BMP_INFO_HEADER;
#pragma pack()
