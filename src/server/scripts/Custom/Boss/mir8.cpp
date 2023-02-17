/*
Coded by Lionzero, Respect my Code and do not remove my Copyright !
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"

enum Spells
{
    VISUAL_NETHER_PORTAL = 30490,
    SPELL_BEBENDE_ERDE = 6524,
    SPELL_BLADESTORM = 40904,
    SPELL_PRITAJ = 53406,
    SPELL_THUNDERING_STORM = 39365,
    SPELL_ENRAGE = 64487
};

enum Events
{
    EVENT_VISUAL_NETHER_PORTAL = 1,
    EVENT_BEBENDE_ERDE = 2,
    EVENT_BLADESTORM = 3,
    EVENT_SUMMONS = 4,
    EVENT_STORM = 5,
    EVENT_ENRAGE = 6
};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3
};

enum Summons
{
    NPC_FEUERTEUFEL = 22996,
    NPC_BLADE_OF_AZZINOTH = 22996,
    NPC_FLAME_OF_AZZINOTH = 22997
};


enum Models
{
    MODEL_BOY = 60124,
    MODEL_MONSTER = 60125
    //MODEL_MONSTER_2 = 55570
};
# define LM_YELL_AGGRO "Добро пожаловать в АД!"
# define LM_YELL_DANGER "Бегите глупцы, теперь вы узрите настоящий АД!!!"
#define SPELL_SARONITE               63364

class mir8 : public CreatureScript
{
public:
    mir8() : CreatureScript("mir8") { }

    struct mir8AI : public ScriptedAI
    {
        mir8AI(Creature* creature) : ScriptedAI(creature), Summons(me) { }

        void Reset() override
        {
            _events.Reset();
            Summons.DespawnAll();
            Summons.DespawnEntry(NPC_FLAME_OF_AZZINOTH);
            me->SetDisplayId(MODEL_BOY);
            me->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.3f);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.5f);
            me->Yell(LM_YELL_AGGRO, LANG_UNIVERSAL, 0);
            DoCast(me, VISUAL_NETHER_PORTAL);
            _events.SetPhase(PHASE_ONE);
            _events.ScheduleEvent(EVENT_VISUAL_NETHER_PORTAL, 1000);
            _events.ScheduleEvent(EVENT_BEBENDE_ERDE, 30000);
        }

        void DamageTaken(Unit*, uint32& damage, DamageEffectType, SpellSchoolMask)
        {
            if (me->HealthBelowPctDamaged(75, damage) && _events.IsInPhase(PHASE_ONE))
            {
                _events.SetPhase(PHASE_TWO);
                _events.ScheduleEvent(EVENT_BLADESTORM, 10000);
            }

            if (me->HealthBelowPctDamaged(40, damage) && _events.IsInPhase(PHASE_TWO))
            {
                me->Yell(LM_YELL_DANGER, LANG_UNIVERSAL, 0);
                me->SetDisplayId(MODEL_MONSTER /*&& MODEL_MONSTER_2*/);
                _events.SetPhase(PHASE_THREE);
                _events.ScheduleEvent(EVENT_SUMMONS, 5000);
                _events.ScheduleEvent(EVENT_BLADESTORM, 10000);
                _events.ScheduleEvent(EVENT_STORM, 10000);
                _events.ScheduleEvent(EVENT_ENRAGE, 120000); //In phase 3 the player have 2 minutes to defeat the Boss or all will die !
            }
        }

        void JustSummoned(Creature* summon) override
        {
            Summons.Summon(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit*)
        {
            Summons.Despawn(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
            Summons.DespawnAll();
        }

        void JustDied(Unit* /*killer*/) override
        {
            Summons.DespawnAll();
            Summons.DespawnEntry(NPC_FEUERTEUFEL);
            Summons.DespawnEntry(NPC_FLAME_OF_AZZINOTH);
        }

        void UpdateAI(uint32 diff) override
        {
            DoCast(me, SPELL_SARONITE);
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_VISUAL_NETHER_PORTAL:
                    DoCast(me, VISUAL_NETHER_PORTAL);
                    DoCast(me, SPELL_PRITAJ);
                    break;
                case EVENT_BEBENDE_ERDE:
                    DoCastVictim(SPELL_BEBENDE_ERDE);
                    _events.ScheduleEvent(EVENT_BEBENDE_ERDE, 8000);
                    break;
                case EVENT_SUMMONS:
                    me->SummonCreature(NPC_FLAME_OF_AZZINOTH, me->GetPositionX() + 10, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
                    me->SummonCreature(NPC_FLAME_OF_AZZINOTH, me->GetPositionX() - 10, me->GetPositionY(), me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
                    me->SummonCreature(NPC_FLAME_OF_AZZINOTH, me->GetPositionX(), me->GetPositionY() -10, me->GetPositionZ() + 5, 0, TEMPSUMMON_CORPSE_DESPAWN, 300000);
                    DoCast(me, SPELL_PRITAJ);
                    break;
                case EVENT_STORM:
                    me->CastSpell(me, SPELL_THUNDERING_STORM, true);
                    events.RepeatEvent(15000);
                    break;
                case EVENT_ENRAGE:
                    DoCast(me, SPELL_ENRAGE);
                    _events.ScheduleEvent(EVENT_ENRAGE, 10000);
                    break;
                default:
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
        SummonList Summons;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new mir8AI(creature);
    }
};

class spell_murmur_thundering_storm : public SpellScriptLoader
{
public:
    spell_murmur_thundering_storm() : SpellScriptLoader("spell_murmur_thundering_storm") { }

    class spell_murmur_thundering_storm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_murmur_thundering_storm_SpellScript);

        void SelectTarget(std::list<WorldObject*>& targets)
        {
            targets.remove_if(Acore::AllWorldObjectsInExactRange(GetCaster(), 100.0f, true));
            targets.remove_if(Acore::AllWorldObjectsInExactRange(GetCaster(), 25.0f, false));
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_murmur_thundering_storm_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_murmur_thundering_storm_SpellScript();
    }
};

void AddSC_mir8()
{
    new mir8();
}
