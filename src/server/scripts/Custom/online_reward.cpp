#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "DBCStores.h"
#include "Mail.h"
#include "Item.h"
#include "DatabaseEnv.h"

class played_time_rewards_WorldScript : public WorldScript
{
public:
    played_time_rewards_WorldScript() : WorldScript("played_time_rewards") {}

    void OnUpdate(uint32 diff) override
    {
        _events.Update(diff);

        if (_events.ExecuteEvent())
        {
            auto& players = ObjectAccessor::GetPlayers();
            const std::set<uint32> rewardDays = { 2, 4, 6, 8, 10, 12, 14, 16, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 198, 200 };

            for (auto itr = players.begin(); itr != players.end(); itr++)
            {
                uint32 played = itr->second->GetTotalPlayedTime() / HOUR;

                if (rewardDays.find(played) != rewardDays.end())
                {
                    SendMail(itr->second, 90705);
                }

                if (played >= 740 && played <= 740 + 1) // Example of a specific day reward.
                {
                    if (CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(141))
                        itr->second->SetTitle(titleInfo);

                    SendMail(itr->second, 90705);
                }
            }

            _events.ScheduleEvent(1, updateInterval * IN_MILLISECONDS);
        }
    }

    void OnStartup() override
    {
        _events.ScheduleEvent(1, updateInterval * IN_MILLISECONDS);
    }

    void SendMail(Player* player, uint32 itemId)
    {
        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
        MailDraft draft("Награда за время в игре!", "Вы получили эту награду, т.к провели в игре достаточно много времени, продолжайте, чтобы получить еще больше наград!");
        auto trans = CharacterDatabase.BeginTransaction();

        if (Item* item = Item::CreateItem(itemId, 5, 0))
        {
            item->SaveToDB(trans);              // Save to prevent being lost at next mail load. If send fails, the item will be deleted.
            draft.AddItem(item);
        }

        draft.SendMailTo(trans, MailReceiver(player), sender);
        CharacterDatabase.CommitTransaction(trans);
    }

private:
    const uint32 updateInterval = 10 * MINUTE;
    EventMap _events;
};

void AddSC_PlayedTimeRewards()
{
   // new played_time_rewards_WorldScript();
}
