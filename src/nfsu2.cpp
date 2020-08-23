#include <injector/injector.hpp>

namespace
{
double __fastcall CalculateVisualRep(void* self, int edx, int a2)
{
    const int& nCurrentStage = *reinterpret_cast<int*>(0x862838);
    switch(nCurrentStage)
    {
        case 0: return 0.0;
        case 1: return 1.0;
        case 2: return 3.0;
        case 3: return 5.5;
        case 4: return 8.0;
        case 5: return 10.0;
        case 6: return 10.0;
        default: return 10.0; // Unreachable code
    }
}
}

bool PatchNFSU2()
{
    if(injector::ReadMemory<uint8_t>(0x557EEB) != 0xE8
    || injector::ReadMemory<uint8_t>(0x5234C0) != 0x55)
        return false;

    // Remove reputation meter from the garage menu.
    injector::MakeNOP(0x557EDE, 0x557EF0 - 0x557EDE);
    /*
        .text:00557EDE 004        push    0
        .text:00557EE0 008        push    0
        .text:00557EE2 00C        push    0
        .text:00557EE4 010        push    offset "UI_Menu_Asset_Reputation.fng" 
        .text:00557EE9 014        push    0
        .text:00557EEB 018        call    ??DoSomethingWithFNG
    */

    // Define Visual Reputation based on current game stage.
    injector::MakeJMP(0x5234C0, injector::raw_ptr(CalculateVisualRep));
    /*
        .text:005234C0 ; double __thiscall ??CalculateVisualRep(_DWORD, _DWORD)
    */
    
    return true;
}
