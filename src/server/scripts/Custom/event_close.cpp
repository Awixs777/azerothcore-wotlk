#include "ScriptPCH.h"

class CloseZone : public PlayerScript
{

public:
	CloseZone() : PlayerScript("CloseZone") {}

	/* запрет входа на событие */
	void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
	{
		if ((newArea == 4179 || newArea == 4987))
		{
			if (sGameEventMgr->IsActiveEvent(120) && player->GetLevel() >= 200 || player->IsGameMaster())
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Событие]:|r Добро пожаловать на Событие [Изумрудка]|r");
			else
			{
				if (player->GetTeamId() == TEAM_HORDE)
					player->TeleportTo(571, 6236.229980f, 5768.240234f, -5.373631f, 0.736042f);
				else
					player->TeleportTo(571, 6236.229980f, 5768.240234f, -5.373631f, 0.736042f);
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Доступ Запрещен]:|r Событие не активно или ваш уровень менее 200|r");
			}
		}

        // AOE LOOT
        if (!player->GetGroup() && (player->GetMapId() == 624 || player->GetAreaId() == 35 || player->GetZoneId() == 2817 ||
            player->GetAreaId() == 279 || player->GetAreaId() == 3749))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[AutoLoot]:|r\nВ данной локации работает Соло [AutoLoot]|r");
        }

        // Кач
        if (player->GetZoneId() == 268)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Путеводитель]:|rНе забудь одеть экипировку 80-го и 130-го уровня!|r");
        }

        // Лолита Лейн
        if (player->GetAreaId() == 3547 && player->GetLevel() < 229)
        {
            player->TeleportTo(571, 6236.229980f, 5768.240234f, -5.373631f, 0.736042f);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Доступ Запрещен]:|rНеобходим 230-й уровень!|r");
        }

	}
};

void AddSC_CloseZone()
{
	new CloseZone();
}
