#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <injector/injector.hpp>

static const char* szModName = "NFSU2 - No Visual Reputation";
static const char* szInvalidStage = "Invalid stage '%d'.\nThis is a bug, please report!";
static const char* szInvalidExe = "Unsupported game executable.\nPlease use 'NFS: Underground 2 v1.2 [ENGLISH] No-CD/Fixed EXE HOODLUM'";

int& nCurrentStage = *(int*)0x862838;

double __fastcall CalculateVisualRep(void* self, int edx, int a2)
{
    switch(nCurrentStage)
    {
        case 0: return 0.0;
        case 1: return 1.0;
        case 2: return 3.0;
        case 3: return 5.5;
        case 4: return 8.0;
        case 5: return 10.0;
        case 6: return 10.0;

        default:
        {
            char buf[512];
            sprintf(buf, szInvalidStage, nCurrentStage);
            MessageBoxA(NULL, buf, szModName, MB_ICONERROR);
            return 10.0;
        }
    }
}

extern "C" BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
)
{
    if(fdwReason == DLL_PROCESS_ATTACH)
    {
        // Check if this patch is supported on this executable.
        if(injector::ReadMemory<uint8_t>(0x557EEB, true) == 0xE8
        && injector::ReadMemory<uint8_t>(0x5234C0, true) == 0x55)
        {
            // Disable adding the 'UI_Menu_Asset_Reputation.fng' to the common menu,
            // thus removing the reputation meter during tuning.
            injector::MakeNOP(0x557EDE, 0x557EF0 - 0x557EDE);
            /*
                .text:00557EDE 004                 push    0
                .text:00557EE0 008                 push    0
                .text:00557EE2 00C                 push    0
                .text:00557EE4 010                 push    offset aUi_menu_asset_ ; "UI_Menu_Asset_Reputation.fng"
                .text:00557EE9 014                 push    0
                .text:00557EEB 018                 call    ??AddFNGToUIObject
            */

            // Define Visual Reputation based on current game stage.
            injector::MakeJMP(0x5234C0, injector::raw_ptr(CalculateVisualRep));
            /*
                .text:005234C0     ; double __thiscall CalculateVisualRep(_DWORD, _DWORD)
            */
        }
        else
        {
            MessageBoxA(NULL, szInvalidExe, szModName, MB_ICONERROR);
            return FALSE;
        }
    }
    return TRUE;
}
