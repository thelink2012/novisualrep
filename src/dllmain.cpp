#include <windows.h>

extern bool PatchNFSU();
extern bool PatchNFSU2();

namespace
{
const char szModName[] = "NFSU/NFSU2 Visual Reputation Remover";
const char szInvalidExe[] = "Unsupported game executable.\n"
                             "Please make sure you are using one of "
                             "the following:\n"
                             "NFS: Underground v1.4 [US] "
                             "No-CD/Fixed EXE [SPiRiTY]\n"
                             "NFS: Underground 2 v1.2 [ENGLISH] "
                             "No-CD/Fixed EXE [HOODLUM]";

bool PatchAppropriateGame()
{
    const auto uImageBase = reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
    const auto* lpDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(uImageBase);
    const auto* lpNtHeader =
      reinterpret_cast<IMAGE_NT_HEADERS*>(uImageBase + lpDosHeader->e_lfanew);
    const auto uAddressOfEntryPoint = uImageBase +
                                lpNtHeader->OptionalHeader.AddressOfEntryPoint +
                                (0x400000 - uImageBase);

    switch(uAddressOfEntryPoint)
    {
        case 0x670CB5: // NFS: Underground v1.4 [US]
            return PatchNFSU();
        case 0x75BCC7: // NFS: Underground 2 v1.2 [ENGLISH]
            return PatchNFSU2();
        default:
            return false;
    }
}
}

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if(fdwReason == DLL_PROCESS_ATTACH)
    {
        if(!PatchAppropriateGame())
        {
            MessageBoxA(0, szInvalidExe, szModName, MB_ICONERROR);
            return FALSE;
        }
    }
    return TRUE;
}