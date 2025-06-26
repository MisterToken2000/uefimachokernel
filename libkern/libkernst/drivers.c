#include "../../gnu-efi/inc/efi.h"
#include "../../gnu-efi/inc/efilib.h"
#include <stdbool.h>

// extern EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color);
extern EFI_STATUS Draw_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, CHAR16 Char, UINT32 x, UINT32 y, UINT32 Color);
extern EFI_STATUS Print_Unicode(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, char *Text, UINT32 Index, UINT32 Line, UINT32 Color);
extern EFI_STATUS OpenBMPFile(EFI_HANDLE ImageHandle, EFI_FILE_HANDLE *FileHandle, CHAR16 *FileName);
extern EFI_STATUS DisplayBMP32(EFI_FILE_HANDLE File, EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
// extern EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
// extern EFI_STATUS Create_File(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16* FilePath, UINTN TextLength, CHAR8* Text);
// extern EFI_STATUS OpenRootFile(EFI_FILE_HANDLE *File, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem, CHAR16* File_Path);
// extern EFI_STATUS Daemon_Log(CHAR16* Message);
// extern EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height);
// extern EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color);
extern EFI_STATUS Shell(CHAR16* Command);
extern UINTN Index;
extern UINTN Pixels;
extern UINT32 y;

EFI_STATUS ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput){
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black = {0, 0, 0, 0};
    uefi_call_wrapper(GraphicsOutput->Blt, 10,
        GraphicsOutput,
        &Black,
        EfiBltVideoFill,
        0, 0,
        0, 0,
        GraphicsOutput->Mode->Info->HorizontalResolution,
        GraphicsOutput->Mode->Info->VerticalResolution,
        0);
    return EFI_SUCCESS;
}

EFI_STATUS OpenRootFile(EFI_FILE_HANDLE *File, EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem, CHAR16* File_Path) {
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL *Root;

    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open file system root\n");
        return Status;
    }

    Status = uefi_call_wrapper(Root->Open, 5, Root, File, File_Path, EFI_FILE_MODE_READ, 0);
    return EFI_SUCCESS;
}

EFI_STATUS Daemon_Log(CHAR16* Message){
        Print(L"log: %s\n", Message);
        return EFI_SUCCESS;
}

EFI_STATUS Set_Screen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, UINTN Width, UINTN Height){
    EFI_STATUS Status;
    UINTN SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    uint32_t ModeNumber;
    unsigned int i;
    for (i = 0; i < GraphicsOutput->Mode->MaxMode; i++) {
        Status = uefi_call_wrapper(GraphicsOutput->QueryMode, 4, GraphicsOutput, i, &SizeOfInfo, &Info);
        if (EFI_ERROR(Status)) {
            continue;
        }

        // Ищем нужное разрешение, например 1920x1080
        if (Info->HorizontalResolution == Width && Info->VerticalResolution == Height) {
            ModeNumber = i;
            // Переключаемся на нужный режим
            Status = uefi_call_wrapper(GraphicsOutput->SetMode, 2, GraphicsOutput, ModeNumber);
            if (!EFI_ERROR(Status)) {
                // Print(L"Resolution set to 1920x1080");
                uefi_call_wrapper(BS->Stall, 1, 200000);
            } else {
                Print(L"Failed to set resolution");
            }
            break;
        }
    }
    return EFI_SUCCESS;
}

EFI_STATUS Set_Background_Color(EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsOutput, UINT32 Color) {
    UINT32 Pitch = GraphicsOutput.Mode->Info->PixelsPerScanLine;
    UINT32 x = 0;
    UINT32 y = 0;
    UINT32 Scale = 1920;

    UINT8 Pix[1] = {0b10000000}; // закрашен самый левый пиксель

    UINT32 *FrameBuffer = (UINT32 *)GraphicsOutput.Mode->FrameBufferBase;

    for (UINTN Row = 0; Row < 1; Row++) {
        for (UINTN Col = 0; Col < 8; Col++) {
            if (Pix[Row] & (1 << (7 - Col))) {
                for (UINTN dy = 0; dy < Scale; dy++) {
                    for (UINTN dx = 0; dx < Scale; dx++) {
                        UINTN screen_y = y + Row * Scale + dy;
                        UINTN screen_x = x + Col * Scale + dx;
                        FrameBuffer[screen_y * Pitch + screen_x] = Color;
                    }
                }
            }
        }
    }

    return EFI_SUCCESS;
}

EFI_STATUS Create_File(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable, CHAR16* FilePath, UINTN TextLength, CHAR8* Text, UINTN Debug)
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL *RootFS;
    EFI_FILE_PROTOCOL *File;

    // Get info about image
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3,
                               ImageHandle, &LoadedImageProtocol, (void **)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get loaded image: %r\n", Status);
        return Status;
    }

    // Get File System
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3,
                               LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)&FileSystem);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get file system: %r\n", Status);
        return Status;
    }

    // Open Root
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &RootFS);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open file system volume: %r\n", Status);
        return Status;
    }

    // Open (create) file
    Status = uefi_call_wrapper(RootFS->Open, 5, RootFS, &File,
                               FilePath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open/create file: %r\n", Status);
        return Status;
    }

    // Writing
    Status = uefi_call_wrapper(File->Write, 3, File, &TextLength, Text);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to write to file: %r\n", Status);
        uefi_call_wrapper(File->Close, 1, File);
        return Status;
    }

    uefi_call_wrapper(File->Close, 1, File);
    if (Debug == 1) {
    	Print(L"File writted: %s\n", FilePath);
    } else if (Debug == 0) {
    	Print(L"");
    } else {
    	Print(L"\nUnknown Number\n");
    }
    return EFI_SUCCESS;
}

EFI_STATUS CatFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath, CHAR16 *Buffer) {
    EFI_STATUS Status;
    EFI_FILE_IO_INTERFACE *FileSystem;
    EFI_FILE_HANDLE Root, File;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
    EFI_GUID FileSystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    // Получаем информацию о загруженном образе
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, ImageHandle, &LoadedImageProtocol, (void **)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get LoadedImage protocol\n");
        return Status;
    }

    // Получаем файловую систему
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, LoadedImage->DeviceHandle, &FileSystemProtocol, (void **)&FileSystem);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get FileSystem protocol\n");
        return Status;
    }

    // Открываем корневую директорию
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open root directory\n");
        return EFI_SUCCESS;
    }

    // Открываем файл
    Status = uefi_call_wrapper(Root->Open, 5, Root, &File, FilePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open file\n");
        return EFI_SUCCESS;
    }

    // Получаем размер файла
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    FileInfo = AllocatePool(FileInfoSize);
    Status = uefi_call_wrapper(File->GetInfo, 4, File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get file info\n");
        return Status;
    }

    UINTN FileSize = FileInfo->FileSize;
    VOID *buffer = AllocatePool(FileSize);

    // Читаем содержимое файла в буфер
    Status = uefi_call_wrapper(File->Read, 3, File, &FileSize, buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot read file\n");
        return Status;
    }

    // Пример вывода первых байт
    // Print(L"Read %d bytes from %s.\n", FileSize, FilePath);

    CHAR8 *TextBuffer = (CHAR8 *)buffer;

    UINTN Index = 0;

    for (UINTN i = 0; i < FileSize; i += 16) {
        for (UINTN j = 0; j < 16; j++) {
                if (i + j < FileSize) {
                        CHAR8 c = TextBuffer[i + j];
                        if (c >= 32 && c <= 126) {
                                Print(L"%c", c);
                                Buffer[Index++] = (CHAR16)c;
                        } else {
                                Print(L"");
                        }
                }
        }

        Print(L"\n");
    }

    Buffer[Index] = 0;

    // Не забудьте освободить ресурсы
    FreePool(buffer);
    FreePool(FileInfo);
    uefi_call_wrapper(File->Close, 1, File);
    uefi_call_wrapper(Root->Close, 1, Root);
    return EFI_SUCCESS;
}

EFI_STATUS HexCatFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath, CHAR16 *Buffer) {
    EFI_STATUS Status;
    EFI_FILE_IO_INTERFACE *FileSystem;
    EFI_FILE_HANDLE Root, File;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
    EFI_GUID FileSystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    // Получаем информацию о загруженном образе
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, ImageHandle, &LoadedImageProtocol, (void **)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get LoadedImage protocol\n");
        return Status;
    }

    // Получаем файловую систему
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, LoadedImage->DeviceHandle, &FileSystemProtocol, (void **)&FileSystem);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get FileSystem protocol\n");
        return Status;
    }

    // Открываем корневую директорию
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open root directory\n");
        return EFI_SUCCESS;
    }

    // Открываем файл
    Status = uefi_call_wrapper(Root->Open, 5, Root, &File, FilePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open file\n");
        return EFI_SUCCESS;
    }

    // Получаем размер файла
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    FileInfo = AllocatePool(FileInfoSize);
    Status = uefi_call_wrapper(File->GetInfo, 4, File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get file info\n");
        return Status;
    }

    UINTN FileSize = FileInfo->FileSize;
    VOID *buffer = AllocatePool(FileSize);

    // Читаем содержимое файла в буфер
    Status = uefi_call_wrapper(File->Read, 3, File, &FileSize, buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot read file\n");
        return Status;
    }

    // Пример вывода первых байт
    // Print(L"Read %d bytes from %s.\n", FileSize, FilePath);

    CHAR8 *TextBuffer = (CHAR8 *)buffer;

    UINTN Index = 0;

    for (UINTN i = 0; i < FileSize; i += 16) {
        Print(L"%08x: ", i);

        for (UINTN j = 0; j < 16; j++) {
                if (i + j < FileSize) {
                        Print(L"%02x ", TextBuffer[i + j] & 0xff);
                } else {
                        Print(L"    ");
                }
        }
        Print(L" ");


        for (UINTN j = 0; j < 16; j++) {
                if (i + j < FileSize) {
                        CHAR8 c = TextBuffer[i + j];
                        if (c >= 32 && c <= 126) {
                                Print(L"%c", c);
                                Buffer[Index++] = (CHAR16)c;
                        } else {
                                Print(L".");
                        }
                }
        }

        Print(L"\n");
    }

    Buffer[Index] = 0;

    // Не забудьте освободить ресурсы
    FreePool(buffer);
    FreePool(FileInfo);
    uefi_call_wrapper(File->Close, 1, File);
    uefi_call_wrapper(Root->Close, 1, Root);
    return EFI_SUCCESS;
}

EFI_STATUS ExecuteCommand(EFI_STATUS Status, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
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
}

EFI_STATUS ExecuteMachoKernel(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable, CHAR16* FilePath) {
    EFI_STATUS Status;
    EFI_FILE_IO_INTERFACE *FileSystem;
    EFI_FILE_HANDLE Root, File;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_GUID LoadedImageProtocol = LOADED_IMAGE_PROTOCOL;
    EFI_GUID FileSystemProtocol = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

    Status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol,
        3,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        (void**)&GraphicsOutput);

    if (EFI_ERROR(Status)) {
        Print(L"mistake: mistake: failed to locate Graphics Output Protocol\n");
    }

    // Получаем информацию о загруженном образе
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, ImageHandle, &LoadedImageProtocol, (void **)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get LoadedImage protocol\n");
        return Status;
    }

    // Получаем файловую систему
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
        3, LoadedImage->DeviceHandle, &FileSystemProtocol, (void **)&FileSystem);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get FileSystem protocol\n");
        return Status;
    }

    // Открываем корневую директорию
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2, FileSystem, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open root directory\n");
        return EFI_SUCCESS;
    }

    // Открываем файл
    Status = uefi_call_wrapper(Root->Open, 5, Root, &File, FilePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot open file\n");
        return EFI_SUCCESS;
    }

    // Получаем размер файла
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    FileInfo = AllocatePool(FileInfoSize);
    Status = uefi_call_wrapper(File->GetInfo, 4, File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot get file info\n");
        return Status;
    }

    UINTN FileSize = FileInfo->FileSize;
    VOID *buffer = AllocatePool(FileSize);

    // Читаем содержимое файла в буфер
    Status = uefi_call_wrapper(File->Read, 3, File, &FileSize, buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Cannot read file\n");
        return Status;
    }

    // Пример вывода первых байт
    Print(L"Read %d bytes from %s.\n", FileSize, FilePath);

    CHAR16 Buffer[4096];
    CHAR8 *TextBuffer = (CHAR8 *)buffer;

    UINTN Index = 0;
    UINTN LineIndex = 0;

    Status = uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
    Status = Set_Screen(GraphicsOutput, 1920, 1080);
    uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2, SystemTable->ConOut, EFI_BACKGROUND_BLACK | EFI_WHITE);

    for (UINTN i = 0; i < FileSize; i += 16) {
	for (UINTN a = 0; a < 4096; a++) Buffer[a] = 0;
	Index = 0;

	for (UINTN j = 0; j < 16; j++) {
                if (i + j < FileSize) {
                        CHAR8 c = TextBuffer[i + j];
                        if (c >= 32 && c <= 126) {
                                Buffer[Index++] = (CHAR16)c;
                        }
                }
        }

	if (StrCmp(Buffer, L"machokrnl") == 0 && LineIndex == 0) {
		Print(L"\nMacho Confirmed\n");
		Status = uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
		Status = Set_Screen(GraphicsOutput, 1920, 1080);
		uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2, SystemTable->ConOut, EFI_BACKGROUND_BLACK | EFI_WHITE);
	} else if (StrCmp(Buffer, L"machokrnl") > 0 && LineIndex == 0) {
		Print(L"\nMacho header not confirmed\n");
		break;
	} else if (StrCmp(Buffer, L"PZ") == 0) {
		CHAR8 *Text = "Macho Kernel Started";
    		UINTN Length = 20;
		
    		Status = Create_File(ImageHandle, SystemTable, L"\\tmp\\log1.tmp", Length, Text, 0);	
	} else if (StrCmp(Buffer, L"PE") == 0) {
		Print(L"Welcome to Mach-O Kernel!\n");
	} else if (StrCmp(Buffer, L"____InModeKernel") == 0) {
		Status = ExecuteCommand(Status, ImageHandle, SystemTable);
	} else {
		Print(L"\nUnknown Command\nBuffer: %s\n", Buffer);
		break;
	}
	LineIndex++;
    }

    Buffer[Index] = 0;

    // Не забудьте освободить ресурсы
    FreePool(buffer);
    FreePool(FileInfo);
    FreePool(Buffer);
    FreePool(TextBuffer);
    uefi_call_wrapper(File->Close, 1, File);
    uefi_call_wrapper(Root->Close, 1, Root);
    return EFI_SUCCESS;
}
