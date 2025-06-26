#include "../gnu-efi/inc/efi.h"
#include "../gnu-efi/inc/efilib.h"
#include "../fonts/latin_font_8x16.h"
#include "../fonts/symbols_font_8x16.h"
#include "../fonts/numbers_font_8x16.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern EFI_STATUS Draw_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, CHAR16 Char, UINT32 x, UINT32 y, UINT32 Color);
extern EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color);
extern EFI_STATUS OpenBMPFile(EFI_HANDLE ImageHandle, EFI_FILE_HANDLE *FileHandle, CHAR16 *FileName);
extern EFI_STATUS DisplayBMP32(EFI_FILE_HANDLE File, EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
extern EFI_STATUS Create_File(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16* FilePath, UINTN TextLength, CHAR8* Text, UINTN Debug);
extern EFI_STATUS OpenRootFile(EFI_FILE_HANDLE *File, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem, CHAR16* File_Path);
extern EFI_STATUS Daemon_Log(CHAR16* Message);
extern EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height);
extern EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color);
extern EFI_STATUS Shell(CHAR16* Command);
extern EFI_STATUS CatFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath, CHAR16* Buffer);
extern EFI_STATUS HexCatFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath, CHAR16* Buffer);
extern EFI_STATUS ExecuteMachoKernel(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath);
extern UINTN Index;
extern UINTN Pixels;
extern UINT32 y;

/*********************************************************************
*********************************************************************/

EFI_TIME Startup_Time;

EFI_STATUS
Draw_Art(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput);

EFI_STATUS Draw_Window_Error(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput) {
    UINT32 Pitch = GraphicsOutput.Mode->Info->PixelsPerScanLine;

    UINT32 x = 640;
    UINT32 y = 380;

    UINT32 Scale = 5;

    UINT32 Color = 0xff272a28;

    UINT64 Window_Bitmap[64] = {
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111,
0b1111111111111111111111111111111111111111111111111111111111111111
    };

    for (UINTN Row = 0; Row < 64; Row++) {
        for (UINTN Col = 0; Col < 64; Col++) {
            if (Window_Bitmap[Row] & ((UINT64)1 << (63 - Col))) {
                for (UINTN dy = 0; dy < Scale; dy++) {
                    for (UINTN dx = 0; dx < Scale; dx++) {
                        UINT32 *Pixel = (UINT32 *)GraphicsOutput.Mode->FrameBufferBase;
                        UINTN screen_y = y + Row * Scale + dy;
                        UINTN screen_x = x + Col * Scale + dx;
                        Pixel[screen_y * Pitch + screen_x] = Color;
                    }
                }
            }
        }
    }

    x = 960;

    for (UINTN Row = 0; Row < 64; Row++) {
        for (UINTN Col = 0; Col < 64; Col++) {
            if (Window_Bitmap[Row] & ((UINT64)1 << (63 - Col))) {
                for (UINTN dy = 0; dy < Scale; dy++) {
                    for (UINTN dx = 0; dx < Scale; dx++) {
                        UINT32 *Pixel = (UINT32 *)GraphicsOutput.Mode->FrameBufferBase;
                        UINTN screen_y = y + Row * Scale + dy;
                        UINTN screen_x = x + Col * Scale + dx;
                        Pixel[screen_y * Pitch + screen_x] = Color;
                    }
                }
            }
        }
    }

    return EFI_SUCCESS;
}

void ScrollUp(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINT32 bg_color) {
    UINT32 width = GraphicsOutput->Mode->Info->HorizontalResolution;
    UINT32 height = GraphicsOutput->Mode->Info->VerticalResolution;
    UINT32 *framebuffer = (UINT32 *)GraphicsOutput->Mode->FrameBufferBase;

    UINT32 line_height = 18;

    for (UINT32 y = 0; y < height - line_height; y++) {
        for (UINT32 x = 0; x < width; x++) {
            framebuffer[y * width + x] = framebuffer[(y + line_height) * width + x];
        }
    }

    for (UINT32 y = height - line_height; y < height; y++) {
        for (UINT32 x = 0; x < width; x++) {
            framebuffer[y * width + x] = bg_color;
        }
    }
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    EFI_STATUS Status;

    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

    EFI_LOADED_IMAGE *LoadedImage;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;

    EFI_FILE_PROTOCOL *RootFS;

    Status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (void**)&GraphicsOutput);

    if (EFI_ERROR(Status)) {
        Print(L"mistake: mistake: failed to locate Graphics Output Protocol\n");
    }

    UINT32 ScreenWidth = GraphicsOutput->Mode->Info->HorizontalResolution;
    UINT32 ScreenHeight = GraphicsOutput->Mode->Info->VerticalResolution;

    /* if (ScreenWidth == 1920 && ScreenHeight == 1080) {
    	Print_Unicode(*GraphicsOutput, "Welcome to Pure!", 896, 1032, 0xffffffff);
    	uefi_call_wrapper(BS->Stall, 1, 5000000);
    } */

    Status = uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
    Status = Set_Screen(GraphicsOutput, 1920, 1080);
    uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2, SystemTable->ConOut, EFI_BACKGROUND_BLACK | EFI_WHITE);


    CHAR8 *Text = "Machokernel bootloader started";
    UINTN Length = 30;

    Status = Create_File(ImageHandle, SystemTable, L"\\tmp\\log.tmp", Length, Text, 1);
    
    EFI_HANDLE *Handles;
    UINTN HandleCount;
    Status = uefi_call_wrapper(SystemTable->BootServices->LocateHandleBuffer,
        5,
        ByProtocol,
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        &HandleCount,
        &Handles);

    if (EFI_ERROR(Status) || HandleCount == 0) {
        Print(L"mistake: failed to locate file system handles\n");
    }

    if (EFI_ERROR(Status)) {
        Print(L"mistake: couldn't the set screen resolution\n");
    }

    EFI_LOADED_IMAGE *Loaded_Image;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;

    Status = uefi_call_wrapper(
        SystemTable->BootServices->HandleProtocol,
        3,
        ImageHandle,
        &LoadedImageProtocol,
        (void **)&Loaded_Image
    );

    Print(L"Bootloader\n");

    CHAR16 Buffer[4096];

    Status = ExecuteMachoKernel(ImageHandle, SystemTable, L"\\boot\\kernel\\kernel.macho");

    /*

    CHAR16 *Command;
    UINTN Command_Index = 0;
    UINTN size = sizeof(CHAR16) * 1024;

    Status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, size, (VOID**)&Command);
    if (EFI_ERROR(Status)) {
        Print(L"mistake: couldn't allocate memory\n");
        return Status;
    }

    EFI_INPUT_KEY Key;

    while (true) {
    	uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3, 1, &SystemTable->ConIn->WaitForKey, NULL);
    	uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key);


    	if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
		Print(L"\n");
        	Command[Command_Index] = L'\0';
		if (StrCmp(Command, L"shutdown") == 0) {
                	SystemTable->RuntimeServices->ResetSystem(
    				EfiResetShutdown, // тип сброса — выключение
    				EFI_SUCCESS,       // статус
    				0,                 // размер данных
   				NULL               // данные
			);
        	} else {
                	Shell(Command);
        	}
        	Command_Index = 0;
    	} else if (Key.UnicodeChar != 0) {
        	Print(L"%c", Key.UnicodeChar);
        	Command[Command_Index] = Key.UnicodeChar;
        	Command_Index++;
    	}
    }

    Status = uefi_call_wrapper(BS->FreePool, 1, Command);
    if (EFI_ERROR(Status)) {
        Print(L"mistake: couldn't clear memory\n");
    }
    */
    while (true);
    
}
