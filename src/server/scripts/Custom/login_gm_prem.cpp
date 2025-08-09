#include "ScriptPCH.h"
#include "TicketMgr.h"
#include "GameTime.h"
#include "WorldSessionMgr.h"

class gm_login : public PlayerScript
{
public:
	gm_login() : PlayerScript("gm_login") { }

	void OnPlayerLogin(Player* player)
	{
		{
            // Временный дебаф
            player->removeSpell(90001, SPEC_MASK_ALL, false);
            player->RemoveAurasDueToSpell(90001);
			if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
			{
				ChatHandler handler(player->GetSession());
				uint32 accid = player->GetSession()->GetAccountId();
				uint16 gmlvl = player->GetSession()->GetSecurity();
				std::string gmname = player->GetName();
				uint32 playeronline = sWorldSessionMgr->GetPlayerCount();
				std::string uptime = secsToTimeString(GameTime::GetUptime().count()).c_str();
				std::string player_ip = player->GetSession()->GetRemoteAddress();
				uint16 tickets = sTicketMgr->GetOpenTicketCount();

				handler.PSendSysMessage("|cffff0000=================================|r");
				handler.PSendSysMessage("|cff00ff00Привет,|r {}", gmname.c_str());
				handler.PSendSysMessage("|cff00ff00Ваш уровень учетной записи:|r {}", gmlvl);
				handler.PSendSysMessage("|cff00ff00Ваш IP:|r {}", player_ip.c_str());
				handler.PSendSysMessage("|cff00ff00Сейчас|r {} |cff00ff00игроков онлайн|r", playeronline);
				handler.PSendSysMessage("|cff00ff00Открытые тикеты:|r {}", tickets);
				handler.PSendSysMessage("|cff00ff00Время работы сервера:|r {}", uptime.c_str());
				handler.PSendSysMessage("|cffff0000=================================|r");
				return;
			}
		}
		if (player->GetSession()->IsPremium())
		{
			ChatHandler handler(player->GetSession());
			uint32 accid = player->GetSession()->GetAccountId();
			std::string gmname = player->GetName();
			uint32 playeronline = sWorldSessionMgr->GetPlayerCount();
			std::string uptime = secsToTimeString(GameTime::GetUptime().count()).c_str();
			std::string player_ip = player->GetSession()->GetRemoteAddress();
			uint16 tickets = sTicketMgr->GetOpenTicketCount();
			handler.PSendSysMessage("|cfffcc141Привет,|r|cff8ab6fc {}|r", gmname.c_str());
			handler.PSendSysMessage("|cfffcc141Сейчас|r |cff8ab6fc{}|r |cfffcc141игроков онлайн|r", playeronline);
			handler.PSendSysMessage("|cfffcc141Время работы сервера:|r |cff8ab6fc{}|r", uptime.c_str());
			handler.PSendSysMessage("|cfffcc141Доступные команды:|r");
            handler.PSendSysMessage("|cff8ab6fc[.vip menu]|cfffcc141 - Меню VIP|r");
			handler.PSendSysMessage("|cff8ab6fc[.vip bank]|cfffcc141 - открыть банк|r");
			handler.PSendSysMessage("|cff8ab6fc[.vip mail]|cfffcc141 - открыть почту|r");
            handler.PSendSysMessage("|cff8ab6fc[.vip home]|cfffcc141 - телепорт в Столицу|r");
            handler.PSendSysMessage("|cff8ab6fc[.vip buff]|cfffcc141 - Баффы|r");
            handler.PSendSysMessage("|cff8ab6fc[.ins un all]|cfffcc141 - сбросить КД подземелий|r");
            handler.PSendSysMessage("|cff8ab6fc[.mir all|cfffcc141 - таймер Мировых боссов|r");
            handler.PSendSysMessage("|cfffcc141[Рейты:] Репутация: х2");
			handler.PSendSysMessage("|cfffcc141Приятной игры.|r|TInterface/ICONS/Achievement_bg_tophealer_wsg:15|t");
            player->CastSpell(player, 90000, true);
            player->CastSpell(player, 90002, true);
			QueryResult result = LoginDatabase.Query("SELECT unsetdate FROM account_premium WHERE id = {}", accid);
			if (result)
			{
				Field * field = result->Fetch();
				uint32 unsetdate = field[0].Get<uint32>();
				std::string timeStr = secsToTimeString(unsetdate - time(NULL), false);
				handler.PSendSysMessage("|cff8ab6fc[VIP] |cfffcc141Аккаунт закончится через:|r");
				handler.PSendSysMessage("|cff8ab6fc{}|r", timeStr.c_str());
			}
			return;
		}
		else
		{
			ChatHandler handler(player->GetSession());
			handler.PSendSysMessage("|cff8ab6fc[VIP] |cfffcc141Премиум аккаунт не активен.|r");
            handler.PSendSysMessage("|cfffcc141Доступные команды:|r");
            handler.PSendSysMessage("|cff8ab6fc[.ins un all]|cfffcc141 - сбросить КД подземелий|r");
            handler.PSendSysMessage("|cff8ab6fc[.mir all|cfffcc141 - таймер Мировых боссов|r");
			player->DestroyItemCount(68912, 1, true, false);
			player->DestroyItemCount(68913, 1, true, false);
			player->DestroyItemCount(68914, 1, true, false);
			player->DestroyItemCount(80052, 1, true, false);
			player->DestroyItemCount(80053, 1, true, false);
			player->DestroyItemCount(80054, 1, true, false);
			player->DestroyItemCount(80055, 1, true, false);
			player->DestroyItemCount(80056, 1, true, false);
			player->DestroyItemCount(80057, 1, true, false);
			player->DestroyItemCount(80058, 1, true, false);
			player->DestroyItemCount(80059, 1, true, false);
			player->DestroyItemCount(80060, 1, true, false);
			player->DestroyItemCount(80061, 1, true, false);
			player->DestroyItemCount(80062, 1, true, false);
            player->DestroyItemCount(80085, 1, true, false);
			player->DestroyItemCount(80090, 1, true, false);
            player->DestroyItemCount(300048, 1, true, false);
			player->removeSpell(300118, SPEC_MASK_ALL, false);
			player->removeSpell(300048, SPEC_MASK_ALL, false);
			player->removeSpell(300194, SPEC_MASK_ALL, false);
			player->removeSpell(90000, SPEC_MASK_ALL, false);
            player->removeSpell(90001, SPEC_MASK_ALL, false);
            player->removeSpell(90002, SPEC_MASK_ALL, false);
            player->RemoveAurasDueToSpell(90000);
            player->RemoveAurasDueToSpell(90001);
            player->RemoveAurasDueToSpell(90002);
			CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(178);
			player->SetTitle(titleInfo, true);
            player->SaveToDB(false, false);
		}
	}
};

void AddSC_gm_login()
{
	new gm_login();
}
