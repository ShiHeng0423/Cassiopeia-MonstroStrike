#include "MissionList.h"

//Construct mission
KillEnemyMission::KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail) : missionID {missionID},
	missionName{ name }, accepted{ false }, completed{false}, available(avail), slimeToKill {slimeTarg}, 
	chargerToKill{ chargerTarg }, flyToKill{ flyTarg } {}

//For pushing back into vector
void MissionSystem::CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly, bool avail)
{
	enemyMissions.push_back(KillEnemyMission(nextMissionID++, missionName, targetSlime, targetCharger, targetFly, avail));
	std::cout << "Mission name: " << missionName << " with the goal of killing " << targetSlime <<
		" amount of slimes, " << targetCharger << " amount of chargers, and " << targetFly << " of Flys have been added\n";

	if (avail)
	{
		std::cout << "Available now\n";
	}
	else
	{
		std::cout << "Not available\n";
	}
}

void MissionSystem::AcceptKillEnemyMission(int missionID)
{
	for (KillEnemyMission& mission : enemyMissions)
	{
		if (mission.missionID == missionID)
		{
			mission.accepted = true;
			std::cout << "Mission " << mission.missionName << " has been accepted\n";
			return;
		}
	}
	std::cout << "Mission not found...\n";
}

size_t MissionSystem::GetEnemyMissionsCount()
{
	return enemyMissions.size();
}

std::vector<int> MissionSystem::GetAvailableEnemyMissionsIDs()
{
	std::vector<int> availableIDs; 

	for (const KillEnemyMission& mission : enemyMissions)
	{
		if (mission.available)
			availableIDs.push_back(mission.missionID);
	}

	return availableIDs;
}