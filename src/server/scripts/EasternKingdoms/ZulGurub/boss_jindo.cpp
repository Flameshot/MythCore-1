/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2012 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You can't share Myth Project's sources! Only for personal use.
 */

#include "ScriptPCH.h"
#include "zulgurub.h"

#define SAY_AGGRO                       -1309014

#define SPELL_BRAINWASHTOTEM            24262
#define SPELL_POWERFULLHEALINGWARD      24309               //We will not use this spell. We will summon a totem by script cause the spell totems will not cast.
#define SPELL_HEX                       24053
#define SPELL_DELUSIONSOFJINDO          24306
#define SPELL_SHADEOFJINDO              24308               //We will not use this spell. We will summon a shade by script.

//Healing Ward Spell
#define SPELL_HEAL                      38588               //Totems are not working right. Right heal spell ID is 24311 but this spell is not casting...

//Shade of Jindo Spell
#define SPELL_SHADOWSHOCK               19460
#define SPELL_INVISIBLE                 24699

class boss_jindo : public CreatureScript
{
    public:
        boss_jindo() : CreatureScript("boss_jindo") { }

        struct boss_jindoAI : public ScriptedAI
        {
            boss_jindoAI(Creature* c) : ScriptedAI(c) { }

            uint32 BrainWashTotem_Timer;
            uint32 HealingWard_Timer;
            uint32 Hex_Timer;
            uint32 Delusions_Timer;
            uint32 Teleport_Timer;

            void Reset()
            {
                BrainWashTotem_Timer = 20000;
                HealingWard_Timer = 16000;
                Hex_Timer = 8000;
                Delusions_Timer = 10000;
                Teleport_Timer = 5000;
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if(!UpdateVictim())
                    return;

                //BrainWashTotem_Timer
                if(BrainWashTotem_Timer <= diff)
                {
                    DoCast(me, SPELL_BRAINWASHTOTEM);
                    BrainWashTotem_Timer = 18000 + rand()%8000;
                } else BrainWashTotem_Timer -= diff;

                //HealingWard_Timer
                if(HealingWard_Timer <= diff)
                {
                    //DoCast(me, SPELL_POWERFULLHEALINGWARD);
                    me->SummonCreature(14987, me->GetPositionX()+3, me->GetPositionY()-2, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30000);
                    HealingWard_Timer = 14000 + rand()%6000;
                } else HealingWard_Timer -= diff;

                //Hex_Timer
                if(Hex_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_HEX);

                    if(DoGetThreat(me->getVictim()))
                        DoModifyThreatPercent(me->getVictim(), -80);

                    Hex_Timer = 12000 + rand()%8000;
                } else Hex_Timer -= diff;

                //Casting the delusion curse with a shade. So shade will attack the same target with the curse.
                if(Delusions_Timer <= diff)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    {
                        DoCast(target, SPELL_DELUSIONSOFJINDO);

                        Creature* Shade = me->SummonCreature(14986, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Shade)
                            Shade->AI()->AttackStart(target);
                    }

                    Delusions_Timer = 4000 + rand()%8000;
                } else Delusions_Timer -= diff;

                //Teleporting a random gamer and spawning 9 skeletons that will attack this gamer
                if(Teleport_Timer <= diff)
                {
                    Unit* target = NULL;
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                    if(target && target->GetTypeId() == TYPEID_PLAYER)
                    {
                        DoTeleportPlayer(target, -11583.7783f, -1249.4278f, 77.5471f, 4.745f);

                        if(DoGetThreat(me->getVictim()))
                            DoModifyThreatPercent(target, -100);

                        Creature* Skeletons;
                        Skeletons = me->SummonCreature(14826, target->GetPositionX()+2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX()-2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX()+4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX()-4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                        Skeletons = me->SummonCreature(14826, target->GetPositionX()+3, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if(Skeletons)
                            Skeletons->AI()->AttackStart(target);
                    }

                    Teleport_Timer = 15000 + rand()%8000;
                } else Teleport_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jindoAI(creature);
        }
};

//Healing Ward
class mob_healing_ward : public CreatureScript
{
    public:
        mob_healing_ward() : CreatureScript("mob_healing_ward") { }

        struct mob_healing_wardAI : public ScriptedAI
        {
            mob_healing_wardAI(Creature* c) : ScriptedAI(c)
            {
                pInstance = c->GetInstanceScript();
            }

            uint32 Heal_Timer;

            InstanceScript* pInstance;

            void Reset()
            {
                Heal_Timer = 2000;
            }

            void EnterCombat(Unit* /*pWho*/) { }

            void UpdateAI (const uint32 diff)
            {
                //Heal_Timer
                if(Heal_Timer <= diff)
                {
                    if(pInstance)
                    {
                        Unit* pJindo = Unit::GetUnit((*me), pInstance->GetData64(DATA_JINDO));
                        if(pJindo)
                            DoCast(pJindo, SPELL_HEAL);
                    }
                    Heal_Timer = 3000;
                } else Heal_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_healing_wardAI(creature);
        }
};

//Shade of Jindo
class mob_shade_of_jindo : public CreatureScript
{
    public:
        mob_shade_of_jindo() : CreatureScript("mob_shade_of_jindo") { }

        struct mob_shade_of_jindoAI : public ScriptedAI
        {
            mob_shade_of_jindoAI(Creature* c) : ScriptedAI(c) { }

            uint32 ShadowShock_Timer;

            void Reset()
            {
                ShadowShock_Timer = 1000;
                DoCast(me, SPELL_INVISIBLE, true);
            }

            void EnterCombat(Unit* /*pWho*/){ }

            void UpdateAI (const uint32 diff)
            {
                //ShadowShock_Timer
                if(ShadowShock_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SHADOWSHOCK);
                    ShadowShock_Timer = 2000;
                } else ShadowShock_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shade_of_jindoAI(creature);
        }
};

void AddSC_boss_jindo()
{
    new boss_jindo;
    new mob_healing_ward;
    new mob_shade_of_jindo;
}

