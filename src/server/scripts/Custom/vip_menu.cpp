#include "Locale.h"
#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "StringConvert.h"
#include "Tokenize.h"
#include "ScriptedGossip.h"
#include "SpellMgr.h"
#include "World.h"
#include "PlayerDump.h"
#include "PoolMgr.h"
#include "GossipDef.h"

#define VIP_MENU_ID        50001
#define DEFAULT_GOSSIP_MESSAGE "Что ты хочешь?"
#define CONST_ARENA_RENAME 0
#define CONST_ARENA_CUSTOMIZE 0
#define CONST_ARENA_CHANGE_FACTION 0
#define CONST_ARENA_CHANGE_RACE 0

#define EMOTE_NO_VIP "|cffff6060[Ошибка]:|r Доступно только для VIP-Аккаунтов!"
#define EMOTE_COOLDOWN "Не могу сделать это сейчас..."
#define EMOTE_NO_SICKENSS "У вас нет ауры 'Слабость после воскрешения'!"
#define EMOTE_NO_DESERTER "У вас нет ауры 'Дезертир'!"
#define EMOTE_COMBAT "Вы находитесь в бою!"
#define EMOTE_NO_ARENA_POINTS "Недостаточно очков арены!"
#define EMOTE_ALREADY_ITEM "Эта вещь у Вас уже есть!"
#define EMOTE_TALENT_RESET "Таланты успешно сброшены!"

#define MSG_RENAME_COMPLETE "Перезайдите и введите новое имя персонажа. Не забудьте после изменения имени выйти и удалить из папки клиента кэш!"
#define MSG_CUSTOMIZE_COMPLETE "Перезайдите и измените внешность персонажа. Не забудьте после изменения внешности выйти и удалить из папки клиента кэш!"
#define MSG_CHANGE_FACTION_COMPLETE "Перезайдите и измените фракцию персонажа. После смены Фракции цепочки квестов будут обнулены!" 
#define MSG_CHANGE_RACE_COMPLETE "Перезайдите и измените расу персонажа. Не забудьте после изменения расы выйти и удалить из папки клиента кэш!"
#define MSG_MAX_SKILL "Ваши навыки повышены до максимума!"
#define MSG_REMOVE_SICKNESS_COMPLETE "'Слабость после воскрешения' удалена. Восстановлены HP & Mana!"
#define MSG_REMOVE_DESERTER_COMPLETE "'Дезертир' удалён. Вы можите снова вернуться на поле боя!"
#define MSG_RIDING_COMPLETE "Ваш навык верховой езды повышен до максимума!"
#define MSG_RESET_COOLDOWN "Ваше время восстановления сброшено!"
#define MSG_CHARACTER_SAVE_TO_DB "Ваш персонаж сохранён!"
#define MSG_RESET_QUEST_STATUS_COMPLETE "Ваши ежедневные и еженедельные задания обновлены!"

using namespace Acore::ChatCommands;

enum Trainers
{
    // Alliance
    DRUID_A = 5504,
    HUNTER_A = 5515,
    MAGE_A = 5497,
    PALADIN_A = 928,
    PRIEST_A = 376,
    ROGUE_A = 918,
    SHAMAN_A = 20407,
    WARLOCK_A = 461,
    WARRIOR_A = 5479,

    // Horde
    DRUID_H = 3033,
    HUNTER_H = 3406,
    MAGE_H = 5883,
    PALADIN_H = 23128,
    PRIEST_H = 3045,
    ROGUE_H = 3401,
    SHAMAN_H = 3344,
    WARLOCK_H = 3324,
    WARRIOR_H = 3354,

    DEATHKNIGHT_AH = 28472
};

namespace
{
    void SummonTempNPC(Player* player, uint32 entry, const char* salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sConfigMgr->GetOption<int32>("Premium.NpcDuration", 60) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 60;

        Creature* npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST - 0.5, player->GetFollowAngle());
        npc->SetFaction(35);

        if (salute && !(salute[0] == '\0'))
            npc->Whisper(salute, LANG_UNIVERSAL, player, false);
    }
}

// Chat command to open VIP menu
class vip_command : public CommandScript
{
public:
    vip_command() : CommandScript("vip_command") {}

    ChatCommandTable GetCommands() const override
    {

        static ChatCommandTable vipCommandTable =
        {
            { "menu", HandleVipMenuCommand, SEC_PLAYER, Console::No},
        };

        static ChatCommandTable commandTable =
        {
            { "vip", vipCommandTable                                      },
        };
        return commandTable;
    }

    static bool HandleVipMenuCommand(ChatHandler* handler, const char* /*args*/)
    {
        // Проверяем, включена ли команда в конфиге
        if (!sConfigMgr->GetOption<bool>("Premium.VipCommandEnable", true))
        {
            handler->SendSysMessage("|cffff6060[Ошибка]:|r Команда VIP отключена на сервере.");
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();
        if (!player->GetSession()->IsPremium() && !player->IsGameMaster())
        {
            handler->SendSysMessage("|cffff6060[Ошибка]:|r Доступно только для VIP-Аккаунтов!");
            return false;
        }

        OpenVipMenu(player, 0);
        return true;
    }

    // Возвращает нужную форму: form1 – для 1, form2 – для 2–4, form5 – для остальных
    static const char* PluralForm(int32 n, const char* form1, const char* form2, const char* form5)
    {
        n = n % 100;
        if (n >= 11 && n <= 19)
            return form5;
        int32 last = n % 10;
        if (last == 1)
            return form1;
        if (last >= 2 && last <= 4)
            return form2;
        return form5;
    }

    static void OpenVipMenu(Player* player, uint32 action)
    {
        ClearGossipMenuFor(player);
        player->PlayerTalkClass->GetGossipMenu().SetMenuId(VIP_MENU_ID);

        // ——— собираем динамический заголовок ———
        std::ostringstream info;
        // имя и время окончания
        std::string name = player->GetName();
        info << "Приветствую, " << name << "!\n\n";

        // время до конца VIP
        uint32 accId = player->GetSession()->GetAccountId();
        if (QueryResult result = LoginDatabase.Query(
            "SELECT unsetdate FROM account_premium WHERE id = {}", accId))
        {
            uint32 unsetdate = result->Fetch()[0].Get<uint32>();
            int32 secsLeft = int32(unsetdate) - int32(time(nullptr));
            if (secsLeft > 0)
            {
                int32 days = secsLeft / (24 * 3600);
                int32 hours = (secsLeft % (24 * 3600)) / 3600;
                int32 minutes = (secsLeft % 3600) / 60;

                info << "Аккаунт закончится через:\n"
                    << days << ' ' << PluralForm(days, "день", "дня", "дней") << ", "
                    << hours << ' ' << PluralForm(hours, "час", "часа", "часов") << ", "
                    << minutes << ' ' << PluralForm(minutes, "минута", "минуты", "минут");
            }
        }

        switch (action)
        {
        case 0: // Главный
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_note_02:20|t Получить Звание [VIP]", GOSSIP_SENDER_MAIN, 9500);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/spell_shadow_deathscream:20|t Снять [Слабость после воскрешения]", GOSSIP_SENDER_MAIN, 1209);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/ability_druid_cower:20|t Снять [Дезертир]", GOSSIP_SENDER_MAIN, 1210);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_holy_healingaura:20|t Вылечить персонажа", GOSSIP_SENDER_MAIN, 1202);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_08:20|t Сохранить персонажа", GOSSIP_SENDER_MAIN, 1213);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_16:20|t Аукционер ->", GOSSIP_SENDER_MAIN, 14);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_book_04:20|t ViP-Бафы ", GOSSIP_SENDER_MAIN, 6999);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_hunter_ferociousinspiration:20|t ViP-Маунт", GOSSIP_SENDER_MAIN, 1216);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/icon_roga:20|t ViP-Диадема", GOSSIP_SENDER_MAIN, 1231);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_shoulder_56:20|t ViP-Крылья ->", GOSSIP_SENDER_MAIN, 1220);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_coin_09:20|t Превращения ->", GOSSIP_SENDER_MAIN, 1203);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Inv_misc_book_07:20|t Обычные бафы", GOSSIP_SENDER_MAIN, 1206);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:20|t Изменить персонажа", GOSSIP_SENDER_MAIN, 2000);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Achievement_reputation_argentchampion:20|t Сбросить таланты", GOSSIP_SENDER_MAIN, 1207, "Таланты будут сброшены. Вы уверены?", 0, false);
            //AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_dualwield:20|t Изучить оружие", GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/inv_misc_book_11:20|t Классовый мастер ->", GOSSIP_SENDER_MAIN, 12);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/inv_misc_book_04:20|t Мастер профессии ->", GOSSIP_SENDER_MAIN, 13);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_mount_charger:20|t Редкие животные ->", GOSSIP_SENDER_MAIN, 1208);
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_mount_gyrocoptor:20|t Элитные животные ->", GOSSIP_SENDER_MAIN, 1217);
            break;

        case 12: /* Class Trainers*/
        {
            uint32 trainerId = 0;
            switch (player->getClass())
            {
            case CLASS_ROGUE:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? ROGUE_A : ROGUE_H;
                break;
            case CLASS_WARRIOR:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARRIOR_A : WARRIOR_H;
                break;
            case CLASS_PRIEST:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PRIEST_A : PRIEST_H;
                break;
            case CLASS_MAGE:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? MAGE_A : MAGE_H;
                break;
            case CLASS_PALADIN:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? PALADIN_A : PALADIN_H;
                break;
            case CLASS_HUNTER:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? HUNTER_A : HUNTER_H;
                break;
            case CLASS_DRUID:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? DRUID_A : DRUID_H;
                break;
            case CLASS_SHAMAN:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? SHAMAN_A : SHAMAN_H;
                break;
            case CLASS_WARLOCK:
                trainerId = player->GetTeamId() == TEAM_ALLIANCE ? WARLOCK_A : WARLOCK_H;
                break;
            case CLASS_DEATH_KNIGHT:
                trainerId = DEATHKNIGHT_AH;
                break;
            }

            SummonTempNPC(player, trainerId);
                   // после спавна — вернуть главное меню
                vip_command::OpenVipMenu(player, 0);
            return;
        }
        case 14: // Аукционер
            SummonTempNPC(player, 17629);
            CloseGossipMenuFor(player);
            return;
        case 13: // Профессии
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Алхимия", GOSSIP_SENDER_MAIN, 20);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Кузнечное дело", GOSSIP_SENDER_MAIN, 21);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Кожевничество", GOSSIP_SENDER_MAIN, 22);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Портняжное дело", GOSSIP_SENDER_MAIN, 23);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Инженерное дело", GOSSIP_SENDER_MAIN, 24);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Наложение чар", GOSSIP_SENDER_MAIN, 25);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Начертание", GOSSIP_SENDER_MAIN, 32);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Ювелирное дело", GOSSIP_SENDER_MAIN, 26);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Травничество", GOSSIP_SENDER_MAIN, 27);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Снятие шкур", GOSSIP_SENDER_MAIN, 28);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Горное дело", GOSSIP_SENDER_MAIN, 29);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Кулинария", GOSSIP_SENDER_MAIN, 30);
            AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Первая помощь", GOSSIP_SENDER_MAIN, 31);
            break;
        
        case 1208: // Редкие ездовые маунты
            AddGossipItemFor(player, 5, "Чёрный киражский резонирующий кристалл", GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 5, "Боевой медведь Амани", GOSSIP_SENDER_MAIN, 101);
            AddGossipItemFor(player, 5, "Большой боевой медведь", GOSSIP_SENDER_MAIN, 102);
            AddGossipItemFor(player, 5, "Поводья рыска смерти", GOSSIP_SENDER_MAIN, 103);
            AddGossipItemFor(player, 5, "Поводья огненной боевой лошади", GOSSIP_SENDER_MAIN, 104);
            AddGossipItemFor(player, 5, "Стремительный багровый волк", GOSSIP_SENDER_MAIN, 105);
            AddGossipItemFor(player, 5, "Великий кодо пивного фестиваля", GOSSIP_SENDER_MAIN, 106);
            AddGossipItemFor(player, 5, "Рог воющего ледяного волка", GOSSIP_SENDER_MAIN, 107);
            AddGossipItemFor(player, 5, "Волшебное яйцо-петух", GOSSIP_SENDER_MAIN, 108);
            AddGossipItemFor(player, 5, "Поводья белого полярного медведя", GOSSIP_SENDER_MAIN, 109);
            AddGossipItemFor(player, 5, "Поводья коричневого полярного медведя", GOSSIP_SENDER_MAIN, 110);
            AddGossipItemFor(player, 5, "Поводья багрового рыска смерти", GOSSIP_SENDER_MAIN, 111);
            AddGossipItemFor(player, 5, "Поводья Владыки Воронов", GOSSIP_SENDER_MAIN, 112);
            AddGossipItemFor(player, 5, "Поводья стремительного призрачного тигра", GOSSIP_SENDER_MAIN, 113);
            AddGossipItemFor(player, 5, "Морская черепаха", GOSSIP_SENDER_MAIN, 114);
            AddGossipItemFor(player, 5, "Стремительный раззашский ящер", GOSSIP_SENDER_MAIN, 115);
            AddGossipItemFor(player, 5, "Стремительный белый ястребиный ездок", GOSSIP_SENDER_MAIN, 116);
            AddGossipItemFor(player, 5, "Стремительная жевра", GOSSIP_SENDER_MAIN, 117);
            AddGossipItemFor(player, 5, "Стремительный зулианский тигр", GOSSIP_SENDER_MAIN, 118);
            AddGossipItemFor(player, 5, "Поводья всадника без головы", GOSSIP_SENDER_MAIN, 119);
            AddGossipItemFor(player, 5, "Шерстистый белый носорог", GOSSIP_SENDER_MAIN, 120);
            AddGossipItemFor(player, 5, "Четыре киражских резонирующих кристалла", GOSSIP_SENDER_MAIN, 121);
            break;

        case 1217: // Элитные ездовые маунты
            AddGossipItemFor(player, 5, "Серебристый гиппогриф", GOSSIP_SENDER_MAIN, 122);
            AddGossipItemFor(player, 5, "Пепел Алар", GOSSIP_SENDER_MAIN, 123);
            AddGossipItemFor(player, 5, "Пылающий гиппогриф", GOSSIP_SENDER_MAIN, 125);
            AddGossipItemFor(player, 5, "Стремительный дракон Пустоты", GOSSIP_SENDER_MAIN, 126);
            AddGossipItemFor(player, 5, "Мстительный дракон Пустоты", GOSSIP_SENDER_MAIN, 127);
            AddGossipItemFor(player, 5, "Безжалостный дракон Пустоты", GOSSIP_SENDER_MAIN, 128);
            AddGossipItemFor(player, 5, "Жестокий дракон Пустоты", GOSSIP_SENDER_MAIN, 129);
            AddGossipItemFor(player, 5, "Небесный скакун", GOSSIP_SENDER_MAIN, 130);
            AddGossipItemFor(player, 5, "Мрачный ледяной дракон гладиатора", GOSSIP_SENDER_MAIN, 131);
            AddGossipItemFor(player, 5, "Яростный ледяной дракон гладиатора", GOSSIP_SENDER_MAIN, 132);
            AddGossipItemFor(player, 5, "Непреклонный ледяной дракон гладиатора", GOSSIP_SENDER_MAIN, 133);
            AddGossipItemFor(player, 5, "Гневный ледяной дракон гладиатора", GOSSIP_SENDER_MAIN, 134);
            AddGossipItemFor(player, 5, "Поводья непобедимых", GOSSIP_SENDER_MAIN, 135);
            AddGossipItemFor(player, 5, "Голова Мимирона", GOSSIP_SENDER_MAIN, 136);
            AddGossipItemFor(player, 5, "Поводья альбиносного дракона", GOSSIP_SENDER_MAIN, 137);
            AddGossipItemFor(player, 5, "Поводья лазурного дракона", GOSSIP_SENDER_MAIN, 138);
            AddGossipItemFor(player, 5, "Поводья чёрного дракона", GOSSIP_SENDER_MAIN, 139);
            AddGossipItemFor(player, 5, "Поводья синего дракона", GOSSIP_SENDER_MAIN, 140);
            AddGossipItemFor(player, 5, "Поводья бронзового дракона", GOSSIP_SENDER_MAIN, 141);
            AddGossipItemFor(player, 5, "Поводья сумеречного дракона", GOSSIP_SENDER_MAIN, 152);
            AddGossipItemFor(player, 5, "Поводья ониксного дракона", GOSSIP_SENDER_MAIN, 142);
            AddGossipItemFor(player, 5, "Адская ракета X-51 X-TREME", GOSSIP_SENDER_MAIN, 143);
            AddGossipItemFor(player, 5, "Туристическая ракета X-53", GOSSIP_SENDER_MAIN, 144);
            break;

        case 1220: // ViP-Wings
            AddGossipItemFor(player, 5, "|TInterface/ICONS/Inv_shoulder_56:20|t ViP-Wings [Сила]", GOSSIP_SENDER_MAIN, 7003);
            AddGossipItemFor(player, 5, "|TInterface/ICONS/Inv_shoulder_56:20|t ViP-Wings [Ловкость]", GOSSIP_SENDER_MAIN, 7004);
            AddGossipItemFor(player, 5, "|TInterface/ICONS/Inv_shoulder_56:20|t ViP-Wings [Интеллект]", GOSSIP_SENDER_MAIN, 7005);
            break;

        case 6999: // ViP-Бафы
            player->CastSpell(player, 90000, true);
            player->CastSpell(player, 90002, true);
            ChatHandler(player->GetSession()).PSendSysMessage("ViP-бафы наложены.");
            // вернуть главное меню
            vip_command::OpenVipMenu(player, 0);
            return;

        case 1216: // VIP-Маунт
            if (player->HasItemCount(300048, 1))
                ChatHandler(player->GetSession()).PSendSysMessage(EMOTE_ALREADY_ITEM);
            else
                {
                player->AddItem(300048, 1);
                player->AddItem(300118, 1);
                player->AddItem(300194, 1);
                ChatHandler(player->GetSession()).PSendSysMessage("ViP-Маунты выданы.");
                }
               // после выдачи — вернуть главное меню
                vip_command::OpenVipMenu(player, 0);
            return;

        case 1203: // Превращения
            // morph submenu
            AddGossipItemFor(player, 5, "Бронированный мурлок", GOSSIP_SENDER_MAIN, 499);
            AddGossipItemFor(player, 5, "Гном", GOSSIP_SENDER_MAIN, 500);
            AddGossipItemFor(player, 5, "Джонатан Богослов", GOSSIP_SENDER_MAIN, 501);
            AddGossipItemFor(player, 5, "Маги", GOSSIP_SENDER_MAIN, 502);
            AddGossipItemFor(player, 5, "Ворген", GOSSIP_SENDER_MAIN, 503);
            AddGossipItemFor(player, 5, "Пандарен монах", GOSSIP_SENDER_MAIN, 504);
            AddGossipItemFor(player, 5, "Крок Гроза Плети", GOSSIP_SENDER_MAIN, 505);
            AddGossipItemFor(player, 5, "Железный дворф", GOSSIP_SENDER_MAIN, 506);
            AddGossipItemFor(player, 5, "Друид", GOSSIP_SENDER_MAIN, 507);
            AddGossipItemFor(player, 5, "Жрец", GOSSIP_SENDER_MAIN, 508);
            AddGossipItemFor(player, 5, "Паладин", GOSSIP_SENDER_MAIN, 509);
            AddGossipItemFor(player, 5, "Разбойник", GOSSIP_SENDER_MAIN, 510);
            AddGossipItemFor(player, 5, "Рыцарь смерти", GOSSIP_SENDER_MAIN, 511);
            AddGossipItemFor(player, 5, "Чернокнижник", GOSSIP_SENDER_MAIN, 512);
            AddGossipItemFor(player, 5, "Воин", GOSSIP_SENDER_MAIN, 513);
            AddGossipItemFor(player, 5, "Маг", GOSSIP_SENDER_MAIN, 514);
            AddGossipItemFor(player, 5, "Шаман", GOSSIP_SENDER_MAIN, 515);
            AddGossipItemFor(player, 5, "Охотник", GOSSIP_SENDER_MAIN, 516);
            AddGossipItemFor(player, 5, "Белая дренейка", GOSSIP_SENDER_MAIN, 517);
            AddGossipItemFor(player, 5, "Черная дренейка", GOSSIP_SENDER_MAIN, 518);
            AddGossipItemFor(player, 0, "Снять превращение", GOSSIP_SENDER_MAIN, 519);
            break;

        case 1206: // Баффы
            AddGossipItemFor(player, 5, "Молитва стойкости", GOSSIP_SENDER_MAIN, 4000);
            AddGossipItemFor(player, 5, "Молитва духа", GOSSIP_SENDER_MAIN, 4001);
            AddGossipItemFor(player, 5, "Молитва защиты от темных сил", GOSSIP_SENDER_MAIN, 4002);
            AddGossipItemFor(player, 5, "Великое благословение королей", GOSSIP_SENDER_MAIN, 4003);
            AddGossipItemFor(player, 5, "Великое благословение могущества", GOSSIP_SENDER_MAIN, 4004);
            AddGossipItemFor(player, 5, "Великое благословение мудрости", GOSSIP_SENDER_MAIN, 4005);
            AddGossipItemFor(player, 5, "Великое благословение неприкосновенности", GOSSIP_SENDER_MAIN, 4006);
            AddGossipItemFor(player, 5, "Чародейский интеллект", GOSSIP_SENDER_MAIN, 4007);
            AddGossipItemFor(player, 5, "Ослабление магии", GOSSIP_SENDER_MAIN, 4008);
            AddGossipItemFor(player, 5, "Усиление магии", GOSSIP_SENDER_MAIN, 4009);
            AddGossipItemFor(player, 5, "Знак дикой природы", GOSSIP_SENDER_MAIN, 4010);
            AddGossipItemFor(player, 5, "Шипы", GOSSIP_SENDER_MAIN, 4011);
            AddGossipItemFor(player, 5, "Гениальность", GOSSIP_SENDER_MAIN, 4012);
            break;

        case 2000: // Изменить персонажа
            AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface/ICONS/Trade_engineering:25|t Изменить внешность", GOSSIP_SENDER_MAIN, 2001);
            break;

        default:
            break;
        }
        // SendGossipMenu автоматически возьмёт текст из SetText(), если он был установлен выше
        player->PlayerTalkClass->SendGossipMenu(info.str().c_str(), player->GetGUID());
    }
};

// PlayerScript for handling selections
class vip_menu : public PlayerScript
{
public:
    vip_menu() : PlayerScript("vip_menu") {}

    void SummonTempNPC(Player* player, uint32 entry, const char* salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sConfigMgr->GetOption<int32>("Premium.NpcDuration", 60) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 60;

        Creature* npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST - 0.5, player->GetFollowAngle());
        npc->SetFaction(35);

        if (salute && !(salute[0] == '\0'))
            npc->Whisper(salute, LANG_UNIVERSAL, player, false);
    }

    void OnPlayerGossipSelect(Player* player, uint32 menuId, uint32 /*sender*/, uint32 action) override
    {
        if (menuId != VIP_MENU_ID)
            return;
        
        if (!player->getAttackers().empty())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Невозможно использовать в бою.");
            CloseGossipMenuFor(player);
            return;
        }

        player->PlayerTalkClass->ClearMenus();
        
        switch (action)
        {
        // Навигация
        case 0:
        case 12:
        case 13:
        case 14:
        case 1208:
        case 1217:
        case 1218:
        case 1220:
        case 1203:
        case 1206:
        case 6999:
        case 1216:
        case 2000:
            vip_command::OpenVipMenu(player, action);
            return;

        // Summon professions NPCs
        case 20: SummonTempNPC(player, 19052); break;
        case 21: SummonTempNPC(player, 33591); break;
        case 22: SummonTempNPC(player, 33581); break;
        case 23: SummonTempNPC(player, 33580); break;
        case 24: SummonTempNPC(player, 33586); break;
        case 25: SummonTempNPC(player, 33633); break;
        case 26: SummonTempNPC(player, 33590); break;
        case 27: SummonTempNPC(player, 28704); break;
        case 28: SummonTempNPC(player, 28696); break;
        case 29: SummonTempNPC(player, 28698); break;
        case 30: SummonTempNPC(player, 33587); break;
        case 31: SummonTempNPC(player, 28706); break;
        case 32: SummonTempNPC(player, 33603); break;

        // VIP Title
        case 9500:         
        {
            CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(178);
            player->SetTitle(titleInfo);
            CloseGossipMenuFor(player);
        }
            break;
        // Remove Resurrection Sickness
        case 1209:
            if (!player->HasAura(15007))
                ChatHandler(player->GetSession()).PSendSysMessage("Слабость отсутствует.");
            else
            {
                player->RemoveAurasDueToSpell(15007);
                player->SetHealth(player->GetMaxHealth());
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
                ChatHandler(player->GetSession()).PSendSysMessage("Слабость после воскрешения снята.");
            }
            break;

        // Remove Deserter
        case 1210:
            if (!player->HasAura(26013))
                ChatHandler(player->GetSession()).PSendSysMessage("Аура Дезертира отсутствует.");
            else
            {
                player->RemoveAurasDueToSpell(26013);
                ChatHandler(player->GetSession()).PSendSysMessage("Дезертир снят.");
            }
            break;

        // Heal
        case 1202:
            if (player->HasAura(45523))
                ChatHandler(player->GetSession()).PSendSysMessage("Эффект восстановления ещё в кулдауне.");
            else
            {
                player->CastSpell(player, 25840, true);
                player->CastSpell(player, 45523, true);
                ChatHandler(player->GetSession()).PSendSysMessage("Вы полностью исцелены!");
                player->SaveToDB(false, false);
            }
            break;

        // Learn Weapons
        case 11:
            switch (player->getClass())
            {
            case CLASS_PALADIN: player->learnSpell(196); player->learnSpell(201); player->learnSpell(200); player->learnSpell(197); player->learnSpell(202); break;
            case CLASS_SHAMAN: player->learnSpell(15590); player->learnSpell(1180); player->learnSpell(196); break;
            case CLASS_WARRIOR: player->learnSpell(264); player->learnSpell(5011); player->learnSpell(1180); player->learnSpell(15590);
                                player->learnSpell(266); player->learnSpell(198); player->learnSpell(200); player->learnSpell(227);
                                player->learnSpell(2567); player->learnSpell(199); player->learnSpell(202); break;
            case CLASS_HUNTER: player->learnSpell(5011); player->learnSpell(1180); player->learnSpell(15590); player->learnSpell(266);
                                player->learnSpell(201); player->learnSpell(200); player->learnSpell(227); player->learnSpell(2567);
                                player->learnSpell(197); player->learnSpell(202); player->learnSpell(264); break;
            case CLASS_ROGUE: player->learnSpell(264); player->learnSpell(5011); player->learnSpell(15590); player->learnSpell(266);
                               player->learnSpell(198); player->learnSpell(201); break;
            case CLASS_DRUID: player->learnSpell(1180); player->learnSpell(15590); player->learnSpell(199); break;
            case CLASS_MAGE: player->learnSpell(201); player->learnSpell(1180); break;
            case CLASS_WARLOCK: player->learnSpell(201); player->learnSpell(227); break;
            case CLASS_PRIEST: player->learnSpell(1180); player->learnSpell(227); break;
            case CLASS_DEATH_KNIGHT: player->learnSpell(198); player->learnSpell(199); break;
            default: break;
            }
            ChatHandler(player->GetSession()).PSendSysMessage("Все навыки оружия изучены.");
            break;

        // Ground mounts items
        case 100: player->AddItem(21176,1); break;
        case 101: player->AddItem(33809,1); break;
        case 102: player->AddItem(38576,1); break;
        case 103: player->AddItem(13335,1); break;
        case 104: player->AddItem(30480,1); break;
        case 105: player->CastSpell(player,65646,true); break;
        case 106: player->AddItem(37828,1); break;
        case 107: player->CastSpell(player,23509,true); break;
        case 108: player->AddItem(46778,1); break;
        case 109: player->AddItem(43962,1); break;
        case 110: player->AddItem(43963,1); break;
        case 111: player->AddItem(52200,1); break;
        case 112: player->AddItem(32768,1); break;
        case 113: player->AddItem(33225,1); break;
        case 114: player->AddItem(46109,1); break;
        case 115: player->AddItem(19872,1); break;
        case 116: player->AddItem(35513,1); break;
        case 117: player->AddItem(37719,1); break;
        case 118: player->AddItem(19902,1); break;
        case 119: player->AddItem(37012,1); break;
        case 120: player->AddItem(54068,1); break;
        case 121:
            player->AddItem(21218,1);
            player->AddItem(21323,1);
            player->AddItem(21321,1);
            player->AddItem(21324,1);
            break;

        // Elite mounts items
        case 122: player->AddItem(45725,1); break;
        case 123: player->AddItem(32458,1); break;
        case 125: player->AddItem(54069,1); break;
        case 126: player->AddItem(30609,1); break;
        case 127: player->AddItem(37676,1); break;
        case 128: player->AddItem(34092,1); break;
        case 129: player->AddItem(43516,1); break;
        case 130: player->AddItem(54811,1); break;
        case 131: player->AddItem(46708,1); break;
        case 132: player->AddItem(46171,1); break;
        case 133: player->AddItem(47840,1); break;
        case 134: player->AddItem(50435,1); break;
        case 135: player->AddItem(50818,1); break;
        case 136: player->AddItem(45693,1); break;
        case 137: player->AddItem(44178,1); break;
        case 138: player->AddItem(43952,1); break;
        case 139: player->AddItem(43986,1); break;
        case 140: player->AddItem(43953,1); break;
        case 141: player->AddItem(43951,1); break;
        case 142: player->AddItem(49636,1); break;
        case 152: player->AddItem(43954,1); break;
        case 143: player->AddItem(35226,1); break;
        case 144: player->AddItem(54860,1); break;
        
        // Proto drake mounts
        case 160: player->AddItem(44164,1); break;
        case 161: player->AddItem(44151,1); break;
        case 162: player->AddItem(44707,1); break;
        case 163: player->AddItem(45801,1); break;
        case 164: player->AddItem(44175,1); break;
        case 165: player->AddItem(44160,1); break;
        case 166: player->AddItem(45802,1); break;
        case 167: player->AddItem(44168,1); break;
        case 168: player->AddItem(44177,1); break;

        // ViP Wings items
        case 7003: if (!player->HasItemCount(68912,1)) player->AddItem(68912,1); else ChatHandler(player->GetSession()).PSendSysMessage("Уже есть."); break;
        case 7004: if (!player->HasItemCount(68913,1)) player->AddItem(68913,1); else ChatHandler(player->GetSession()).PSendSysMessage("Уже есть."); break;
        case 7005: if (!player->HasItemCount(68914,1)) player->AddItem(68914,1); else ChatHandler(player->GetSession()).PSendSysMessage("Уже есть."); break;

        // ViP Diadem
        case 1231: // ViP-Диадема — раздаём сразу по классу
        {
            uint32 itemId = 0;
            switch (player->getClass())
            {
            case CLASS_WARRIOR:     itemId = 80052; break; // Воин
            case CLASS_PALADIN:     itemId = 80053; break; // Паладин
            case CLASS_HUNTER:      itemId = 80055; break; // Охотник
            case CLASS_ROGUE:       itemId = 80058; break; // Разбойник
            case CLASS_PRIEST:      itemId = 80059; break; // Жрец
            case CLASS_SHAMAN:      itemId = 80057; break; // Шаман
            case CLASS_MAGE:        itemId = 80060; break; // Маг
            case CLASS_WARLOCK:     itemId = 80061; break; // Чернокнижник
            case CLASS_DRUID:       itemId = 80056; break; // Друид (две вещи)
            { player->AddItem(80062, 1); }
            break;
            case CLASS_DEATH_KNIGHT:itemId = 80054; break; // Рыцарь Смерти
            default: break;
            }

            if (!itemId)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Вашему классу нет ViP-Диадемы.");
            }
            else if (player->HasItemCount(itemId, 1))
            {
                ChatHandler(player->GetSession()).PSendSysMessage(EMOTE_ALREADY_ITEM);
            }
            else
            {
                player->AddItem(itemId, 1);
                ChatHandler(player->GetSession()).PSendSysMessage("ViP-Диадема выдана.");
            }

            // вернуться в главное меню
            vip_command::OpenVipMenu(player, 0);
            return;
        }

        // Buff spells
        case 4000: player->CastSpell(player,48162,true); break;
        case 4001: player->CastSpell(player,48074,true); break;
        case 4002: player->CastSpell(player,48170,true); break;
        case 4003: player->CastSpell(player,43223,true); break;
        case 4004: player->CastSpell(player,48934,true); break;
        case 4005: player->CastSpell(player,48938,true); break;
        case 4006: player->CastSpell(player,25899,true); break;
        case 4007: player->CastSpell(player,36880,true); break;
        case 4008: player->CastSpell(player,43015,true); break;
        case 4009: player->CastSpell(player,43017,true); break;
        case 4010: player->CastSpell(player,69381,true); break;
        case 4011: player->CastSpell(player,467,true); break;
        case 4012: player->CastSpell(player,20217,true); break;

        // Change appearance
        case 2001:
            if (player->GetArenaPoints() < CONST_ARENA_RENAME)
                ChatHandler(player->GetSession()).PSendSysMessage(EMOTE_NO_ARENA_POINTS);
            else
            {
                player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                player->ModifyArenaPoints(-CONST_ARENA_RENAME);
                ChatHandler(player->GetSession()).PSendSysMessage(MSG_RENAME_COMPLETE);
            }
            break;

        case 2002:
            if (player->GetArenaPoints() < CONST_ARENA_CHANGE_FACTION)
                ChatHandler(player->GetSession()).PSendSysMessage(EMOTE_NO_ARENA_POINTS);
            else
            {
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                player->ModifyArenaPoints(-CONST_ARENA_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage(MSG_CHANGE_FACTION_COMPLETE);
            }
            break;

        // Reset talents
        case 1207:
            player->resetTalents(true);
            player->SendTalentsInfoData(false);
            ChatHandler(player->GetSession()).PSendSysMessage(EMOTE_TALENT_RESET);
            break;

        // Reset quests cooldowns
        case 1215:
            player->ResetDailyQuestStatus();
            player->ResetWeeklyQuestStatus();
            ChatHandler(player->GetSession()).PSendSysMessage(MSG_RESET_QUEST_STATUS_COMPLETE);
            break;

        // Max skills
        case 1204:
            player->UpdateSkillsToMaxSkillsForLevel();
            ChatHandler(player->GetSession()).PSendSysMessage(MSG_MAX_SKILL);
            break;

        default:
            break;
        }

        CloseGossipMenuFor(player);
    }
};

void AddSC_vip_menu()
{
    new vip_command();
    new vip_menu();
}
