
#include "ScriptPCH.h"
#include "Chat.h"
#include <cstring>
#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "WorldSession.h"

using namespace Acore::ChatCommands;

class boss_commandscript : public CommandScript
{
public:
	boss_commandscript() : CommandScript("boss_commandscript") { }

    ChatCommandTable GetCommands() const override
	{
        static ChatCommandTable BossCommandTable =
		{
            { "all", HandleAllCommand, SEC_PLAYER, Console::No},
		};
        static ChatCommandTable commandTable =
		{
            { "mir", BossCommandTable },
		};
		return commandTable;
	}

    static bool HandleAllCommand(ChatHandler* handler, char const* /*args*/)
    {
        if (!handler)
            return false;

        time_t current_time = time(0);
        int time_stamp = int(current_time);

        Player* pPlayer = handler->GetSession()->GetPlayer();

        // Список боссов
        std::vector<std::pair<uint64, std::string>> bosses = {
            {4360017, "[Лорд]"},
            {4660737, "[Иллидан]"},
            {3932621, "[Эфириал]"},
            {2376962, "[Инквизитор]"},
            {4352959, "[Изера]"},
            {4763736, "[Кил'Джеден]"},
            {4763931, "[Повелитель]"},
            {10495, "[Вариан]"},
            {4770, "[Тралл]"},
            {4764200, "[Лолита Лейн]"}
        };

        std::ostringstream announce_message;
        announce_message << "|CFFFE8A0E[Таймер Мировых Боссов]\n";

        for (const auto& boss : bosses)
        {
            uint64 guid = boss.first;
            std::string boss_name = boss.second;
            std::string respawn_info;

            QueryResult queryResult = CharacterDatabase.Query("SELECT respawnTime FROM creature_respawn WHERE guid = " + std::to_string(guid));
            if (queryResult)
            {
                uint32 respawnUnix = queryResult->Fetch()[0].Get<uint32>();
                int respawn_time = respawnUnix - time_stamp;
                if (respawn_time <= 0)
                    respawn_info = boss_name + " жив. Вперед, убейте его!";
                else
                    respawn_info = boss_name + " мертв, реснется через |cff02A4B1" + secsToTimeString(respawn_time) + "\n";
            }
            else
            {
                respawn_info = boss_name + " жив. Вперед, убейте его!";
            }

            announce_message << "|CFFFE8A0EВозрождение " << respawn_info << "\n";
        }

        // Отправляем сформированное сообщение с таймерами
        ChatHandler(pPlayer->GetSession()).PSendSysMessage(announce_message.str().c_str());
        return true;
    }

};
void AddSC_boss_commandscript()
{
	new boss_commandscript();
}
