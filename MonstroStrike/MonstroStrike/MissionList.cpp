#include "MissionList.h"

//Construct mission
KillEnemyMission::KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail) : missionID {missionID},
	missionName{ name }, accepted{ false }, completed{false}, available(avail), slimeToKill {slimeTarg}, 
	chargerToKill{ chargerTarg }, flyToKill{ flyTarg } {}

void MissionSystem::InitialMission()
{
	CreateKillEnemyMission("Damn the pestering airborne pests!", 0, 0, 5, true);
	CreateKillEnemyMission("Slimy disaster", 5, 0, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true);

}

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
			mission.available = false;
			std::cout << "Mission " << mission.missionName << " has been accepted\n";
			return;
		}
	}
	std::cout << "Mission not found...\n";
}

void MissionSystem::MissionComplete(int missionID)
{
	for (KillEnemyMission& mission : enemyMissions)
	{
		if (mission.missionID == missionID)
		{
			mission.completed = true;
			
			std::cout << "Mission " << mission.missionName << " has been completed\n";

			//Here to add clear rewards
			switch (missionID)
			{
			case 0: //Need to know which mission ID is which mission
				//Add new mission / add rewards to give
				break;
			case 1:
				std::cout << mission.missionName << " cleared\n";
				CreateKillEnemyMission("Rampaging nightmare strike again", 0, 15, 0, true);
				break;
			case 2:
				break;
			}
			return;
		}
	}


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