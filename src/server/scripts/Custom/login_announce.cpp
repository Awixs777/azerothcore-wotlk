#include "ScriptPCH.h"
#include "WorldSessionMgr.h"

class announce_login : public PlayerScript
{
public:
    announce_login() : PlayerScript("announce_login") {}

    void OnPlayerLogin(Player* player) override
    {
        if (!player->IsGameMaster())
        {
            // Определение фракции и соответствующего сообщения
            std::string factionName;
            std::string factionColor;

            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                factionName = "Альянс";
                factionColor = "|CFF0042FF";
            }
            else
            {
                factionName = "Орда";
                factionColor = "|CFFFF0303";
            }

            // Формируем сообщение
            std::ostringstream ss;
            ss << "|CFFFE8A0EПриветствуем нового игрока|r: |CFFE55BB0" << player->GetName() << "|r|CFFFE8A0E Фракция|r: " << factionColor << factionName << "|r";
            sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());

            // Установка максимальных значений энергии и здоровья
            player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            player->SetPower(POWER_ENERGY, player->GetMaxPower(POWER_ENERGY));
            player->SetPower(POWER_RAGE, player->GetMaxPower(POWER_RAGE));
            player->SetPower(POWER_RUNIC_POWER, player->GetMaxPower(POWER_RUNIC_POWER));
            player->SetFullHealth(); // Устанавливаем полное здоровье
        }
    }
};

void AddSC_announce_login()
{
    new announce_login;
}
