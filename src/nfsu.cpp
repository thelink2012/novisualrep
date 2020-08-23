#include <injector/injector.hpp>
#include <injector/calling.hpp>
#include <cassert>

#ifdef NDEBUG
#define DebugPrint(...) ((void)0)
#else
#define DebugPrint(...) do { char szBuf[1024]; \
                             sprintf(szBuf, __VA_ARGS__); \
                             OutputDebugStringA(szBuf); \
                           } while(0)
#endif

namespace
{
struct RaceReputationEntry
{
    uint8_t m_ubEventId;
    double m_fReputationDiff;
};

struct RaceMagazineEntry
{
    uint8_t m_ubEventId;
    void (*m_fnUnlockReward)(int iUnknownId);
};

void UnlockMagazine15(int);
void UnlockMagazine16(int);
void UnlockMagazine17(int);
void UnlockMagazine18(int);
void UnlockMagazine19(int);

// The star reputation of the player is increased as he installs Visual
// Upgrades in his car. One may not want to do so, therefore we scratch
// that system in favour of the following bump table. The table is built
// as if as soon as Visual Upgrades are unlocked the player went on and
// installed them on their car.
const RaceReputationEntry aRaceReputationTable[] {
    // Level 1 Parts are available from the beggining of UG.
    // Installing all of them achieves a total reputation of 2.048780441
    // but we are going to distribute these points across some races.
    { 0, 0.128048778 },           // Total Reputation: 0.128048778
    { 1, 0.128048778 },           // Total Reputation: 0.256097555
    { 2, 0.128048778 },           // Total Reputation: 0.384146333
    { 3, 0.128048778 },           // Total Reputation: 0.512195110
    { 4, 0.128048778 },           // Total Reputation: 0.640243888
    { 5, 0.128048778 },           // Total Reputation: 0.768292665
    { 6, 0.128048778 },           // Total Reputation: 0.896341443
    { 7, 0.128048778 },           // Total Reputation: 1.024390220
    { 8, 0.128048778 },           // Total Reputation: 1.152438998
    { 9, 0.128048778 },           // Total Reputation: 1.280487776
    { 10, 0.128048778 },          // Total Reputation: 1.408536553
    { 11, 0.128048778 },          // Total Reputation: 1.536585331
    { 12, 0.128048778 },          // Total Reputation: 1.664634108
    { 13, 0.128048778 },          // Total Reputation: 1.792682886
    { 14, 0.128048778 },          // Total Reputation: 1.920731663
    { 15, 0.128048778 },          // Total Reputation: 2.048780441
    // UG Event #18 Unlocks Level 2 Neon
    { 18, 0.097561121 },          // Total Reputation: 2.146341562
    // UG Event #23 Unlocks Level 2 Paint
    { 23, 0.146341324 },          // Total Reputation: 2.292682886
    // UG Event #28 Unlocks Level 2 Muffler Tip
    { 28, 0.097561121 },          // Total Reputation: 2.390244007
    // UG Event #29 Unlocks Level 2 Window Tint
    { 29, 0.048780441 },          // Total Reputation: 2.439024448
    // UG Event #31 Unlocks Level 2 Roof Scoop
    { 31, 0.2926828869999998 },   // Total Reputation: 2.731707335
    // UG Event #34 Unlocks Level 2 Rim
    { 34, 0.19512200300000027 },  // Total Reputation: 2.926829338
    // UG Event #39 Unlocks Level 2 Side Skirt
    { 39, 0.19512176499999967 },  // Total Reputation: 3.121951103
    // UG Event #43 Unlocks Level 2 Hood
    { 43, 0.19512224200000006 },  // Total Reputation: 3.317073345
    // UG Event #44 Unlocks Level 2 Front Bumper
    { 44, 0.2439022070000001 },   // Total Reputation: 3.560975552
    // UG Event #45 Unlocks Level 2 Spoiler
    { 45, 0.19512200300000027 },  // Total Reputation: 3.756097555
    // UG Event #49 Unlocks Level 2 Rear Bumper
    { 49, 0.2439024449999998 },   // Total Reputation: 4.0
    // UG Event #54 Unlocks Level 3 Muffler Tip & Window Tint
    { 54, 0.0416665079999996 },   // Total Reputation: 4.041666508
    // UG Event #60 Unlocks Level 3 Headlight
    { 60, 0.125 },                // Total Reputation: 4.166666508
    // UG Event #61 Unlocks Level 3 Taillights
    { 61, 0.125 },                // Total Reputation: 4.291666508
    // UG Event #62 Unlocks Level 3 Neon
    { 62, 0.041666984000000795 }, // Total Reputation: 4.333333492
    // UG Event #65 Unlocks Level 3 Paint
    { 65, 0.0625 },               // Total Reputation: 4.395833492
    // UG Event #70 Unlocks Level 3 Roof Scoop
    { 70, 0.0625 },               // Total Reputation: 4.458333492
    // UG Event #81 Unlocks Level 3 Rim
    { 81, 0.0833330159999992 },   // Total Reputation: 4.541666508
    // UG Event #84 Unlocks Level 3 Side Skirt
    { 84, 0.0833334920000004 },   // Total Reputation: 4.625
    // UG Event #89 Unlocks Level 3 Hood
    { 89, 0.0833334920000004 },   // Total Reputation: 4.708333492
    // UG Event #90 Unlocks Level 3 Front Bumper
    { 90, 0.1041665079999996 },   // Total Reputation: 4.8125
    // UG Event #93 Unlocks Level 3 Spoilers
    { 93, 0.0833334920000004 },   // Total Reputation: 4.895833492
    // UG Event #100 Unlocks Level 3 Rear Bumper
    { 100, 0.1041665079999996 },  // Total Reputation: 5.0
};

// The magazines listed in this table are unlocked by installing certain
// Visual Upgrades in a car. A player mayn't want to do so, therefore we
// give the magazine away a certain number of events after the event
// that unlocked the last upgrade necessary to build a car for such a
// magazine. If the player hasn't built a car using such upgrades that
// far, it's likely he's not interested in doing so.
const RaceMagazineEntry aRaceMagazineTable[] {

    // NOTE: These should be events that are the sole events in the 
    // Race Map once they are unlocked. See IsPreviousEventCompleted for
    // a reasoning. Otherwise, the said function must be tweaked to
    // check for more than one previous event.

    // Would require building a 1 star reputation car (~3 upgrades).
    // Possible to achieve since the beggining of UG.
    //
    // We give it away in a Drag Tournament which is ~20 events after it
    // becomes possible to build a car for this magazine.
    { 20, UnlockMagazine15 },
    
    // Would require building a car with all Level 1 Visual Upgrades.
    // Possible to achieve since the beggining of UG.
    //
    // We give it away in a Circuit Tournament which is 10 events after
    // the previous entry in this table.
    { 30, UnlockMagazine16 },
    
    // Would require building a car with all Level 2 Visual Upgrades.
    // Possible to achieve since UG Event #49.
    //
    // We give it away in a Drag Tournament which is ~20 events after it
    // becomes possible to build a car for this magazine.
    { 80, UnlockMagazine17 },
    
    // Would require building a car with all Level 3 Visual Upgrades.
    // Possible to achieve since UG Event #100.
    //
    // We give this magazine away very closely to the event that makes
    // it possible to build a car for it. This is a deliberate choice.
    // The races following UG Event #105 have some kind of storyline
    // importance for which the magazine could be a distraction.
    { 105, UnlockMagazine18 },
    
    // Would require building a car with all Level 3 Visual Upgrades and
    // all four Unique Visual Upgrades. Possible to achieve since
    // UG Event #110 [?not sure].
    //
    // We give this magazine away very closely to the event that makes
    // it possible to build a car for it but we have no choice, since
    // the final race is UG Event #112 itself.
    { 112, UnlockMagazine19 },
};

bool IsEventCompleted(uint8_t ubEventId)
{
    const int MAX_EVENTS = 114;
    const auto aEventState = reinterpret_cast<uint8_t*>(0x75F244);
    
    enum eEventState : uint8_t
    {
        EVENT_STATE_LOCKED = 0,
        EVENT_STATE_UNLOCKED_UNRACED = 1,
        EVENT_STATE_COMPLETED = 2,
        EVENT_STATE_UNLOCKED_RACING = 3,
        EVENT_STATE_SKIPPED = 4,
        EVENT_STATE_UNLOCKED_RACED = 5,
    };

    assert(ubEventId >= 0 && ubEventId < MAX_EVENTS);
    const auto ubEventState = aEventState[ubEventId];
    
    return ubEventState == EVENT_STATE_COMPLETED ||
            ubEventState == EVENT_STATE_SKIPPED;
}

bool IsPreviousEventCompleted(uint8_t ubEventId)
{
    // This is a hack, but it should work.
    //
    // Since our magazine table specifies the event on which the
    // magazine should be received, and the event completion is only
    // marked at the loading screen after the race, we actually check
    // for the races completed before the race of interest.
    //
    // Better, we check for a single race before the interesting one.
    // This works fine because all events in the magazine table are
    // the sole events in the Race Map once they are unlocked. Therefore
    // if the previous event was completed, all others were too.
    assert(ubEventId > 0);
    return IsEventCompleted(ubEventId - 1);
}

void ProcessMagazineGiveaway()
{
    // Based off sub_51B060
    
    const auto fnComputeUnkHash = injector::fastcall<int(int, const char*)>::call<0x567C70>;
    const auto fnComputeUnkIndex = injector::fastcall<int(int)>::call<0x51A8B0>;
    
    const auto iUnknownHash = fnComputeUnkHash(0, "MAGAZINE_TASK_1");
    const auto iUnknownId = fnComputeUnkIndex(iUnknownHash);
    
    DebugPrint("iUnknownHash = %d\n", iUnknownHash);
    DebugPrint("iUnknownId = %d\n", iUnknownId);
    
    for(const auto& magEntry : aRaceMagazineTable)
    {
        if(IsPreviousEventCompleted(magEntry.m_ubEventId))
            magEntry.m_fnUnlockReward(iUnknownId);
    }
}

double __cdecl CalculateVisualRep(/*int a1@<edi>*/)
{
    // The magazine giveaway could be hooked into many places of the
    // game, but for simplicity, it is placed here. It should be good
    // enough since the calculation of visual reputation is done every
    // frame in the Underground Mode Menu.
    ProcessMagazineGiveaway();
    
    const double MIN_REPUTATION = 0.1935483813; // Stock Parts
    const double MAX_REPUTATION = 5.0;
    
    double fTotalRep = 0.0;
    
    for(const auto& repEntry : aRaceReputationTable)
    {
        if(IsEventCompleted(repEntry.m_ubEventId))
            fTotalRep += repEntry.m_fReputationDiff;
    }
    
    if(fTotalRep < MIN_REPUTATION)
        fTotalRep = MIN_REPUTATION;
    
    if(fTotalRep > MAX_REPUTATION)
        fTotalRep = MAX_REPUTATION;

    DebugPrint("CalculateVisualRep() = %f\n", fTotalRep);
    return fTotalRep;
}

double __declspec(naked) UnchangedCalculateVisualRep(/*int a1@<edi>*/)
{
    __asm
    {
        // Execute the code replaced by our hook.
        sub esp, 8
        fld dword ptr ds:[0x6CC7A4]
        // Jump back to the original code, past the hook.
        mov eax, 0x5A05B9
        jmp eax
    }
}

void UnlockMagazine15(int iUnknownId)
{
    // Based off sub_51B060
    auto byte_760F9D = reinterpret_cast<char*>(0x760F9D);
    byte_760F9D[1108 * iUnknownId] = 1;
    DebugPrint("Unlocking Magazine 15\n");
}

void UnlockMagazine16(int iUnknownId)
{
    // Based off sub_51B060
    auto byte_7613F1 = reinterpret_cast<char*>(0x7613F1);
    byte_7613F1[1108 * iUnknownId] = 1;
    DebugPrint("Unlocking Magazine 16\n");
}

void UnlockMagazine17(int iUnknownId)
{
    // Based off sub_51B060
    auto byte_761845 = reinterpret_cast<char*>(0x761845);
    byte_761845[1108 * iUnknownId] = 1;
    DebugPrint("Unlocking Magazine 17\n");
}

void UnlockMagazine18(int iUnknownId)
{
    // Based off sub_51B060
    auto byte_761C99 = reinterpret_cast<char*>(0x761C99);
    byte_761C99[1108 * iUnknownId] = 1;
    DebugPrint("Unlocking Magazine 18\n");
}

void UnlockMagazine19(int iUnknownId)
{
    // Based off sub_51B060
    auto byte_7620ED = reinterpret_cast<char*>(0x7620ED);
    byte_7620ED[1108 * iUnknownId] = 1;
    DebugPrint("Unlocking Magazine 19\n");
}
}

bool PatchNFSU()
{
    if(injector::ReadMemory<uint32_t>(0x5A05B0) != 0xD908EC83)
        return false;

    // Define the star reputation through a lookup table.
    injector::MakeJMP(0x5A05B0, injector::raw_ptr(CalculateVisualRep));
    /*
        .text:005A05B0 ; double __usercall ??CalculateVisualRep@<st0>(int a1@<edi>)
    */
    
    // Keep magazine 15's check for the star reputation of the car.
    // This way, the player may either get it by building the one star 
    // car that the game requires, or by the method implemented in here,
    // by getting past a certain UG Event.
    injector::MakeRelativeOffset(0x51B38F+1, injector::raw_ptr(UnchangedCalculateVisualRep));
    /*
        .text:0051B38F    call    ??CalculateVisualRep
        .text:0051B394    fcomp   ds:flt_6CC7BC  ; 1.0
    */
    
    return true;
}
