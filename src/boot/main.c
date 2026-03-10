#include <efi.h>
#include <efilib.h>
#include <elf.h>

#define KERNEL_PATH L"\\kernel.elf"

// ELF yükleyici
EFI_STATUS
LoadElfKernel(
    EFI_SYSTEM_TABLE      *ST,
    UINT8                 *ElfBuffer,
    UINTN                  ElfSize,
    EFI_PHYSICAL_ADDRESS  *EntryPoint
) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr*)ElfBuffer;
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3 ||
        ehdr->e_type   != ET_EXEC    ||
        ehdr->e_machine!= EM_X86_64) {
        return EFI_LOAD_ERROR;
    }

    Elf64_Phdr *phdr = (Elf64_Phdr*)(ElfBuffer + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type != PT_LOAD) continue;
        EFI_PHYSICAL_ADDRESS seg_addr = phdr[i].p_paddr;
        UINTN memsz = phdr[i].p_memsz, filesz = phdr[i].p_filesz;
        UINTN off   = phdr[i].p_offset;

        EFI_STATUS st = uefi_call_wrapper(
            ST->BootServices->AllocatePages, 4,
            AllocateAddress, EfiLoaderData,
            EFI_SIZE_TO_PAGES(memsz), &seg_addr
        );
        if (EFI_ERROR(st)) return st;

        CopyMem((VOID*)(UINTN)seg_addr,
                ElfBuffer + off,
                filesz);
        if (memsz > filesz) {
            SetMem((VOID*)((UINTN)seg_addr + filesz),
                   memsz - filesz, 0);
        }
    }

    *EntryPoint = ehdr->e_entry;
    return EFI_SUCCESS;
}

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS                       Status;
    EFI_LOADED_IMAGE                *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    EFI_FILE_PROTOCOL               *RootFS = NULL, *KernelFile = NULL;
    EFI_FILE_INFO                   *FileInfo;
    UINTN                             FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    VOID                            *FileInfoBuf;
    EFI_PHYSICAL_ADDRESS             ReadBuffer;
    UINTN                            KernelSize;

    EFI_MEMORY_DESCRIPTOR           *MemoryMap = NULL;
    UINTN                            MemoryMapSize = 0;
    UINTN                            MapKey = 0;
    UINTN                            DescriptorSize = 0;
    UINT32                           DescriptorVersion = 0;

    EFI_GRAPHICS_OUTPUT_PROTOCOL    *Gop;

    InitializeLib(ImageHandle, SystemTable);

    // [1] LoadedImageProtocol
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3,
                               ImageHandle, &LoadedImageProtocol,
                               (void**)&LoadedImage);
    Print(L"[1] LoadedImage: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;

    // [2] FileSystemProtocol
    Status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol, 3,
                               LoadedImage->DeviceHandle,
                               &FileSystemProtocol, (void**)&FileSystem);
    Print(L"[2] FileSystem: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;

    // [3] OpenVolume
    Status = uefi_call_wrapper(FileSystem->OpenVolume, 2,
                               FileSystem, &RootFS);
    Print(L"[3] OpenVolume: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;

    // [4] Open kernel.elf
    Status = uefi_call_wrapper(RootFS->Open, 5,
                               RootFS, &KernelFile,
                               KERNEL_PATH, EFI_FILE_MODE_READ, 0);
    Print(L"[4] Open kernel.elf: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;

    // [5] GetInfo
    FileInfoBuf = AllocatePool(FileInfoSize);
    Status = uefi_call_wrapper(KernelFile->GetInfo, 4,
                               KernelFile, &gEfiFileInfoGuid,
                               &FileInfoSize, FileInfoBuf);
    Print(L"[5] GetInfo: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;
    FileInfo = (EFI_FILE_INFO*)FileInfoBuf;
    KernelSize = FileInfo->FileSize;
    FreePool(FileInfoBuf);

    // [6] Read buffer allocate
    ReadBuffer = 0x200000;
    Status = uefi_call_wrapper(SystemTable->BootServices->AllocatePages, 4,
                               AllocateAddress, EfiLoaderData,
                               EFI_SIZE_TO_PAGES(KernelSize),
                               &ReadBuffer);
    Print(L"[6] Alloc read buf: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;

    // [7] kernel.elf oku
    Status = uefi_call_wrapper(KernelFile->Read, 3,
                               KernelFile, &KernelSize,
                               (VOID*)(UINTN)ReadBuffer);
    Print(L"[7] Read kernel: %r\n", Status);
    if (EFI_ERROR(Status)) return Status;
    uefi_call_wrapper(KernelFile->Close, 1, KernelFile);

    // [8] ELF yükleyici
    EFI_PHYSICAL_ADDRESS KernelEntry;
    Status = LoadElfKernel(SystemTable,
                           (UINT8*)(UINTN)ReadBuffer,
                           KernelSize,
                           &KernelEntry);
    Print(L"[8] ELF Load: %r, Entry=0x%lx\n", Status, KernelEntry);
    if (EFI_ERROR(Status)) return Status;

    // [9] GOP protokolü al
    Status = uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3,
                               &gEfiGraphicsOutputProtocolGuid,
                               NULL, (void**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP bulunamadi: %r\n", Status);
        return Status;
    }

    // [10] Memory map & ExitBootServices — tek adımda yapılmalı
    Status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5,
                               &MemoryMapSize, NULL,
                               &MapKey, &DescriptorSize,
                               &DescriptorVersion);
    if (Status != EFI_BUFFER_TOO_SMALL) {
        Print(L"[9a] GetMemoryMap (size only): %r\n", Status);
        return Status;
    }

    MemoryMapSize += DescriptorSize * 4;
    MemoryMap = AllocatePool(MemoryMapSize);
    if (!MemoryMap) {
        Print(L"[9b] MemoryMap AllocatePool FAILED\n");
        return EFI_OUT_OF_RESOURCES;
    }

    Status = uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5,
                               &MemoryMapSize, MemoryMap,
                               &MapKey, &DescriptorSize,
                               &DescriptorVersion);
    if (EFI_ERROR(Status)) {
        Print(L"[9c] GetMemoryMap final: %r\n", Status);
        return Status;
    }

    Status = uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2,
                               ImageHandle, MapKey);
    if (EFI_ERROR(Status)) {
        Print(L"[9d] ExitBootServices FAILED: %r\n", Status);
        return Status;
    }
    UINT32 pixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;

    // [11] Kernel'i başlat - kernel_main(void* framebuffer, uint32_t width, uint32_t height)
    ((void(*)(void*, uint32_t, uint32_t, uint32_t))KernelEntry)(
        (void*)(UINTN)Gop->Mode->FrameBufferBase,
        Gop->Mode->Info->HorizontalResolution,
        Gop->Mode->Info->VerticalResolution,
        Gop->Mode->Info->PixelsPerScanLine * 4  // pitch = satır başına byte
    );

    Print(L"[11] Kernel basladi\n");
    return EFI_SUCCESS;
}
