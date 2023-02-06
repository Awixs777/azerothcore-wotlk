class New_Character : public PlayerScript
{
public:
	New_Character() : PlayerScript("New_Character") {}

	void OnLogin(Player* player)
	{
        
		if (player->GetTotalPlayedTime() < 1)
		{          
				switch (player->getClass())
				{                
					/*
					// Skill Reference
					player->learnSpell(204);	// Defense
					player->learnSpell(264);	// Bows
					player->learnSpell(5011);	// Crossbow
					player->learnSpell(674);	// Dual Wield
					player->learnSpell(15590);	// Fists
					player->learnSpell(266);	// Guns
					player->learnSpell(196);	// Axes
					player->learnSpell(198);	// Maces
					player->learnSpell(201);	// Swords
					player->learnSpell(750);	// Plate Mail
					player->learnSpell(200);	// PoleArms
					player->learnSpell(9116);	// Shields
					player->learnSpell(197);	// 2H Axe
					player->learnSpell(199);	// 2H Mace
					player->learnSpell(202);	// 2H Sword
					player->learnSpell(227);	// Staves
					player->learnSpell(2567);	// Thrown
					*/

				case CLASS_PALADIN:
					player->learnSpell(196);	// Axes
					player->learnSpell(750);	// Plate Mail
					player->learnSpell(200);	// PoleArms
					player->learnSpell(197);	// 2H Axe
					player->learnSpell(199);	// 2H Mace
                    player->SetSkill(762, 4, 300, 300);
					break;


				case CLASS_SHAMAN:
					player->learnSpell(15590);	// Fists
					player->learnSpell(8737);	// Mail
					player->learnSpell(196);	// Axes
					player->learnSpell(197);	// 2H Axe
					player->learnSpell(199);	// 2H Mace
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_WARRIOR:
					player->learnSpell(264);	// Bows
					player->learnSpell(5011);	// Crossbow
					player->learnSpell(674);	// Dual Wield
					player->learnSpell(15590);	// Fists
					player->learnSpell(266);	// Guns
					player->learnSpell(750);	// Plate Mail
					player->learnSpell(200);	// PoleArms
					player->learnSpell(199);	// 2H Mace
					player->learnSpell(227);	// Staves
                    player->learnSpell(2567);
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_HUNTER:
					player->learnSpell(674);	// Dual Wield
					player->learnSpell(15590);	// Fists
					player->learnSpell(266);	// Guns
					player->learnSpell(8737);	// Mail
					player->learnSpell(200);	// PoleArms
					player->learnSpell(227);	// Staves
					player->learnSpell(202);	// 2H Sword
                    player->learnSpell(2567);
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_ROGUE:
					player->learnSpell(264);	// Bows
					player->learnSpell(5011);	// Crossbow
					player->learnSpell(15590);	// Fists
					player->learnSpell(266);	// Guns
					player->learnSpell(196);	// Axes
					player->learnSpell(198);	// Maces
					player->learnSpell(201);	// Swords
                    player->learnSpell(2567);
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_DRUID:
					player->learnSpell(1180);	// Daggers
					player->learnSpell(15590);	// Fists
					player->learnSpell(198);	// Maces
					player->learnSpell(200);	// PoleArms
					player->learnSpell(227);	// Staves
					player->learnSpell(199);	// 2H Mace
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_MAGE:
					player->learnSpell(201);	// Swords
					player->learnSpell(1180);	// Daggers
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_WARLOCK:
					player->learnSpell(201);	// Swords
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_PRIEST:
					player->learnSpell(1180);	// Daggers
                    player->SetSkill(762, 4, 300, 300);
					break;

				case CLASS_DEATH_KNIGHT:
					player->learnSpell(198);	// Maces
					player->learnSpell(199);
                    player->SetSkill(762, 4, 300, 300);
					break;

				default:
					break;
				}

				if (player->getClass() == CLASS_PALADIN || player->getClass() == CLASS_WARRIOR || player->getClass() == CLASS_DEATH_KNIGHT)
				{
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_NECK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BODY, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TABARD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET, true);
                    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 61071, true); // Грудбь ДД
                    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 61073, true); // Ноги ДД
                    player->EquipNewItem(EQUIPMENT_SLOT_FEET, 61075, true); // Ботинки ДД
                    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 61072, true); // Руки ДД
                    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 61069, true); // Голова ДД
					player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 100403, true); // Плечи ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 60016, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 61128, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 61134, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 61134, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_NECK, 61131, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 61074, true); // Пояс  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BACK, 61095, true); // Плащ ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TABARD, 61178, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BODY, 61166, true); // Рубашка  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 61159, true); // Пуха
					player->EquipNewItem(INVENTORY_SLOT_BAG_1, 61137, true); // Сумка 1
					player->EquipNewItem(INVENTORY_SLOT_BAG_2, 61137, true); // Сумка 2
					player->EquipNewItem(INVENTORY_SLOT_BAG_3, 61137, true); // Сумка 3
					player->EquipNewItem(INVENTORY_SLOT_BAG_4, 61137, true); // Сумка 4
					player->AddItem(21215, 10); // Кекс
					player->ResurrectPlayer(player->GetSession()->GetSecurity() ? 1.0f : 1.0f);
					player->SpawnCorpseBones();
					player->SaveToDB(false, false);
				//	player->AddItem(60015, 1); // Кольцо НОВИЧКАМ 
				// Оповещение в чате
				std::ostringstream ss;
				ss << "|cffFF0000[Приятной игры]|cffFF8000Вы получили бонус:|cff02A4B1 ViP-Аккаунт [3] дня|r";
				ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
				}

				if (player->getClass() == CLASS_HUNTER || player->getClass() == CLASS_ROGUE || player->getClass() == CLASS_SHAMAN || player->getClass() == CLASS_DRUID)
				{
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_NECK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BODY, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TABARD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET, true);
					player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 61078, true); // Грудбь ДД
					player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 61081, true); // Ноги ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FEET, 61082, true); // Ботинки ДД
					player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 61079, true); // Руки ДД
					player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 61076, true); // Голова ДД
					player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 100404, true); // Плечи ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 60016, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 61129, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 61135, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 61135, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_NECK, 61132, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 61080, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BACK, 61141, true); // Плащ ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TABARD, 61188, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BODY, 61175, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 61163, true); // Пуха
					player->EquipNewItem(INVENTORY_SLOT_BAG_1, 61138, true); // Сумка 1
					player->EquipNewItem(INVENTORY_SLOT_BAG_2, 61138, true); // Сумка 2
					player->EquipNewItem(INVENTORY_SLOT_BAG_3, 61138, true); // Сумка 3
					player->EquipNewItem(INVENTORY_SLOT_BAG_4, 61138, true); // Сумка 4
					player->AddItem(21215, 10); // Кекс
				//	player->AddItem(60015, 1); // Кольцо НОВИЧКАМ 
					// Оповещение в чате
					std::ostringstream ss;
					ss << "|cffFF0000[Приятной игры]|cffFF8000Вы получили бонус:|cff02A4B1 ViP-Аккаунт [3] дня|r";
					ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
				}

				if (player->getClass() == CLASS_MAGE || player->getClass() == CLASS_PRIEST || player->getClass() == CLASS_WARLOCK)
				{
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_NECK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BODY, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET1, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TRINKET2, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_TABARD, true);
					player->DestroyItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET, true);
					player->EquipNewItem(EQUIPMENT_SLOT_CHEST, 61064, true); // Грудбь ДД
					player->EquipNewItem(EQUIPMENT_SLOT_LEGS, 61066, true); // Ноги ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FEET, 61068, true); // Ботинки ДД
					player->EquipNewItem(EQUIPMENT_SLOT_HANDS, 61065, true); // Руки ДД
					player->EquipNewItem(EQUIPMENT_SLOT_HEAD, 61062, true); // Голова ДД
					player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, 100405, true); // Плечи ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, 60016, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, 61130, true); // Колцьцо ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, 61136, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, 61136, true); // Триня ДД
					player->EquipNewItem(EQUIPMENT_SLOT_NECK, 61133, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_WAIST, 61067, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BACK, 61142, true); // Плащ ДД
					player->EquipNewItem(EQUIPMENT_SLOT_TABARD, 61189, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_BODY, 61177, true); // Шея  ДД
					player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, 61160, true); // Пуха
					player->EquipNewItem(INVENTORY_SLOT_BAG_1, 61139, true); // Сумка 1
					player->EquipNewItem(INVENTORY_SLOT_BAG_2, 61139, true); // Сумка 2
					player->EquipNewItem(INVENTORY_SLOT_BAG_3, 61139, true); // Сумка 3
					player->EquipNewItem(INVENTORY_SLOT_BAG_4, 61139, true); // Сумка 4
					player->AddItem(21215, 10); // Кекс
				//	player->AddItem(60015, 1); // Кольцо НОВИЧКАМ 
					// Оповещение в чате
					std::ostringstream ss;
					ss << "|cffFF0000[Приятной игры]|cffFF8000Вы получили бонус:|cff02A4B1 ViP-Аккаунт [3] дня|r";
					ChatHandler(player->GetSession()).SendSysMessage(ss.str().c_str());
				}
		}
	}
};

	void AddSC_New_Character()
	{
		new New_Character();
	}
