#include "../../gnu-efi/inc/efi.h"
#include "../../gnu-efi/inc/efilib.h"

extern EFI_STATUS Draw_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, CHAR16 Char, UINT32 x, UINT32 y, UINT32 Color);
extern EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color);
extern EFI_STATUS OpenBMPFile(EFI_HANDLE ImageHandle, EFI_FILE_HANDLE *FileHandle, CHAR16 *FileName);
extern EFI_STATUS DisplayBMP32(EFI_FILE_HANDLE File, EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS OpenRootFile(EFI_FILE_HANDLE *File, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem, CHAR16* File_Path);
extern EFI_STATUS Daemon_Log(CHAR16* Message);
extern EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height);
extern EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color);
extern void ScrollUp(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINT32 bg_color);
extern EFI_TIME Startup_Time;
extern EFI_STATUS Status;
extern UINTN Index;
extern UINTN Pixels;
extern UINT32 y;

EFI_STATUS Shell(CHAR16* Command, EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput) {
    if (StrCmp(Command, L"date") == 0) {
    	Status = uefi_call_wrapper(RT->GetTime, 2, &Startup_Time, NULL);
    	if (EFI_ERROR(Status)) {
        	Print(L"get time error: %r\n", Status);
    	}
	Print(L"%04d-%02d-%02d %02d:%02d:%02d\n", Startup_Time.Year, Startup_Time.Month, Startup_Time.Day, Startup_Time.Hour, Startup_Time.Minute, Startup_Time.Second);
    } else {
        Print(L"sh: unknown command: %s\n", Command);
    }
    return EFI_SUCCESS;
}
