#include "ScriptPCH.h"

class BuffInst : public PlayerScript
{

public:
    BuffInst() : PlayerScript("BuffInst") {}

    /* бафф инст*/
    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
    {
        // проверка входа
        if (!player->GetGroup() && (player->GetMapId() == 550 || player->GetMapId() == 552
            || player->GetMapId() == 553 || player->GetMapId() == 554 || player->GetMapId() == 555 || player->GetMapId() == 632
            || player->GetMapId() == 658 || player->GetMapId() == 668 || player->GetMapId() == 602 || player->GetMapId() == 565))
        {
            player->CastSpell(player, 90005);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Buff-Solo]:|rВы получили бафф увеличения характеристик.");
        }

        if (!player->GetGroup() && (player->GetMapId() == 544 || player->GetMapId() == 558 || player->GetMapId() == 601
            || player->GetMapId() == 533 || player->GetMapId() == 585 || player->GetMapId() == 615))
        {
            player->CastSpell(player, 90005);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Buff-Solo]:|rВы получили бафф увеличения характеристик.");
        }

        // проверка выхода, сброс бафа
        if (player->GetMapId() == 1 || player->GetMapId() == 0 || player->GetMapId() == 571 || player->GetMapId() == 530 || player->GetGroup())
        {
            player->RemoveAurasDueToSpell(90005);
            player->RemoveAurasDueToSpell(90003);
        }
    }
};

void AddSC_BuffInst()
{
    new BuffInst();
}
