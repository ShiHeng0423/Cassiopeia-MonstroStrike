#pragma once
/*!************************************************************************
  \file					MissionList.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (95%)
  \secondary author		Keith Ng Hon Yew (5%)
  \brief				File containing declarations of structures and functions for managing missions.

  This file contains declarations of structures and functions essential for managing missions. The header defines structures for 
  representing missions, as well as a mission system to handle mission creation, acceptance, completion, and rewards. Getter 
  functions to check for current mission, available missions, number of missions, mission clearable etc are also included. 
  Values that store the number of entity types killed or amount of rewards are also declared here.


All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/

#include "NonPlayableCharacters.h"

struct KillEnemyMission {
	const char* missionName;
	const char* missionDetails;

	bool completed;
	bool accepted;
	bool available;

	int missionID;
	int slimeToKill;
	int chargerToKill;
	int flyToKill;

	std::vector<std::string> rewardDetails;

	KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail, 
		const char* missionDetails, const std::vector<std::string>& rewards);
};

struct MissionSystem {
	
	std::vector<KillEnemyMission> enemyMissions;
	
	int nextMissionID;
	
	int slimesKilled;
	
	int chargersKilled;
	
	int fliesKilled;

	int reward1Amount;
	
	int reward2Amount;

	void CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly, bool avail, const char* missionDetails,
		const std::vector<std::string>& rewardDetails); //List numbers of enemy
	
	void AcceptKillEnemyMission(int missionID);

	void MissionComplete(int missionID);

	bool CheckMissionClear();

	size_t GetEnemyMissionsCount();

	std::vector<int>GetAvailableEnemyMissionsIDs();

	std::vector<std::string> GetRewardsForMission(int reward1ID, int amountForReward1, int reward2ID, int amountForReward2);

	int FindItemID(const std::string& itemName);

	void InitialMission(); //Called only once through out entire game

	int GetAcceptedMissionID();

	void PrintMissionText();

	void CleanMemory();
};

extern struct MissionSystem missionSystem;
