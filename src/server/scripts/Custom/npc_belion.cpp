/*
* Patch: NPC Bonus Buff - Boss respawn - Exchange
* By: r0m1ntik
* Date: 25.04.2019
*/

#include <cstring>
#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "WorldSession.h"
#include "WorldSessionMgr.h"


using namespace std;

struct BuffData
{
	uint32 Entry;
	uint8  Cost;
	string Name;
	string SubName;
};

BuffData vvData[] =
{
	{ 46587, 15, "Берсерк", "Повышает скорость атаки на 30% и урона на 20% на 1 час" },
	{ 26035, 10, "Счастливый", "+10% ко всем характеристикам на 30 мин" }
};

/* выводим количество VP на акке у игрока */
uint32 GetBonus(Player* player)
{
    uint32 accId = player->GetSession()->GetAccountId();
    QueryResult result = CharacterDatabase.Query("SELECT vp FROM fusion.account_data WHERE id = {}", accId);

    if (!result)  // Проверка на успешность запроса
    {
        player->GetSession()->SendAreaTriggerMessage("Ошибка! Сообщите Администратору!");
        return 0;  // Возвращаем 0 в случае ошибки
    }
    Field* field = result->Fetch();  // Получаем данные из результата запроса
    return field[0].Get<uint32>();   // Возвращаем значение VP
}


/* выводим количество DP на акке у игрока */
uint32 GetBonusDP(Player* player)
{
    uint32 accId = player->GetSession()->GetAccountId();
    QueryResult result = CharacterDatabase.Query("SELECT dp FROM fusion.account_data WHERE id = {}", accId);

    if (!result)  // Проверка на успешность запроса
    {
        player->GetSession()->SendAreaTriggerMessage("Ошибка! Сообщите Администратору!");
        return 0;  // Возвращаем 0 в случае ошибки
    }
    Field* field = result->Fetch();  // Получаем данные из результата запроса
    return field[0].Get<uint32>();   // Возвращаем значение DP
}

void DelBonus(Player* player, uint32 bonus)
{
	uint32 accId = player->GetSession()->GetAccountId();
	CharacterDatabase.Query("UPDATE fusion.account_data SET vp = vp - {} WHERE id = {}", bonus, accId);
}

void DelBonusDP(Player* player, uint32 bonus)
{
	uint32 accId = player->GetSession()->GetAccountId();
	CharacterDatabase.Query("UPDATE fusion.account_data SET dp = dp - {} WHERE id = {}", bonus, accId);
}

void AddBonusVP(Player* player, uint32 bonus)
{
	uint32 accId = player->GetSession()->GetAccountId();
	CharacterDatabase.Query("UPDATE fusion.account_data SET vp = vp + {} WHERE id = {}", bonus, accId);
}

void AddBonusDP(Player* player, uint32 bonus)
{
    uint32 accId = player->GetSession()->GetAccountId();
    CharacterDatabase.Query("UPDATE fusion.account_data SET dp = dp + {} WHERE id = {}", bonus, accId);
}

/* выводим время респавна моба */
uint64 GetRespawnTime(Player* player, uint64 guid)
{
	QueryResult queryResult = CharacterDatabase.Query("SELECT respawnTime FROM creature_respawn WHERE guid = {}", guid);
	if (!queryResult)
		return 0;
	else
	{
		Field *field = queryResult->Fetch();
		return field[0].Get<uint64>();
	}
}

/* выводим онлайн игроков и выдает бафф */
void GetBuffOnline(uint32 i)
{
    WorldSessionMgr::SessionMap const& sessions = sWorldSessionMgr->GetAllSessions();
    for (WorldSessionMgr::SessionMap::const_iterator it = sessions.begin(); it != sessions.end(); ++it)
    {
		if (Player* player = it->second->GetPlayer())
		{
			if (player->IsInWorld() && !player->GetMap()->IsBattlegroundOrArena())
				player->AddAura(vvData[i].Entry, player);
		}
	}
}

std::string GetNameSpell(uint8 i)
{
	std::string str = vvData[i].Name + " - стоймость|cff065961 " + std::to_string(vvData[i].Cost) + "|r бонусов\n|cff065961" + vvData[i].SubName;
	return str;
}

class npc_bonus_buff : public CreatureScript
{
public: npc_bonus_buff() : CreatureScript("npc_bonus_buff") { }

		bool OnGossipHello(Player* player, Creature* creature)
		{
			std::string name = player->GetName();
			std::ostringstream info;

			ClearGossipMenuFor(player);
            info << "Приветствую, " << name << "\n\nНа вашем счету:\n|cff065961" << GetBonus(player) << "|r очков голосования.\n|cff065961" << GetBonusDP(player) << "|r очков пожертвования.\n";

			AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_bg_killingblow_most:25:25:-20:0|tТаймер Мировых Боссов", GOSSIP_SENDER_MAIN, 1);
			AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_valentinesboxofchocolates02:25:25:-20:0|tБаффнуть всех -|cff065961 ОНЛАЙН|r", GOSSIP_SENDER_MAIN, 3);
		//	AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_pvp_o_05:25:25:-20:0|tОбменять Доступ-Карты|r", GOSSIP_SENDER_MAIN, 16);
			AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:25:25:-20:0|tИзменить персонажа", GOSSIP_SENDER_MAIN, 17);
			AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_01:25:25:-20:0|tПеревод ЛК -> [Vote-Token]", GOSSIP_SENDER_MAIN, 2);
			AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_02:25:25:-20:0|tПеревод ЛК -> [Монета Donate]", GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_02:25:25:-20:0|t[Монета Donate] -> [Vote-Token]", GOSSIP_SENDER_MAIN, 154);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_17:25:25:-20:0|t[Монета Donate] -> Баланс ЛК", GOSSIP_SENDER_MAIN, 200, "Вы уверены, что хотите перевести все монеты в ЛК?", 0, false);
            player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
            return true;
		}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
		{
			ClearGossipMenuFor(player);
			if (sender == GOSSIP_SENDER_MAIN)
			{
                // Можно объявить глобально или статически: в реальном коде вынесите в отдельный .cpp/.h
                static std::unordered_map<uint64, uint64> s_PlayerCooldown;

                // Проверка кулдауна (2 сек = 2000 мс)
                uint64 currentTime = getMSTime(); // TrinityCore: время в мс с момента старта сервера
                uint64& nextUseTime = s_PlayerCooldown[player->GetGUID().GetRawValue()];
				switch (action)
				{
                case 200:
                {
                    if (currentTime < nextUseTime)
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000Подождите пару секунд прежде, чем снова использовать эту опцию!|r");
                        CloseGossipMenuFor(player);
                        break;
                    }

                    // Устанавливаем новый кулдаун
                    nextUseTime = currentTime + 5000; // 3 сек в миллисекундах

                    // ищем id аккаунта
                    uint32 account_id = player->GetSession()->GetAccountId();
                    // проверяем текущий баланс ЛК и кол-во валюты
                    uint32 balans_before = GetBonusDP(player);
                    uint32 coinCount = player->GetItemCount(90033, false);

                    if (coinCount == 0)
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000У вас нет Монета Donate!|r");
                        CloseGossipMenuFor(player);
                        break;
                    }

                    player->DestroyItemCount(90033, coinCount, true, false);
                    // Рассчитываем сумму для зачисления: 80% от coinCount
                    uint32 dpToAdd = coinCount * 80 / 100;

                    // Добавляем бонусные очки (на баланс ЛК)
                    AddBonusDP(player, dpToAdd);

                    ChatHandler(player->GetSession()).PSendSysMessage(
                        "Вы успешно перевели [{}] Монета-Donate на баланс ЛК.\nПолучив за это [{}] очков пожертвования.",
                        uint32(coinCount), uint32(dpToAdd)
                    );

                    // проверяем баланс после зачисления DP
                    uint32 balans_after = GetBonusDP(player);

                    // пишем логи
                    CharacterDatabase.Query(
                        "INSERT INTO `belion_logs` (account_id, nickname, balans_before, perevod, balans_after, logdate) "
                        "VALUES ({}, '{}', {}, {}, {}, CURRENT_TIMESTAMP)",
                        account_id,
                        player->GetName(),
                        balans_before,
                        dpToAdd,
                        balans_after
                    );

                    // Создаем транзакцию для сохранения инвентаря и золота
                    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
                    player->SaveInventoryAndGoldToDB(trans);
                    CharacterDatabase.CommitTransaction(trans);

                    CloseGossipMenuFor(player);
                }
                break;
                    case 1: /* Таймер мировых боссов */
                    {
                        std::ostringstream announce;
                        time_t current_time = time(0);
                        int time_stamp = int(current_time);

                        // Список GUID мировых боссов
                        std::vector<uint64> bosses = { 4360017, 4660737, 3932621, 2376962, 4352959, 4763736, 4763931, 4764200, 10495, 4770 };
                        std::vector<std::string> boss_names = {
                            "[Лорд]", "[Иллидан]", "[Эфириал]", "[Инквизитор]", "[Изера]", "[Кил'Джеден]", "[Повелитель]",
                            "[Лолита Лейн]", "[Вариан]", "[Тралл]"
                        };

                        // Формируем строки для каждого босса
                        std::string announce_message = "|cff02A4B1[Таймер Боссов]\n";  // Начальная строка

                        for (size_t i = 0; i < bosses.size(); ++i)
                        {
                            uint64 guid = bosses[i];
                            std::string boss_name = boss_names[i];

                            uint64 respawn_time = GetRespawnTime(player, guid);

                            // Проверка, если босс жив (respawn_time == 0)
                            if (respawn_time == 0)
                            {
                                announce_message += "|cff02A4B1" + boss_name + "|CFFE55BB0 Жив. Вперед, убейте его!\n";
                            }
                            else
                            {
                                // Проверка на адекватное значение времени респауна
                                int time_diff = respawn_time - time_stamp;
                                if (time_diff <= 0)
                                {
                                    announce_message += "|cff02A4B1" + boss_name + "|CFFE55BB0 Жив. Вперед, убейте его!\n";
                                }
                                else if (time_diff > 2147483647)  // Это большое значение, больше чем максимальный int
                                {
                                    announce_message += "|cff02A4B1" + boss_name + "|CFFE55BB0 Жив. Вперед, убейте его!\n";
                                }
                                else
                                {
                                    announce_message += "|cff02A4B1" + boss_name + "|CFFE55BB0 мертв, реснется через |cff02A4B1" + secsToTimeString(time_diff) + "\n";
                                }
                            }
                        }

                        // Отправляем сообщение с таймерами
                        ChatHandler(player->GetSession()).PSendSysMessage(announce_message.c_str());
                        CloseGossipMenuFor(player);
                    }
                    break;

				case 2: /* Обменик бонусов */
				{
							std::string name = player->GetName();
							std::ostringstream info;
						    info << "Приветствую, " << name << "\n\nНа вашем счету:\n|cff065961" << GetBonus(player) << "|r очков голосования.\n\n"
								<< "В данном меню вы сможете обменять ваши бонусы на [Vote-Token].\nВыберите нужный вам пункт:";

							ClearGossipMenuFor(player);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_05:25:25:-20:0|t5 бонусов на [Vote-Token]x5", GOSSIP_SENDER_MAIN, 5, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_03:25:25:-20:0|t50 бонусов на [Vote-Token]x50", GOSSIP_SENDER_MAIN, 6, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_01:25:25:-20:0|t100 бонусов на [Vote-Token]x100", GOSSIP_SENDER_MAIN, 7, "Вы уверены?", 0, false);
                            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_01:25:25:-20:0|t[Vote-Token]x10 - Перевести в ЛК", GOSSIP_SENDER_MAIN, 88, "Токены будут переведены на баланс ЛК.\n Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:25:25:-20:0|tНазад", GOSSIP_SENDER_MAIN, 4);
                            player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
				}
					break;

				case 11: /* Обменик DP => VP */
				{
							std::string name = player->GetName();
							std::ostringstream info;
							info << "Приветствую, " << name << "\n\nНа вашем счету:\n|cff065961" << GetBonus(player) << "|r очков голосования.\n|cff065961" << GetBonusDP(player) << "|r очков пожертвования.\n\n"
								<< "DP - очки пожертвования\n"
								<< "VP - очки голосования\n"
							    << "Выберите нужный вам пункт:";

							ClearGossipMenuFor(player);
							//AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_05:25:25:-20:0|t10 DP на 10 бонусов", GOSSIP_SENDER_MAIN, 8, "Вы уверены?", 0, false);
							//AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_03:25:25:-20:0|t50 DP на 50 бонусов", GOSSIP_SENDER_MAIN, 9, "Вы уверены?", 0, false);
							//AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_01:25:25:-20:0|t100 DP на 100 бонусов", GOSSIP_SENDER_MAIN, 10, "Вы уверены?", 0, false);
                            AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_06:25:25:-20:0|t100 DP на [Монета-Donate]x100", GOSSIP_SENDER_MAIN, 122, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_06:25:25:-20:0|t400 DP на [Монета-Donate]x500", GOSSIP_SENDER_MAIN, 12, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_04:25:25:-20:0|t800 DP на [Монета-Donate]x1000", GOSSIP_SENDER_MAIN, 13, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_02:25:25:-20:0|t1600 DP на [Монета-Donate]x2000", GOSSIP_SENDER_MAIN, 14, "Вы уверены?", 0, false);
							AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:25:25:-20:0|tНазад", GOSSIP_SENDER_MAIN, 4);
                            player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
				}
					break;

				case 3: /* Бонус баффы */
				{
							std::string name = player->GetName();
							std::ostringstream info;

							ClearGossipMenuFor(player);
							info << "Приветствую, " << name << "\n\nНа вашем счету |cff065961" << GetBonus(player)
								<< "|r бонусов.\n\nВы можете выдать временный бафф всем онлайн игрокам.\n"
								<< "|cff065961Баффы не выдаются игрокам на арене/бг и духам.|r\n\n"
								<< "Список доступных баффов:";

							for (uint8 i = 0; i < (sizeof(vvData) / sizeof(*vvData)); i++)
								AddGossipItemFor(player,GOSSIP_ICON_BATTLE, GetNameSpell(i), GOSSIP_SENDER_MAIN + 1, i);

							AddGossipItemFor(player,GOSSIP_ICON_BATTLE, "Назад в главное меню", GOSSIP_SENDER_MAIN, 4);
                            player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
				}
					break;

				case 4:
					OnGossipHello(player, creature);
					break;

				case 5:
				case 6:
				case 7:
				{
						  uint32 need = action == 5 ? 5 : action == 6 ? 50 : 100;
						  if (GetBonus(player) < need)
						  {
							  ChatHandler(player->GetSession()).PSendSysMessage("У вас не хватает бонусов.\nНужно {}", need);
						  }
						  else
						  {
							  DelBonus(player, need);
							  player->AddItem(90201, need);
							  ChatHandler(player->GetSession()).PSendSysMessage("Вы успешно получили [Vote-Token] x{}\nПотратив на это {} бонусов.", need, need);
						  }
						  CloseGossipMenuFor(player);
				}
					break;
                case 88:
                {
                    if (!player->HasItemCount(90201, 10))
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("У вас не хватает [Vote-Token]\n");
                    }
                    else
                    {
                        player->DestroyItemCount(90201, 10, true, false);
                        AddBonusVP(player, 10);
                        ChatHandler(player->GetSession()).PSendSysMessage("Вы успешно перевели [Vote-Token]x10 в Личный кабинет!\n");
                    }
                    CloseGossipMenuFor(player);
                }
                break;
				case 8:
				case 9:
				case 10:
				{
						   uint32 need = action == 8 ? 10 : action == 9 ? 50 : 100;
						  if (GetBonusDP(player) < need)
						  {
							  ChatHandler(player->GetSession()).PSendSysMessage("У вас не хватает очков пожертвования.\nНужно {}", need);
						  }
						  else
						  {
							  DelBonusDP(player, need);
							  AddBonusVP(player, need);
							  ChatHandler(player->GetSession()).PSendSysMessage("Вы успешно получили [{}] очков голосования.\nПотратив на это [{}] очков пожертвования.", need, need);
						  }
						  CloseGossipMenuFor(player);
				}
					break;
                case 122:
                case 12:
                case 13:
                case 14:
                case 144:
                {
                    uint32 dpCost = 0;
                    uint32 coinReward = 0;

                    // Определяем стоимость в DP и количество выдаваемых монет в зависимости от выбранного действия
                    switch (action)
                    {
                    case 122:
                        dpCost = 100;   // 100 DP
                        coinReward = 100; // 100 монет
                        break;
                    case 12:
                        dpCost = 400;   // 400 DP
                        coinReward = 500; // 500 монет
                        break;
                    case 13:
                        dpCost = 800;   // 800 DP
                        coinReward = 1000; // 1000 монет
                        break;
                    case 14:
                        dpCost = 1600;  // 1600 DP
                        coinReward = 2000; // 2000 монет
                        break;
                    }

                    // Проверяем, хватает ли у игрока DP
                    if (GetBonusDP(player) < dpCost)
                    {
                        ChatHandler(player->GetSession()).PSendSysMessage("У вас не хватает очков пожертвования.\nНужно {}", dpCost);
                    }
                    else
                    {
                        uint32 balans_before = GetBonusDP(player);
                        DelBonusDP(player, dpCost);
                        player->AddItem(90033, coinReward);
                        ChatHandler(player->GetSession()).PSendSysMessage("Вы успешно получили [{}] Монета-Donate.\nПотратив на это [{}] очков пожертвования.", coinReward, dpCost);
                        uint32 balans_after = GetBonusDP(player);

                        // Ищем id аккаунта
                        uint32 account_id = player->GetSession()->GetAccountId();

                        // Записываем лог обмена
                        CharacterDatabase.Query(
                            "INSERT INTO `belion_logs` (account_id, nickname, balans_before, perevod, balans_after, logdate, comment) "
                            "VALUES ({}, '{}', {}, {}, {}, CURRENT_TIMESTAMP, '{}')",
                            account_id,
                            player->GetName(),
                            balans_before,
                            dpCost,
                            balans_after,
                            "DP -> ITEM"
                        );
                    }
                    CloseGossipMenuFor(player);
                }
                break;
				case 15:
				{
						   if (player->HasItemCount(100502, 1) && player->HasItemCount(100503, 1) && player->HasItemCount(100504, 1))
						   {
							   CloseGossipMenuFor(player);
							   player->DestroyItemCount(100502, 1, true, false);
							   player->DestroyItemCount(100503, 1, true, false);
							   player->DestroyItemCount(100504, 1, true, false);
							   player->AddItem(100510, 1);
						   }
						   else
						   {
							   CloseGossipMenuFor(player);
							   ChatHandler(player->GetSession()).PSendSysMessage("Необходимо иметь все Доступ-Карты [3]");
						   }
				}
					break;
                case 154: /* Обменик DP => VP */
                {
                    std::string name = player->GetName();
                    std::ostringstream info;
                    info << "Приветствую, " << name << "\n\nНа вашем счету:\n|cff065961" << GetBonus(player) << "|r очков голосования.\n|cff065961" << GetBonusDP(player) << "|r очков пожертвования.\n\n"
                        << "В данном меню вы можете обменять |cff065961[Монета-Donate]|r на |cff065961[Vote-Token]|r\n"
                        << "MD - [Монета-Donate]\n"
                        << "VT - [Vote-Token]\n"
                        << "Выберите нужный вам пункт:";

                    ClearGossipMenuFor(player);
                    AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_06:25:25:-20:0|t[MD]x10 -> [VT]x10", GOSSIP_SENDER_MAIN, 155, "Вы уверены?", 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_04:25:25:-20:0|t[MD]x50 -> [VT]x50", GOSSIP_SENDER_MAIN, 156, "Вы уверены?", 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_02:25:25:-20:0|t[MD]x100 -> [VTx100", GOSSIP_SENDER_MAIN, 157, "Вы уверены?", 0, false);
                    AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:25:25:-20:0|tНазад", GOSSIP_SENDER_MAIN, 4);
                    player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
                }
                break;
                case 155:
                {
                    if (player->HasItemCount(90033, 10))
                    {
                        CloseGossipMenuFor(player);
                        player->DestroyItemCount(90033, 10, true, false);
                        player->AddItem(90201, 10);
                        ChatHandler(player->GetSession()).PSendSysMessage("Вы получили [10] Vote-Token.\nПотратив на это [10] Moнета-Donate.");
                    }
                    else
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).PSendSysMessage("Недостаточно -  Монета [Donate]");
                    }
                }
                break;
                case 156:
                {
                    if (player->HasItemCount(90033, 50))
                    {
                        CloseGossipMenuFor(player);
                        player->DestroyItemCount(90033, 50, true, false);
                        player->AddItem(90201, 50);
                        ChatHandler(player->GetSession()).PSendSysMessage("Вы получили [50] Vote-Token.\nПотратив на это [50] Moнета-Donate.");
                    }
                    else
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).PSendSysMessage("Недостаточно -  Монета [Donate]");
                    }
                }
                break;
                case 157:
                {
                    if (player->HasItemCount(90033, 100))
                    {
                        CloseGossipMenuFor(player);
                        player->DestroyItemCount(90033, 100, true, false);
                        player->AddItem(90201, 100);
                        ChatHandler(player->GetSession()).PSendSysMessage("Вы получили [100] Vote-Token.\nПотратив на это [100] Moнета-Donate.");
                    }
                    else
                    {
                        CloseGossipMenuFor(player);
                        ChatHandler(player->GetSession()).PSendSysMessage("Недостаточно -  Монета [Donate]");
                    }
                }
                break;
				case 16:
				{
						   std::string name = player->GetName();
						   std::ostringstream info;
						   info << "Приветствую, " << name << "\n\nНа вашем счету |cff065961" << GetBonus(player) << "|r бонусов.\n\n"
							   << "В данном меню вы сможете обменять ваши [Доступ]карты на одну:\n-Доступ 255 \n-Доступ 300\n-Доступ в Огненные Недра\nКроме [Доступ в Крепость Бурь]\n";

						   ClearGossipMenuFor(player);
						   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_pvp_o_05:25:25:-20:0|tОбменять Доступ-Карты|r", GOSSIP_SENDER_MAIN, 15);
						   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:25:25:-20:0|tНазад", GOSSIP_SENDER_MAIN, 4);
                           player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
				}
					break;
				case 17:
				{
						   std::string name = player->GetName();
						   std::ostringstream info;
						   info << "Приветствую, " << name << "\n\nНа вашем счету |cff065961" << GetBonus(player) << "|r бонусов.\n\n"
							   << "В данном меню вы можете изменить имя или фракцию персонажа\n\n";

						   ClearGossipMenuFor(player);
						   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:25:25:-20:0|tИзменить Имя|r - [Vote-Token] х10", GOSSIP_SENDER_MAIN, 18);
						   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:25:25:-20:0|tИзменить фракцию|r - [Vote-Token] х20", GOSSIP_SENDER_MAIN, 20);
						//   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:25:25:-20:0|tИсправить [Вы не знаете этого языка]", GOSSIP_SENDER_MAIN, 21);
						   AddGossipItemFor(player,GOSSIP_ICON_DOT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:25:25:-20:0|tНазад", GOSSIP_SENDER_MAIN, 4);
                           player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), creature->GetGUID());
				}
					break;
				case 18:
				{
						   if (player->HasItemCount(90201, 10))
						   {
							   CloseGossipMenuFor(player);
							   player->DestroyItemCount(90201, 20, true, false);
							   player->SetAtLoginFlag(AT_LOGIN_RENAME);
							   creature->Whisper("Перезайдите и введите новое имя персонажа. Не забудьте после изменения имени выйти и удалить из папки клиента кэш!", LANG_UNIVERSAL, player);
						   }
						   else
						   {
							   CloseGossipMenuFor(player);
							   creature->Whisper("Недостаточно Vote-Token.", LANG_UNIVERSAL, player);
						   }
				}
					break;
				case 20:
				{
						   if (player->HasItemCount(90201, 20))
						   {
							   CloseGossipMenuFor(player);
							   player->DestroyItemCount(90201, 20, true, false);
							   player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
							   creature->Whisper("Перезайдите для смены фракции. Не забудьте после изменения имени выйти и удалить из папки клиента кэш!", LANG_UNIVERSAL, player);
						   }
						   else
						   {
							   CloseGossipMenuFor(player);
							   creature->Whisper("Недостаточно Vote-Token.", LANG_UNIVERSAL, player);
						   }
				}
					break;
				case 21:
				{
						   if (player->GetTeamId() == TEAM_HORDE)
						   {
							   CloseGossipMenuFor(player);
							   player->removeSpell(669, SPEC_MASK_ALL, false);
							   player->learnSpell(669);
							   player->SaveToDB(false, false);
						   }
						   else
						   {
							   CloseGossipMenuFor(player);
							   player->removeSpell(668, SPEC_MASK_ALL, false);
							   player->learnSpell(668);
							   player->SaveToDB(false, false);
						   }
						   creature->Whisper("Исправленно, теперь вы можете говорить.", LANG_UNIVERSAL, player);
				}
					break;
				}
			}
			if (sender == GOSSIP_SENDER_MAIN + 1)
			{
				if (GetBonus(player) < vvData[action].Cost)
					ChatHandler(player->GetSession()).PSendSysMessage("У вас не хватает бонусов.");
				else
				{
					std::string name = player->GetName();
					std::ostringstream message;
					message << "|cff5da673[BELION] |CFFE55BB0" << name << "|CFFFE8A0E баффнул всех игроков онлайн баффом [|cffEAF4F5" << vvData[action].Name << "|cff02A4B1]|r";
					GetBuffOnline(action);

					CharacterDatabase.Query("UPDATE fusion.account_data SET vp = vp - {} WHERE id = {}", vvData[action].Cost, player->GetSession()->GetAccountId());

					sWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, message.str().c_str());
					CloseGossipMenuFor(player);
				}
			}
			return true;
		}
};

/*
class Login_script : public PlayerScript
{
public:
	Login_script() : PlayerScript("Login_script") {}

	void OnLogin(Player* player)
	{
		uint32 accId = player->GetSession()->GetAccountId();

		QueryResult result = CharacterDatabase.Query("SELECT vp FROM fusion.account_data WHERE id = {}", accId);
		if (!result)
			CharacterDatabase.Query("INSERT INTO fusion.account_data (id, dp, vp) VALUES ({}, 0, 0)", accId);

	}
};
*/
void AddSC_npc_bonus_buff()
{
	new npc_bonus_buff();
    /*new Login_script(); */
}
