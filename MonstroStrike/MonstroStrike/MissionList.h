#pragma once
#include "NonPlayableCharacters.h"

struct KillEnemyMission {
	const char* missionName;
	bool completed;
	bool accepted;
	bool available;

	int missionID;
	int slimeToKill;
	int chargerToKill;
	int flyToKill;

	KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail);
};

struct MissionSystem {
	
	std::vector<KillEnemyMission> enemyMissions;
	
	int nextMissionID;
	
	void CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly, bool avail); //List numbers of enemy
	
	void AcceptKillEnemyMission(int missionID);

	void MissionComplete(int missionID);

	size_t GetEnemyMissionsCount();

	std::vector<int>GetAvailableEnemyMissionsIDs();

	void InitialMission(); //Called only once through out entire game
};

