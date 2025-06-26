#include "../gnu-efi/inc/efi.h"
#include "../gnu-efi/inc/efilib.h"
#include "../fonts/latin_font_8x16.h"
#include "../fonts/symbols_font_8x16.h"
#include "../fonts/numbers_font_8x16.h"
#include <stdbool.h>

extern EFI_STATUS Draw_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, CHAR16 Char, UINT32 x, UINT32 y, UINT32 Color);
extern EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color);
extern EFI_STATUS OpenBMPFile(EFI_HANDLE ImageHandle, EFI_FILE_HANDLE *FileHandle, CHAR16 *FileName);
extern EFI_STATUS DisplayBMP32(EFI_FILE_HANDLE File, EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS OpenRootFile(EFI_FILE_HANDLE *File, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem, CHAR16* File_Path);
extern EFI_STATUS Daemon_Log(CHAR16* Message);
extern EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height);
extern EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color);
extern UINTN Index;
extern UINTN Pixels;
extern UINT32 y;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_LOADED_IMAGE *LoadedImage = NULL;
    EFI_GUID LoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_DEVICE_PATH *DevicePath;
    EFI_HANDLE NewImageHandle;

    InitializeLib(ImageHandle, SystemTable);
  
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

    Status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (void**)&GraphicsOutput);


    Status = uefi_call_wrapper(BS->HandleProtocol, 3,
                              ImageHandle,
                              &LoadedImageProtocol,
                              (void**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"not find LoadedImageProtocol: %r\n", Status);
        while (true);
    }

    // Вывод текста и задержка идут ДО загрузки kernel64.efi
    Status = Set_Screen(GraphicsOutput, 1920, 1080);
    Print_Unicode(*GraphicsOutput, "Booting...", 930, 1000, 0xffffffff);
    uefi_call_wrapper(BS->Stall, 1, 1000000);
    Status = Set_Screen(GraphicsOutput, 1920, 1080);
    
    EFI_LOADED_IMAGE *Loaded_Image;
    EFI_GUID Loaded_Image_Protocol = LOADED_IMAGE_PROTOCOL;

    Status = uefi_call_wrapper(
        SystemTable->BootServices->HandleProtocol,
        3,
        ImageHandle,
        &Loaded_Image_Protocol,
        (void **)&Loaded_Image
    );
    Print(L"boot file path: \\kernel\\kernel64.efi\n");
    uefi_call_wrapper(BS->Stall, 1, 500000);


    DevicePath = FileDevicePath(LoadedImage->DeviceHandle, L"\\kernel\\kernel64.efi");

    if (!DevicePath) {
        Print(L"ERROR: Cannot create path kernel64.efi\n");
        return EFI_NOT_FOUND;
    }

    Status = uefi_call_wrapper(BS->LoadImage, 6,
                              FALSE,
                              ImageHandle,
                              DevicePath,
                              NULL,
                              0,
                              &NewImageHandle);

    if (EFI_ERROR(Status)) {
        Print(L"ERROR: boot failed kernel64.efi");
        while (true);
    }

    Status = uefi_call_wrapper(BS->StartImage, 3,
                              NewImageHandle,
                              NULL,
                              NULL);

    if (EFI_ERROR(Status)) {
        Print(L"ERROR: kernel64.efi not loaded: %r\n", Status);
        while (true);
    }

    return EFI_SUCCESS;
}
