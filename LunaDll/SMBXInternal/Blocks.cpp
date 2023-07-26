#include "Blocks.h"
#include "PlayerMOB.h"
#include <cstdint>
#include <utility>
#include <algorithm>

bool isBlocksSortingRequired = false;

Block* Blocks::Get(int index) {
    if(GM_BLOCKS_PTR == 0 || index < 0 || index > GM_BLOCK_COUNT) {
        return NULL;
    } else {
        return &((GetBase())[index]);
    }
}

static ExtendedBlockFields g_extendedBlockFields[20001];

ExtendedBlockFields* Blocks::GetRawExtended(int index)
{
    if (index < 0 || index >= 20001)
        return nullptr;

    return &g_extendedBlockFields[index];
}

void Blocks::ClearExtendedFields()
{
    for (int i = 0; i < 20001; i++)
    {
        g_extendedBlockFields[i].Reset();
    }
}

bool Blocks::IsPlayerTouchingType(int type, int sought, PlayerMOB* demo) {	
    Block* blocks = Blocks::GetBase();
    Block* block = 0;
    double playerX = demo->momentum.x - 0.20;
    double playerY = demo->momentum.y - 0.20;
    double playerX2 = demo->momentum.x + demo->momentum.width + 0.20;
    double playerY2 = demo->momentum.y + demo->momentum.height + 0.20;

    for(int i = 1; i <= GM_BLOCK_COUNT; i++) {
        if(blocks[i].BlockType == type) {
            block = &blocks[i];

            if(playerX > block->momentum.x + block->momentum.width ||
                playerX2 < block->momentum.x  ||
                playerY > block->momentum.y + block->momentum.height ||
                playerY2 < block->momentum.y)
                continue;

            if(TestCollision(demo, block) == sought)
                return true;
        }
    }
    return false; // no collision
}

void Blocks::DoSortingIfRequired()
{
    if (!isBlocksSortingRequired)
        return;

    WORD beginIndex = 1;
    WORD blockCount = GM_BLOCK_COUNT;
    native_sortX((short*)&beginIndex, (short*)&blockCount);

    for (WORD i = 2; i <= blockCount; i++) {
        if (Blocks::Get(i)->momentum.x > Blocks::Get(i - 1)->momentum.x) {
            WORD prev = i - 1;
            native_sortY((short*)&i, (short*)&prev);
        }
    }

    native_sort_finalize1();
    native_sort_finalize2();

    isBlocksSortingRequired = false;
}

void Blocks::SetNextFrameSorting()
{
    isBlocksSortingRequired = true;
}

// TEST COLLISION (SMBX BUILTIN)
int Blocks::TestCollision(PlayerMOB* pMobPOS, Block* pBlockPOS) {	
    typedef int __stdcall colfunc(void*, void*);
    colfunc* f = (colfunc*)GF_MOB_BLOCK_COL;	
    return f(&pMobPOS->momentum.x, &pBlockPOS->momentum.x);
}

// SET ALL
void Blocks::SetAll(int type1, int type2) {
    Block* blocks = Blocks::GetBase();	
    for(int i = 1; i <= GM_BLOCK_COUNT; i++) {
        if(blocks[i].BlockType == type1) {
            blocks[i].BlockType = type2;
        }
    }
}

// SWAP ALL
void Blocks::SwapAll(int type1, int type2) {
    Block* blocks = Blocks::GetBase();	
    for(int i = 1; i <= GM_BLOCK_COUNT; i++) {
        if(blocks[i].BlockType == type1) {
            blocks[i].BlockType = type2;
        }
        else if(blocks[i].BlockType == type2) {
            blocks[i].BlockType = type1;
        }
    }
}

void Blocks::ShowAll(int type) {
    Block* blocks = Blocks::GetBase();	
    for(int i = 1; i <= GM_BLOCK_COUNT; i++) {
        if(blocks[i].BlockType == type) {
            blocks[i].IsInvisible3 = 0;
        }		
    }
}

void Blocks::HideAll(int type) {
    Block* blocks = Blocks::GetBase();	
    for(int i = 1; i <= GM_BLOCK_COUNT; i++) {
        if(blocks[i].BlockType == type) {
            blocks[i].IsInvisible3 = 0xFFFF;
        }		
    }
}

static int16_t blockprop_bumpable[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_playerfilter[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_npcfilter[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_hitid[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_breakable[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_breakeffectid[Block::MAX_ID + 1] = { 0 };
static int16_t blockprop_breaksoundid[Block::MAX_ID + 1] = { 0 };

void Blocks::InitProperties() {
    for (int id = 1; id <= Block::MAX_ID; id++)
    {
        SetBlockBumpable(id, false);
        SetBlockPlayerFilter(id, 0);
        SetBlockNPCFilter(id, 0);
        SetBlockHitId(id, 2); // modBlocks.bas line 171
        SetBlockBreakable(id, false); // modBlocks.bas line 1286
        SetBlockBreakEffectId(id, 1); // modBlocks.bas line 901
        SetBlockBreakSoundId(id, 4); // modBlocks.bas line 893
    }

    // Default game config
    SetBlockBumpable(0x4, true);
    SetBlockBumpable(0x267, true);
    SetBlockBumpable(0x37, true);
    SetBlockBumpable(0x3C, true);
    SetBlockBumpable(0x5A, true);
    SetBlockBumpable(0x9F, true);
    SetBlockBumpable(0xA9, true);
    SetBlockBumpable(0xAA, true);
    SetBlockBumpable(0xAD, true);
    SetBlockBumpable(0xB0, true);
    SetBlockBumpable(0xB3, true);
    SetBlockBumpable(0xBC, true);
    SetBlockBumpable(0xE2, true);
    SetBlockBumpable(0x119, true);
    SetBlockBumpable(0x11A, true);
    SetBlockBumpable(0x11B, true);
    for (int id = 0x26E; id <= 0x271; id++)
    {
        SetBlockBumpable(id, true);
    }
    SetBlockBumpable(0x277, true);

    SetBlockPlayerFilter(626, 1);
    SetBlockPlayerFilter(627, 2);
    SetBlockPlayerFilter(628, 3);
    SetBlockPlayerFilter(629, 4);
    SetBlockPlayerFilter(632, 5);

    SetBlockHitId(55, 55);
    SetBlockHitId(60, 192);
    SetBlockHitId(88, 89);
    SetBlockHitId(89, 89);
    SetBlockHitId(90, 89);
    SetBlockHitId(159, 159);
    SetBlockHitId(171, 89);
    SetBlockHitId(174, 89);
    SetBlockHitId(177, 89);
    SetBlockHitId(180, 89);
    SetBlockHitId(188, 192);
    SetBlockHitId(192, 192);
    SetBlockHitId(193, 192);
    SetBlockHitId(224, 225);
    SetBlockHitId(225, 225);
    SetBlockHitId(226, 225);
    SetBlockHitId(369, 192);

    SetBlockBreakable(4, true);
    SetBlockBreakable(60, true);
    SetBlockBreakable(90, true);
    SetBlockBreakable(188, true);
    SetBlockBreakable(226, true);
    SetBlockBreakable(293, true);
    SetBlockBreakable(526, true);

    SetBlockBreakEffectId(60, 21);
    SetBlockBreakEffectId(188, 51);
    SetBlockBreakEffectId(293, 135);
    SetBlockBreakEffectId(457, 100);
    SetBlockBreakEffectId(526, 107);

    SetBlockBreakSoundId(186, 43);
    SetBlockBreakSoundId(526, 64);
}

bool Blocks::GetBlockBumpable(int id) {
    if ((id < 1) || (id > Block::MAX_ID)) return false;
    return (blockprop_bumpable[id] != 0);
}

void Blocks::SetBlockBumpable(int id, bool bumpable) {
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_bumpable[id] = bumpable ? -1 : 0;
}

short Blocks::GetBlockPlayerFilter(int id)
{
    if ((id < 1) || (id > Block::MAX_ID)) return 0;
    return blockprop_playerfilter[id];
}

void Blocks::SetBlockPlayerFilter(int id, short characterId)
{
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_playerfilter[id] = characterId;
}

short Blocks::GetBlockNPCFilter(int id)
{
    if ((id < 1) || (id > Block::MAX_ID)) return 0;
    return blockprop_npcfilter[id];
}

void Blocks::SetBlockNPCFilter(int id, short npcId)
{
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_npcfilter[id] = npcId;
}

short Blocks::GetBlockHitId(int id)
{
    if ((id < 1) || (id > Block::MAX_ID)) return 0;
    return blockprop_hitid[id];
}

void Blocks::SetBlockHitId(int id, short hitId)
{
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_hitid[id] = hitId;
}

bool Blocks::GetBlockBreakable(int id)
{
    if ((id < 1) || (id > Block::MAX_ID)) return false;
    return (blockprop_breakable[id] != 0);
}

void Blocks::SetBlockBreakable(int id, bool breakable)
{
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_breakable[id] = breakable ? -1 : 0;
}

short Blocks::GetBlockBreakEffectId(int id) {
    if ((id < 1) || (id > Block::MAX_ID)) return 0;
    return blockprop_breakeffectid[id];
}

void Blocks::SetBlockBreakEffectId(int id, short breakEffectId) {
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_breakeffectid[id] = breakEffectId;
}

short GetBlockBreakSoundId(int id) {
    if ((id < 1) || (id > Block::MAX_ID)) return 0;
    return blockprop_breaksoundid[id];
}

void SetBlockBreakSoundId(int id, short breakSoundId) {
    if ((id < 1) || (id > Block::MAX_ID)) return;
    blockprop_breaksoundid[id] = breakSoundId;
}

// This array should always be lexicographically sorted by property name.
static std::pair<const char*, uintptr_t> const propertyTables[] {
    std::make_pair("breakable", reinterpret_cast<uintptr_t>(blockprop_breakable)),
    std::make_pair("breakeffectid", reinterpret_cast<uintptr_t>(blockprop_breakeffectid)),
    std::make_pair("breaksoundid", reinterpret_cast<uintptr_t>(blockprop_breaksoundid)),
    std::make_pair("bumpable", reinterpret_cast<uintptr_t>(blockprop_bumpable)),
    std::make_pair("hitid", reinterpret_cast<uintptr_t>(blockprop_hitid)),
    std::make_pair("npcfilter", reinterpret_cast<uintptr_t>(blockprop_npcfilter)),
    std::make_pair("playerfilter", reinterpret_cast<uintptr_t>(blockprop_playerfilter))
};

// Getter for address of Block property arrays
uintptr_t Blocks::GetPropertyTableAddress(const std::string& s)
{
    std::pair<const char*, uintptr_t> const* foundEntryPointer = std::lower_bound(
        std::begin(propertyTables),
        std::end(propertyTables),
        s,
        [] (std::pair<const char*, uintptr_t> const& entry, std::string const& property) -> bool {
            return std::strcmp(entry.first, property.c_str()) < 0;
        }
    );

    if (foundEntryPointer == std::cend(propertyTables)) {
        return 0;
    } else {
        return foundEntryPointer->second;
    }
}
