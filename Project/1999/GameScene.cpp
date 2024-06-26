#define _CRT_SECURE_NO_WARNINGS

#include "GameScene.h"

GameScene::GameScene()
{
	srand((unsigned int)time(NULL));

	WhichMusic = EMUSIC::INFINI;
}

GameScene::~GameScene()
{
	delete build;
	delete menu;
	delete to;
	delete info;
	delete news;
	delete ending;
	delete music;
}

void GameScene::PlayInGame()
{
	// Initialize
	build->BuildingIntializer();
	info->GameInfoInitializer();
	news->NewsInitializer();
	WhichMusic = EMUSIC::INFINI;

	// Map Setting
	PlayGetName();
	to->CleanInputBuffer();
	build->PrintWholeMap();

	// Play Game
	music->PlayMusicInfini();
	info->PrintInfo();
	menu->PrintNewsImage();
	news->ShowNewNews();

	int page = 0;
	int DefaultMenuFlag = 0;

	int ZombieDayCnt = 0;
	int DayCount = 1;
	int EndingFlag = -1;
	while (1)
	{
		int ZombieDay = rand() % (info->GetArmyPower()+1);
		if ((ZombieDay < 10 || (DayCount % 10 == 0)) && DayCount > 5 && EndingFlag == -1)
		{
			ZombieDayCnt++;

			// info update
			to->PartClean(MENU_CLEAN_X, MENU_CLEAN_Y, MENU_CLEAN_BX, MENU_CLEAN_BY);
			to->PartClean(INFO_POSITION_X + 45, INFO_POSITION_Y + 1, 96, 8);
			info->PrintInfo();

			news->MakeQueueEmpty();
			
			menu->PrintZombieMenu();

			music->PlayZombieSiren();
			Sleep(1000);

			build->ZombieDayRandomDestory(info, news);
			ZombieAttack();
			news->ShowZombieNews();

			to->PartClean(INFO_POSITION_X + 45, INFO_POSITION_Y + 1, 96, 8);
			info->PrintInfo();

			Sleep(1000);

			// Day Update
			DayCount++;
			DateUpdate();
			EndingFlag = AllDieEndingCheck();

			to->PartClean(INFO_POSITION_X + 45, INFO_POSITION_Y + 1, 96, 8);
			to->PartClean(NEWS_POSITION_X + 2, NEWS_POSITION_Y + 4, 40, 15);
			news->ShowNewNews();
			info->PrintInfo();
			
			MusicCheck(1);
			to->CleanInputBuffer();
		}
		else
		{
			while (EndingFlag == -1)
			{
				to->PartClean(MENU_CLEAN_X, MENU_CLEAN_Y, MENU_CLEAN_BX, MENU_CLEAN_BY);
				int MenuSelect = menu->ShowDefaultMenu(MENU_POINT_POSITION_X, MENU_POINT_POSITION_Y, DEFAULT_MENU_BETWEEN);

				if (MenuSelect == EMENU_SELECT::CONSTRUCT)
				{
					int page = 0;
					int DefaultMenuFlag = 0;

					while (1)
					{
						if (page == 0)
						{
							to->PartClean(MENU_CLEAN_X, MENU_CLEAN_Y, MENU_CLEAN_BX, MENU_CLEAN_BY);
							menu->ShowConstructMenu1();

							while (1)
							{
								if (_kbhit())
								{
									int nKey = _getch();

									if (nKey == EKEYBOARD::KOREAN)
									{
										nKey = _getch();
										if (nKey == 189) { nKey = EKEYBOARD::V_KEY; }
										else if (nKey == 186) { nKey = EKEYBOARD::C_KEY; }
										else if (nKey == 209) { nKey = EKEYBOARD::M_KEY; }
										else if (nKey == 196) { nKey = EKEYBOARD::P_KEY; }
										else if (nKey == 187) { nKey = EKEYBOARD::Z_KEY; }
									}
									if (nKey == EKEYBOARD::Z_KEY)
									{
										music->PlayMoveBeep();
										to->CleanInputBuffer();
										page = 1;
										break;
									}
									else if (nKey == EKEYBOARD::M_KEY)
									{
										music->PlayBigClick();
										to->CleanInputBuffer();
										page = 0;
										DefaultMenuFlag = 1;
										break;
									}
									else if (nKey >= EKEYBOARD::NUM1_KEY && nKey <= EKEYBOARD::NUM6_KEY)
									{
										music->PlayMoveBeep();
										int ConstructInput = build->BuildBuilding(nKey, info);
										if (ConstructInput == EKEYBOARD::M_KEY)
										{
											to->CleanInputBuffer();
											page = 0;
											DefaultMenuFlag = 1;
											break;
										}
										else if (ConstructInput == EKEYBOARD::Z_KEY)
										{
											music->PlayMoveBeep();
											to->CleanInputBuffer();
											page = 1;
											break;
										}
									}
									to->CleanInputBuffer();
								}
							}
						}
						else if (page == 1)
						{
							to->PartClean(MENU_CLEAN_X, MENU_CLEAN_Y, MENU_CLEAN_BX, MENU_CLEAN_BY);
							menu->ShowConstructMenu2();

							while (1)
							{
								if (_kbhit())
								{
									int nKey = _getch();
									if (nKey == EKEYBOARD::Z_KEY)
									{
										music->PlayMoveBeep();
										to->CleanInputBuffer();
										page = 0;
										break;
									}
									else if (nKey == EKEYBOARD::M_KEY)
									{
										music->PlayBigClick();
										to->CleanInputBuffer();
										page = 0;
										DefaultMenuFlag = 1;
										break;
									}
									else if (nKey >= EKEYBOARD::NUM7_KEY && nKey <= EKEYBOARD::NUM8_KEY)
									{
										music->PlayMoveBeep();
										int ConstructInput = build->BuildBuilding(nKey, info);
										if (ConstructInput == EKEYBOARD::M_KEY)
										{
											music->PlayBigClick();
											to->CleanInputBuffer();
											page = 0;
											DefaultMenuFlag = 1;
											break;
										}
										else if (ConstructInput == EKEYBOARD::Z_KEY)
										{
											music->PlayMoveBeep();
											to->CleanInputBuffer();
											page = 0;
											break;
										}
									}
									to->CleanInputBuffer();
								}
							}
						}
						if (DefaultMenuFlag == 1)
						{
							DefaultMenuFlag = 0;
							break;
						}
					}
				}
				else if (MenuSelect == EMENU_SELECT::CITIZEN)
				{
					menu->ShowCitizenMenu();
					while (1)
					{
						if (_kbhit())
						{
							int nKey = _getch();
							if ((nKey == EKEYBOARD::NUM1_KEY || nKey == EKEYBOARD::NUM2_KEY) && info->GetCitizenPower() > 0) // 세금 1
							{
								music->PlayMoveBeep();
								to->CleanInputBuffer();
								if (nKey == EKEYBOARD::NUM1_KEY)
								{
									info->ForceMoney(EKEYBOARD::NUM1_KEY);
									info->SetCntArr(ECOUNT::CITIZEN_L_FORCE_CNT, info->GetCntArr()[ECOUNT::CITIZEN_L_FORCE_CNT] + 1);
									news->PushNewsQueue(ENEWS_CATEGORY::CitizenLittleForceNews);
								}
								else if (nKey == EKEYBOARD::NUM2_KEY)
								{
									info->ForceMoney(EKEYBOARD::NUM2_KEY);
									info->SetCntArr(ECOUNT::CITIZEN_M_FORCE_CNT, info->GetCntArr()[ECOUNT::CITIZEN_M_FORCE_CNT] + 1);
									news->PushNewsQueue(ENEWS_CATEGORY::CitizenManyForceNews); 
								}

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::NUM3_KEY && info->GetCitizenPower() > 0) // 복지정책
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::CITIZEN_WALFARE_CNT, info->GetCntArr()[ECOUNT::CITIZEN_WALFARE_CNT] + 1);
								to->CleanInputBuffer();
								info->Welfare();
								news->PushNewsQueue(ENEWS_CATEGORY::CitizenWelfareNews);

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::M_KEY) // 메뉴로
							{
								music->PlayBigClick();
								to->CleanInputBuffer();
								break;
							}
							else
							{
								// 금지 소리 출력
								music->PlayProhibitSound();
								to->CleanInputBuffer();
								continue;
							}

							to->CleanInputBuffer();
						}
					}
				}
				else if (MenuSelect == EMENU_SELECT::ARMY)
				{
					menu->ShowArmyMenu();
					while (1)
					{
						if (_kbhit())
						{
							int nKey = _getch();
							if (nKey == EKEYBOARD::NUM1_KEY && info->GetArmyPower() > 0) // 군인 월급 감소
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::ARMY_FORCE_CNT, info->GetCntArr()[ECOUNT::ARMY_FORCE_CNT] + 1);
								to->CleanInputBuffer();
								info->ForceArmyMoney();
								news->PushNewsQueue(ENEWS_CATEGORY::ArmyForceNews);

								info->PrintInfo();
								break;
							}
							if (nKey == EKEYBOARD::NUM2_KEY && info->GetArmyPower() > 0) // 프로파간다
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::ARMY_PROPAGANDA_CNT, info->GetCntArr()[ECOUNT::ARMY_PROPAGANDA_CNT] + 1);
								to->CleanInputBuffer();
								int Result = info->Propaganda();
								if (Result == 1) // Bad
								{
									news->PushNewsQueue(ENEWS_CATEGORY::ArmyPropagandaBadNews);
								}
								else if (Result == 0) // Good
								{
									news->PushNewsQueue(ENEWS_CATEGORY::ArmyPropagandaGoodNews);
								}

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::NUM3_KEY && info->GetArmyPower() > 0) // 국방부 예산 확대
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::ARMY_WELFARE_CNT, info->GetCntArr()[ECOUNT::ARMY_WELFARE_CNT] + 1);
								to->CleanInputBuffer();
								info->ArmyWelfare();
								news->PushNewsQueue(ENEWS_CATEGORY::ArmyWelfareNews);

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::M_KEY) // 메뉴로
							{
								music->PlayBigClick();
								to->CleanInputBuffer();
								break;
							}
							else
							{
								music->PlayProhibitSound();
								to->CleanInputBuffer();
								continue;
							}

							to->CleanInputBuffer();
						}
					}
				}
				else if (MenuSelect == EMENU_SELECT::RELIGION)
				{
					menu->ShowReligionMenu();
					while (1)
					{
						if (_kbhit())
						{
							int nKey = _getch();
							if (nKey == EKEYBOARD::NUM1_KEY && info->GetReligionPower() > 0) // 국민 종교활동 금지
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::PROHIBIT_R_C_CNT, info->GetCntArr()[ECOUNT::PROHIBIT_R_C_CNT] + 1);
								to->CleanInputBuffer();
								info->ProhibitReligionForCitizen();
								news->PushNewsQueue(ENEWS_CATEGORY::ReligionProhibitCitizenNews);

								info->PrintInfo();
								break;
							}
							if (nKey == EKEYBOARD::NUM2_KEY && info->GetReligionPower() > 0) // 군인 종교활동 금지
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::PROHIBIT_R_A_CNT, info->GetCntArr()[ECOUNT::PROHIBIT_R_A_CNT] + 1);
								to->CleanInputBuffer();
								info->ProhibitReligionForArmy();
								news->PushNewsQueue(ENEWS_CATEGORY::ReligionProhibitArmyNews);

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::NUM3_KEY && info->GetReligionPower() > 0) // 종교활동 참여
							{
								music->PlayMoveBeep();
								info->SetCntArr(ECOUNT::RELIGION_WELFARE_CNT, info->GetCntArr()[ECOUNT::RELIGION_WELFARE_CNT] + 1);
								to->CleanInputBuffer();
								info->ParticiateReligion();
								news->PushNewsQueue(ENEWS_CATEGORY::ParticiateReligionNews);

								info->PrintInfo();
								break;
							}
							else if (nKey == EKEYBOARD::M_KEY) // 메뉴로
							{
								music->PlayBigClick();
								to->CleanInputBuffer();
								break;
							}
							else
							{
								music->PlayProhibitSound();
								to->CleanInputBuffer();
								continue;
							}

							to->CleanInputBuffer();
						}
					}
				}
				else if (MenuSelect == EKEYBOARD::P_KEY)
				{
					music->PlayFinish();
					to->CleanInputBuffer();

					DateUpdate();
					EndingFlag = EndingCheck();
					EndOfTheDay();
					MusicCheck(0);
					DayCount++;

					to->PartClean(INFO_POSITION_X + 45, INFO_POSITION_Y + 1, 96, 8);
					news->ShowNewNews();
					info->PrintInfo();

					break;
				}
			}
			
		}

		if (EndingFlag != -1)
		{
			break;
		}

	}

	info->SetCntArr(ECOUNT::ZOMBIE_ATTACK_CNT, ZombieDayCnt);
	info->SetCntArr(ECOUNT::DAYCOUNT, DayCount);
	
	// Ending 작성
	if (EndingFlag == ESELECT_SCENE::CITIZEN_ENDING)
	{
		 // Citizen Ending
		ending->CitizenEnding(info);

	}
	else if (EndingFlag == ESELECT_SCENE::ARMY_ENDING)
	{
		 // Army Ending
		ending->ArmyEnding(info);
	}
	else if (EndingFlag == ESELECT_SCENE::RELIGION_ENDING)
	{
		 // Religion Ending
		ending->ReligionEnding(info);
	}
	else if (EndingFlag == ESELECT_SCENE::ALLDIE_ENDING)
	{
		 // AllDie Ending
		ending->AllDieEnding(info);
	}
}

void GameScene::PlayGetName()
{
	music->PlayMusicXenial();
	system("cls");
	std::string Name;
	std::string CountryName;

	std::string sName;
	std::string sCountryName;

	to->SetColor(10);
	to->GoToXYPosition(PREVIEW_TEXT_POSITION_X, PREVIEW_TEXT_POSITION_Y);
	printf("당신의 이름은?(최대 10글자): ");
	to->GoToXYPosition(PREVIEW_TEXT_POSITION_X + 30, PREVIEW_TEXT_POSITION_Y);
	std::cin >> sName;
	music->PlayMoveBeep();
	if (sName.size() > 20)
	{
		while (sName.size() > 20)
		{
			sName.pop_back();
		}
		info->SetPresidentName(sName);
	}
	else
	{
		info->SetPresidentName(sName);
	}

	to->GoToXYPosition(PREVIEW_TEXT_POSITION_X, PREVIEW_TEXT_POSITION_Y + 3);
	printf("당신의 나라의 이름은?(최대 10글자): ");
	to->GoToXYPosition(PREVIEW_TEXT_POSITION_X + 36, PREVIEW_TEXT_POSITION_Y + 3);
	std::cin >> sCountryName;
	music->PlayMoveBeep();
	if (sCountryName.size() > 20)
	{
		while (sCountryName.size() > 20)
		{
			sCountryName.pop_back();
		}
		info->SetCountryName(sCountryName);
	}
	else
	{
		info->SetCountryName(sCountryName);
	}
}

void GameScene::DateUpdate()
{
	int Year = info->GetYear();
	int Month = info->GetMonth();
	int Day = info->GetDay();

	if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12)
	{
		if (Day == 31)
		{
			info->SetMonth(++Month);
			info->SetDay(1);
		}
		else
		{
			Day++;
			info->SetDay(Day);
		}
	}
	else if (Month == 4 || Month == 6 || Month == 9 || Month == 11)
	{
		if (Day == 30)
		{
			info->SetMonth(++Month);
			info->SetDay(1);
		}
		else
		{
			Day++;
			info->SetDay(Day);
		}
	}
	else if (Month == 2)
	{
		if (Day == 29 && Year % 4 == 0)
		{
			info->SetMonth(++Month);
			info->SetDay(1);
		}
		else if (Day == 28 && Year % 4 != 0)
		{
			info->SetMonth(++Month);
			info->SetDay(1);
		}
		else
		{
			Day++;
			info->SetDay(Day);
		}
	}

	if (Month > 12)
	{
		info->SetYear(++Year);
		info->SetMonth(1);
	}
}

int GameScene::EndingCheck()
{
	//Citizen Ending Checking
	if (info->GetCitizenPower() <= 10)
	{
		if (info->GetCitizenEnding() == 0)
		{
			news->PushNewsQueue(ENEWS_CATEGORY::CitizenEndingNews);
			info->SetCitizenEnding(1);
			return -1;
		}
		else if (info->GetCitizenEnding() == 1)
		{
			to->GoToXYPosition(0, 0);
			return ESELECT_SCENE::CITIZEN_ENDING; // Citizen Ending
		}
	}
	else
	{
		if (info->GetCitizenEnding() == 1)
		{
			info->SetCitizenEnding(0);
			return -1;
		}
	}
	// Army Ending Checking
	if (info->GetArmyPower() >= 50 && info->GetCitizenPower() <= 20)
	{
		if (info->GetArmyEnding() == 0)
		{
			news->PushNewsQueue(ENEWS_CATEGORY::ArmyEndingNews);
			info->SetArmyEnding(1);
			return -1;
		}
		else if (info->GetArmyEnding() == 1)
		{
			return ESELECT_SCENE::ARMY_ENDING; // Army Ending
		}
	}
	else
	{
		if (info->GetArmyEnding() == 1)
		{
			info->SetArmyEnding(0);
			return -1;
		}
	}

	// Religion Ending Checking
	if (info->GetArmyPower() <= 20 && info->GetCitizenPower() <= 20)
	{
		if (info->GetReligionEnding() == 0)
		{
			news->PushNewsQueue(ENEWS_CATEGORY::ReligionEndingNews);
			info->SetReligionEnding(1);
			return -1;
		}
		else if (info->GetReligionEnding() == 1)
		{
			return ESELECT_SCENE::RELIGION_ENDING; // Religion Ending
		}
	}
	else
	{
		if (info->GetReligionEnding() == 1)
		{
			info->SetReligionEnding(0);
			return -1;
		}
	}
	// ALLDie Ending Checking
	if (info->GetArmyPower() <= 0 && info->GetCitizenPower() <= 0 && info->GetReligionPower() <= 0)
	{
		return ESELECT_SCENE::ALLDIE_ENDING; // ALLDie Ending
	}

	return -1;

}

int GameScene::AllDieEndingCheck()
{
	// ALLDie Ending Checking
	if (info->GetArmyPower() <= 0 && info->GetCitizenPower() <= 0 && info->GetReligionPower() <= 0)
	{
		return ESELECT_SCENE::ALLDIE_ENDING; // ALLDie Ending
	}

	return -1;
}

void GameScene::EndOfTheDay() // Info Update When The End of the Day
{

	// 민심이 절반보다 높을 때 전체 인구수 * 0.1 * 민심률 만큼 인구수 증가
	// 민심이 절반보다 낮을 때 전체 인구수 * 0.1 * 민심률 만큼 인구수 감소
	if (info->GetCitizenPower() < 50)
	{
		info->SetPeopleNum(info->GetPeopleNum() - (int)((double)info->GetPeopleNum() * 0.1));
	}
	else
	{
		info->SetPeopleNum(info->GetPeopleNum() + (int)((double)info->GetPeopleNum() * 0.1));
	}

	// 종교 세금 납부
	if (info->GetReligionPower() >= 50)
	{
		news->PushNewsQueue(ENEWS_CATEGORY::ReligionPay);
		info->SetMoney(info->GetMoney() + info->GetReligionPower() * 100);
	}
	else
	{
		news->PushNewsQueue(ENEWS_CATEGORY::ReligionNotPay);
	}

	// default 세금 납부
	info->SetMoney(info->GetMoney() + (int)((double)info->GetPeopleNum() * 0.05));

	// 사이비 메이커
	build->Religon42BMaker(info, news);
}

void GameScene::ZombieAttack()
{
	// 좀비 뉴스 push
	news->PushNewsQueue(ENEWS_CATEGORY::ZombieAppear);
	
	// 사망률 계산
	int DeathNum = (int)((double)info->GetPeopleNum() * 0.03); // default death
	DeathNum += (int)((double)info->GetPeopleNum() * (double)build->DistanceBetweenAPTExit() * 0.001); // Exit&APT

	int Save = 0;
	Save += build->ReligionCntSave(info, news);
	Save += build->CitizenCntSave(info, news);
	Save += build->ArmyCntSave(info, news);
	Save += (int)(MAX_DISTANCE - (double)info->GetPeopleNum() * (double)build->DistanceBetweenAPTReligion() * 0.0001); // APT&Religion

	int TotalDeath = DeathNum - Save;
	int FeelingSad = info->GetPeopleNum() / TotalDeath;

	to->GoToXYPosition(0, 0);
	to->SetColor(10);

	// 사망률에 대한 민심 군사력 종교권위 하락
	if (FeelingSad < 10)
	{
		info->SetCntArr(ECOUNT::SAFE_ZOMBIE_CNT, info->GetCntArr()[ECOUNT::SAFE_ZOMBIE_CNT] + 1);
		news->PushNewsQueue(ENEWS_CATEGORY::AfterZombieGood);
	}
	else
	{
		info->SetCntArr(ECOUNT::NONSAFE_ZOMBIE_CNT, info->GetCntArr()[ECOUNT::NONSAFE_ZOMBIE_CNT] + 1);
		news->PushNewsQueue(ENEWS_CATEGORY::AfterZombieBad);
		if (info->GetCitizenPower() - (FeelingSad * 2) < 0)
		{
			info->SetCitizenPower(0);
		}
		else
		{
			info->SetCitizenPower(info->GetCitizenPower() - (FeelingSad * 2));
		}

		if (info->GetArmyPower() - (FeelingSad * 2) < 0)
		{
			info->SetArmyPower(0);
		}
		else
		{
			info->SetArmyPower(info->GetArmyPower() - (FeelingSad * 2));
		}

		if (info->GetReligionPower() - (FeelingSad * 2) < 0)
		{
			info->SetReligionPower(0);
		}
		else
		{
			info->SetReligionPower(info->GetReligionPower() - (FeelingSad * 2));
		}
	}


	// 인구수 변화
	if (info->GetPeopleNum() + TotalDeath == 0)
	{
		info->SetPeopleNum(0);
	}
	else
	{
		info->SetPeopleNum(info->GetPeopleNum() + Save - DeathNum);
	}

}

void GameScene::MusicCheck(int zombie)
{
	// 노래재생 변경 - 민심, 군사력, 종교권위가 하나라도 50미만이면 노래변경
	if (info->GetCitizenPower() < 50 || info->GetArmyPower() < 50 || info->GetReligionPower() < 50)
	{
		if (WhichMusic == EMUSIC::INFINI || zombie == 1)
		{
			music->PlayMusicDaedulus();
			WhichMusic = EMUSIC::DAEDULUS;
		}
	}
	else
	{
		if (WhichMusic == EMUSIC::DAEDULUS || zombie == 1)
		{
			music->PlayMusicInfini();
			WhichMusic = EMUSIC::INFINI;
		}
	}
}


