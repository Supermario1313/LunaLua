#include "PlayerMOB.h"
#include "../Defines.h"
#include "../Misc/MiscFuncs.h"
#include "NPCs.h"

// ACCESSORS
PlayerMOB* Player::Get(int index) {
    return PlayerMOB::Get(index);
}

int Player::GetIdx(PlayerMOB *player) {
    int idx = ((PlayerMOB*)GM_PLAYERS_PTR - player) / sizeof(PlayerMOB);

    if (0 <= idx && idx <= 200) return idx;

    idx = ((PlayerMOB*)GM_PLAYERS_TEMPLATE - player) / sizeof(PlayerMOB);

    if (0 <= idx && idx <= 10) return idx + 1000;

    return -1;
}


// MANAGEMENT
bool Player::InternalSwap(int player1, int player2) {
    char temp[500];
    ExtendedPlayerFields tempFields;

    PlayerMOB* p1 = Player::Get(1);
    PlayerMOB* p2 = Player::Get(2);

    if(p1 == 0 || p2 == 0)
        return false;

    memcpy(temp, p1, 0x184);
    memcpy(p1, p2, 0x184);
    memcpy(p2, temp, 0x184);

    tempFields = *Player::GetExtended(1);
    *Player::GetExtended(1) = *Player::GetExtended(2);
    *Player::GetExtended(2) = tempFields;

    return true;
}

bool Player::InternalSwap(PlayerMOB* player1, PlayerMOB* player2) {
    char temp[500];

    if(player1 == 0 || player2 == 0)
        return false;

    memcpy(temp, player1, 0x184);
    memcpy(player1, player2, 0x184);
    memcpy(player2, temp, 0x184);

    int p1Idx = Player::GetIdx(player1);
    int p2Idx = Player::GetIdx(player2);

    if (p1Idx >= 0 && p2Idx >= 0) {
        ExtendedPlayerFields tempFields;

        tempFields = *Player::GetExtended(p1Idx);
        *Player::GetExtended(p1Idx) = *Player::GetExtended(p2Idx);
        *Player::GetExtended(p2Idx) = tempFields;
    }

    return true;
}

void Player::MemSet(int offset, double value, OPTYPE operation, FIELDTYPE ftype) {
    char* dbg =  "PLAYER MEM SET";
    if(ftype == FT_INVALID || offset > (0x184 * 99) || offset < 0)
        return;
    PlayerMOB* pPlayer = Player::Get(1);
    void* ptr = ((&(*(byte*)pPlayer)) + offset);
    MemAssign((int)ptr, value, operation, ftype);
}


// BUTTON FUNCS
bool Player::PressingDown(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.downKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingJump(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.jumpKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingUp(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.upKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingRight(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.rightKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingLeft(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.leftKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingRun(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.altRunKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingSEL(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.dropItemKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingAltJump(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.altJumpKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingAltRun(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->keymap.altRunKeyState == 0xFFFFFFFF ? true : false;
}

bool Player::PressingPause(PlayerMOB* pPlayer)  {
    return (unsigned)pPlayer->keymap.pauseKeyState == 0xFFFFFFFF ? true : false;
}

void Player::ConsumeAllKeys(PlayerMOB* pPlayer) {
    //pPlayer->
}


// PLAYER CYCLE FUNCS
void Player::CycleRight(PlayerMOB* pPlayer) {

    // Check mount stuff for safe cycle
    //if(pPlayer->MountType != 0) {
    //    if(pPlayer->Identity > 1)
    //        return false; // fail to cycle because next character can't be mounted
    //}

    int playerID = static_cast<int>(pPlayer->Identity);

    playerID++;
    if(playerID > 5)
        playerID = 1;

    pPlayer->Identity = static_cast<Characters>(playerID);

}

void Player::CycleLeft(PlayerMOB* pPlayer) {
    int playerID = static_cast<int>(pPlayer->Identity);

    playerID--;
    if (playerID <= 0)
        playerID = 5;

    pPlayer->Identity = static_cast<Characters>(playerID);
}


// STATE FUNCS
bool Player::UsesHearts(PlayerMOB* pPlayer) {
    return (pPlayer->Identity == 1 || pPlayer->Identity == 2 ? false : true);
}

bool Player::IsSpinjumping(PlayerMOB* pPlayer) {
    return (unsigned)pPlayer->IsSpinjumping == 0xFFFFFFFF ? true : false;
}

bool Player::IsInForcedAnimation(PlayerMOB* player) {
    short state = player->ForcedAnimationState;
    if(state == 1 || state == 2 || state == 3 || state == 4 || state == 7)
        return true;
    return false;
}

bool Player::IsHoldingSpriteType(PlayerMOB* player, int NPC_ID) {
    if(player->HeldNPCIndex != 0) {
        NPCMOB* npc = NPC::Get(player->HeldNPCIndex);
        if(npc->id == NPC_ID)
            return true;
    }
    return false;
}

int	Player::IsStandingOnNPC(PlayerMOB* player) {
    return player->NPCBeingStoodOnIndex;
}

// FILTER FUNCS
void Player::FilterToFire(PlayerMOB* player) {
    if(player->CurrentPowerup > 3)
        player->CurrentPowerup = 3;
}

void Player::FilterToBig(PlayerMOB* player) {
    if(player->CurrentPowerup > 2)
        player->CurrentPowerup = 2;
}

void Player::FilterToSmall(PlayerMOB* player) {
    if(player->CurrentPowerup > 1)
        player->CurrentPowerup = 1;
}

void Player::FilterReservePowerup(PlayerMOB* player) {
    player->PowerupBoxContents = 0;
}

void Player::FilterMount(PlayerMOB* player) {
    player->MountType = 0;
    player->MountState = 0;
}

// HEARTS
void Player::SetHearts(PlayerMOB* player, int new_hearts) {
    if(new_hearts <= 1) {
        player->Hearts = new_hearts;
        player->CurrentPowerup = 1;
    }
    else {
        player->Hearts = new_hearts;
        player->CurrentPowerup = (player->CurrentPowerup >= 1 ? player->CurrentPowerup : 2);
    }
}


// ACTIONS
void Player::Kill(short pIndex) {
    native_killPlayer(&pIndex);
}

void Player::Harm(short pIndex) {
    native_harmPlayer(&pIndex);
}


// GET SCREEN POSITION
RECT Player::GetScreenPosition(PlayerMOB* player) {
    double* pCameraY = (double*)GM_CAMERA_Y;
    double* pCameraX = (double*)GM_CAMERA_X;
    double cam_y = -pCameraY[1];
    double cam_x = -pCameraX[1];
    double cam_d = cam_y + 600;
    double cam_r = cam_x + 800;

    RECT ret_rect;
    ret_rect.left = (LONG)(player->momentum.x - cam_x);
    ret_rect.top = (LONG)(player->momentum.y - cam_y);
    ret_rect.right = ret_rect.left + (LONG)player->momentum.width;
    ret_rect.bottom = ret_rect.top + (LONG)player->momentum.height;
    return ret_rect;
}

// TODO Should probably replace magic numbers by a macro
static PlayerPhysics extCPhysics[17];
LegacyPlayerPhysics &Player::ogPhysics = *((LegacyPlayerPhysics*) GM_JUMPHIGHT_CONSTPTR);

// Characters enum doesn't account for new X2 characters
PlayerPhysics* Player::GetPhysicsForChar(int character) {
    if (character < 0 || character > 16) return nullptr;

    return &extCPhysics[character];
}

// 0 - 200 for players, 201 - 211 for player templates
static ExtendedPlayerFields extPFields[212];

ExtendedPlayerFields* Player::GetExtended(int index) {
    if (index >= 1000 && index <= 1010) {
        return &extPFields[index - 799];
    }
    else if (index >= 0 && index <= GM_PLAYERS_COUNT) {
        return &extPFields[index];
    }
    else {
        return nullptr;
    }
}

void Player::ClearExtendedFields() {
    for (int i = 0; i < 212; i++) {
        extPFields[i].overridenFields = 0;
    }
}
