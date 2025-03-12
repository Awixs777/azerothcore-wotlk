#include "ScriptPCH.h"

class CloseZone : public PlayerScript
{

public:
	CloseZone() : PlayerScript("CloseZone") {}

	/* запрет входа на событие */
	void OnPlayerUpdateZone(Player* player, uint32 newZone, uint32 newArea)
	{
		if ((newArea == 4179 || newArea == 4987))
		{
            if ((sGameEventMgr->IsActiveEvent(120) && player->GetLevel() >= 200) || player->IsGameMaster())
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
            player->GetAreaId() == 279 || player->GetAreaId() == 3749 || player->GetMapId() == 557 || player->GetMapId() == 556
            || player->GetMapId() == 544 || player->GetMapId() == 558))
        {
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[AutoLoot]:|r В данной локации работает SOLO автолут.|r");
        }
        
        // Кач
        if (player->GetAreaId() == 3792)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Путеводитель]:|rВы не получаете опыта за убийство - это нормально.\nПосле выполнения квеста, Вы, получите предмет для повышения уровня.!|r");
        }

        // Лолита Лейн
        if (player->GetAreaId() == 3547 && player->GetLevel() < 230 && !player->IsGameMaster())
        {
            player->TeleportTo(571, 6236.229980f, 5768.240234f, -5.373631f, 0.736042f);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Доступ Запрещен]:|r Необходим 230-й уровень!|r");
        }

        // Повелитель
        if (player->GetAreaId() == 3615 && player->GetLevel() < 230 && !player->IsGameMaster())
        {
            player->TeleportTo(571, 6236.229980f, 5768.240234f, -5.373631f, 0.736042f);
            ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Доступ Запрещен]:|r Необходим 230-й уровень!|r");
        }

        // Помощь Стратхольму
        if (player->GetAreaId() == 4100)
        {
            // Список квестов
            std::array<uint32_t, 7> questIds = { 20551, 20057, 20058, 20059, 20060, 20061, 20062 };

            // Флаг, чтобы отправить сообщение только один раз
            bool questsAdded = false;

            // Проходим по всем квестам
            for (uint32_t questId : questIds)
            {
                // Получаем квест из базы данных
                Quest const* quest = sObjectMgr->GetQuestTemplate(questId);

                // Проверка наличия квеста и его статуса
                if (quest)
                {
                    bool questAlreadyAdded = player->HasQuest(questId);

                    // Квест можно добавить, если он еще не был выдан или активен
                    if (!questAlreadyAdded && (!player->GetQuestRewardStatus(questId) || player->IsActiveQuest(questId)))
                    {
                        player->AddQuest(quest, player);
                        questsAdded = true;  // Устанавливаем флаг, что хотя бы один квест был выдан
                    }
                }
            }

            // Сообщение об успешной выдаче квестов
            if (questsAdded)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Доступ ТМ]:|r Выданы квесты необходимые для получения доступа к ТМ|r");
            }
        }
	}
};

void AddSC_CloseZone()
{
	new CloseZone();
}
