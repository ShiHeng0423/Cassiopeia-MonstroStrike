#pragma once
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

	KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail, 
		const char* missionDetails);
};

struct MissionSystem {
	
	std::vector<KillEnemyMission> enemyMissions;
	
	int nextMissionID;
	
	int slimesKilled;
	
	int chargersKilled;
	
	int fliesKilled;

	void CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly, bool avail, const char* missionDetails); //List numbers of enemy
	
	void AcceptKillEnemyMission(int missionID);

	void MissionComplete(int missionID);

	size_t GetEnemyMissionsCount();

	std::vector<int>GetAvailableEnemyMissionsIDs();

	void InitialMission(); //Called only once through out entire game

	size_t GetAcceptedMissionID();

	void PrintMissionText();
};

extern struct MissionSystem missionSystem;
