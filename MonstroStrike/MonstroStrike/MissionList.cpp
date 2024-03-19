#include "MissionList.h"
#include "main.h"

//Global System
struct MissionSystem missionSystem;

//Construct mission
KillEnemyMission::KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg, bool avail, const char* missionDetails) : missionID {missionID},
	missionName{ name }, accepted{ false }, completed{false}, available(avail), slimeToKill {slimeTarg}, 
	chargerToKill{ chargerTarg }, flyToKill{ flyTarg }, missionDetails{missionDetails} {}

void MissionSystem::InitialMission()
{
	CreateKillEnemyMission("Damn the pestering airborne pests!", 0, 5, 5, true, "Defeat 5 flies!" );
	CreateKillEnemyMission("Slimy disaster", 5, 0, 0, true, "Defeat 5 slimes!");
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true, "Defeat 5 chargers!");
}

//For pushing back into vector
void MissionSystem::CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly, bool avail, const char* missionDetails)
{
	enemyMissions.push_back(KillEnemyMission(nextMissionID++, missionName, targetSlime, targetCharger, targetFly, avail, missionDetails));
	std::cout << "Mission name: " << missionName << " with the goal of killing " << targetSlime <<
		" amount of slimes, " << targetCharger << " amount of chargers, and " << targetFly << " of Flys have been added\n";

	std::cout << missionDetails << std::endl;

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
	if (GetAcceptedMissionID() == -1)
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
	}
	else
	{
		std::cout << "You have an ongoing mission!\n";
	}
}

void MissionSystem::MissionComplete(int missionID) //Please check if clear condition is met before calling
{
	for (KillEnemyMission& mission : enemyMissions)
	{
		if (mission.missionID == missionID)
		{
			mission.completed = true;
			mission.accepted = false;
			
			missionSystem.chargersKilled = missionSystem.fliesKilled = missionSystem.slimesKilled = 0; //Reset

			std::cout << "Mission " << mission.missionName << " has been completed\n";

			//Here to add clear rewards
			switch (missionID)
			{
			case 0: //Need to know which mission ID is which mission
				//Add new mission / add rewards to give
				break;
			case 1:
				CreateKillEnemyMission("Rampaging nightmare strike again", 0, 15, 0, true, "Defeat 10 chargers!");
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

size_t MissionSystem::GetAcceptedMissionID()
{
	for (const KillEnemyMission& mission : enemyMissions)
	{
		if (mission.accepted)
			return mission.missionID;
	}

	return -1; //Nothing
}

void MissionSystem::PrintMissionText()
{
	if (missionSystem.GetAcceptedMissionID() != -1)
	{
		const char* missionBegin = "Mission: ";
		const char* missionName = missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].missionName;

		// Calculate the length of the concatenated string, including null terminators
		size_t charLength = strlen(missionName) + strlen(missionBegin) + 1; // +1 for null terminator

		// Allocate memory for the concatenated string
		char* concatenatedString = new char[charLength];

		// Copy the first string
		strcpy_s(concatenatedString, charLength, missionBegin);

		// Concatenate the second string
		strcat_s(concatenatedString, charLength, missionName);

		// Print the concatenated string
		AEGfxPrint(fontID, concatenatedString,
			-1.f, 0.7f, 0.5f, 1.f, 1.f, 1.f, 1.f);

		//Print the goals
		int numberOfTargets = 0;
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].slimeToKill != 0)
		{
			
			numberOfTargets++;
			std::string str = "Slimes: " + std::to_string(slimesKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].slimeToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
				-1.f, 0.7f - numberOfTargets * 0.1f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		}
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].flyToKill != 0)
		{
			numberOfTargets++;
			std::string str = "Flies: " + std::to_string(fliesKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].flyToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
				-1.f, 0.7f - numberOfTargets * 0.1f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		}
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].chargerToKill != 0)
		{
			numberOfTargets++;

			std::string str = "Chargers: " + std::to_string(chargersKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].chargerToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
				-1.f, 0.7f - numberOfTargets * 0.1f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		}
		delete[] concatenatedString;
	}
}

void MissionSystem::CleanMemory()
{
	enemyMissions.clear();
}
