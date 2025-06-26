#include "../../gnu-efi/inc/efi.h"
#include "../../gnu-efi/inc/efilib.h"
#include "bmp.h"

EFI_STATUS OpenBMPFile(EFI_HANDLE ImageHandle, EFI_FILE_HANDLE *FileHandle, CHAR16 *FileName) {
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_HANDLE Root;
    EFI_STATUS Status;

    Status = uefi_call_wrapper(BS->HandleProtocol, 3,
        ImageHandle,
        &LoadedImageProtocol,
        (VOID**)&LoadedImage);

    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(BS->HandleProtocol, 3,
        LoadedImage->DeviceHandle,
        &FileSystemProtocol,
        (VOID**)&FileSystem);

    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) return Status;

    Status = uefi_call_wrapper(Root->Open, 5,
        Root,
        FileHandle,
        FileName,
        EFI_FILE_MODE_READ,
        0);

    return Status;
}

EFI_STATUS DisplayBMP32(EFI_FILE_HANDLE File, EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput) {
    EFI_STATUS Status;
    UINTN BufferSize;
    BMP_FILE_HEADER FileHeader;
    BMP_INFO_HEADER InfoHeader;

    // Чтение заголовков BMP
    BufferSize = sizeof(BMP_FILE_HEADER);
    Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, &FileHeader);
    if (EFI_ERROR(Status) || FileHeader.Signature != 0x4D42) {
        Print(L"Error reading BMP file header\n");
        return EFI_LOAD_ERROR;
    }

    BufferSize = sizeof(BMP_INFO_HEADER);
    Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, &InfoHeader);
    if (EFI_ERROR(Status)) {
        Print(L"Error reading BMP info header\n");
        return EFI_LOAD_ERROR;
    }

    if (InfoHeader.BitsPerPixel != 32 || InfoHeader.Compression != 0) {
        Print(L"Unsupported BMP format (must be 32-bit uncompressed)\n");
        return EFI_UNSUPPORTED;
    }

    UINT32 Width = InfoHeader.Width;
    UINT32 Height = InfoHeader.Height;

    // Устанавливаем позицию на пиксели
    Status = uefi_call_wrapper(File->SetPosition, 2, File, FileHeader.PixelDataOffset);
    if (EFI_ERROR(Status)) {
        Print(L"Error seeking to pixel data\n");
        return Status;
    }

    // Чтение изображения
    UINTN ImageSize = Width * Height * 4;
    VOID *BmpBuffer;
    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, ImageSize, &BmpBuffer);
    if (EFI_ERROR(Status)) {
        Print(L"Error allocating memory for BMP data\n");
        return Status;
    }

    BufferSize = ImageSize;
    Status = uefi_call_wrapper(File->Read, 3, File, &BufferSize, BmpBuffer);
    if (EFI_ERROR(Status) || BufferSize != ImageSize) {
        Print(L"Error reading BMP pixels\n");
        uefi_call_wrapper(BS->FreePool, 1, BmpBuffer);
        return EFI_LOAD_ERROR;
    }

    UINT8 *Src = (UINT8*)BmpBuffer;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Pixels = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)BmpBuffer;

    // Получаем размеры экрана
    UINT32 ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
    UINT32 ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;
    
    // Обрезаем, если изображение больше экрана
    UINT32 DrawWidth = Width > ScreenWidth ? ScreenWidth : Width;
    UINT32 DrawHeight = Height > ScreenHeight ? ScreenHeight : Height;

    for (UINT32 y = 0; y < DrawHeight; y++) {
        // Переход к строке снизу вверх (BMP — снизу вверх)
        EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Line = Pixels + (Height - 1 - y) * Width;

        // Обновим Blt чтобы рисовать только DrawWidth
        Status = uefi_call_wrapper(GraphicsOutput->Blt, 10,
            GraphicsOutput,
            Line,
            EfiBltBufferToVideo,
            0, 0,           // SourceX, SourceY
            0, y,           // DestinationX, DestinationY
            DrawWidth, 1,   // Width, Height
            0);

        if (EFI_ERROR(Status)) {
            Print(L"Error displaying line %u\n", y);
            break;
        }
    }
    uefi_call_wrapper(BS->FreePool, 1, BmpBuffer);

    return Status;
}
