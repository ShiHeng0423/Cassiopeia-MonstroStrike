#include "MissionList.h"
#include "main.h"
#include "Inventory.h"

//Global System
struct MissionSystem missionSystem;

//Construct mission
KillEnemyMission::KillEnemyMission(int missionID, const char* name, int slimeTarg, int chargerTarg, int flyTarg,
	bool avail, const char* missionDetails, const std::vector<std::string>& rewards)
	: missionName{ name },
	missionDetails{ missionDetails },
	rewardDetails{ rewards }, // Initialize the vector of rewards
	completed{ false }, accepted{ false },
	available{ avail }, missionID{ missionID },
	slimeToKill{ slimeTarg },
	chargerToKill{ chargerTarg },
	flyToKill{ flyTarg }
{
}

void MissionSystem::InitialMission()
{

	//Available from the start
	CreateKillEnemyMission("Damn the pestering airborne pests!", 0, 0, 5, true, "Defeat 5 flies!", GetRewardsForMission(14, 2, 17, 1));
	CreateKillEnemyMission("Slimy disaster", 5, 0, 0, true, "Defeat 5 slimes!", GetRewardsForMission(15, 2, 17, 1));
	CreateKillEnemyMission("Rampaging nightmare", 0, 5, 0, true, "Defeat 5 chargers!", GetRewardsForMission(16, 2, 17, 1));

	//Slowly unlock
	CreateKillEnemyMission("Airborne annoyance", 0, 0, 10, false, "Defeat 10 flyers!", GetRewardsForMission(14, 2, 18, 1));
	CreateKillEnemyMission("Sticky situation", 0, 10, 0, false, "Defeat 10 slimes!", GetRewardsForMission(15, 2, 18, 1));
	CreateKillEnemyMission("Rampaging nightmare strike again", 0, 10, 0, false, "Defeat 10 chargers!", GetRewardsForMission(16, 2, 18, 1));
}

//For pushing back into vector
void MissionSystem::CreateKillEnemyMission(const char* missionName, int targetSlime, int targetCharger, int targetFly,
	bool avail, const char* missionDetails, const std::vector<std::string>& rewardDetails)
{
	enemyMissions.push_back(KillEnemyMission(nextMissionID++, missionName, targetSlime, targetCharger, targetFly,
		avail, missionDetails, rewardDetails));
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
				missionSystem.chargersKilled = missionSystem.fliesKilled = missionSystem.slimesKilled = 0;
				return;
			}
		}
	}
	else
	{
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
			mission.available = true; //Decided to just keep the mission available for players

			missionSystem.chargersKilled = missionSystem.fliesKilled = missionSystem.slimesKilled = 0; //Reset
			//Here to add clear rewards
			switch (missionID)
			{
			case 0:
				Inventory::AddItem(fullInventoryList[14]);
				Inventory::AddItem(fullInventoryList[14]);

				Inventory::AddItem(fullInventoryList[17]);

				enemyMissions[3].available = true;
				break;
			case 1:
				Inventory::AddItem(fullInventoryList[15]);
				Inventory::AddItem(fullInventoryList[15]);

				Inventory::AddItem(fullInventoryList[17]);

				enemyMissions[4].available = true;
				break;
			case 2:
				Inventory::AddItem(fullInventoryList[16]);
				Inventory::AddItem(fullInventoryList[16]);

				Inventory::AddItem(fullInventoryList[17]);

				enemyMissions[5].available = true;
				break;
			case 3:
				Inventory::AddItem(fullInventoryList[14]);
				Inventory::AddItem(fullInventoryList[14]);

				Inventory::AddItem(fullInventoryList[18]);

				break;
			case 4:
				Inventory::AddItem(fullInventoryList[15]);
				Inventory::AddItem(fullInventoryList[15]);

				Inventory::AddItem(fullInventoryList[18]);

				break;
			case 5:
				Inventory::AddItem(fullInventoryList[15]);
				Inventory::AddItem(fullInventoryList[15]);

				Inventory::AddItem(fullInventoryList[18]);

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
		           -0.95f, 0.65f, 0.3f, 1.f, 1.f, 1.f, 1.f);

		//Print the goals
		int numberOfTargets = 0;
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].slimeToKill != 0)
		{
			numberOfTargets++;
			std::string str = "Slimes: " + std::to_string(slimesKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].slimeToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
			           -0.95f, 0.7f - numberOfTargets * 0.1f, 0.3f, 1.f, 1.f, 1.f, 1.f);
		}
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].flyToKill != 0)
		{
			numberOfTargets++;
			std::string str = "Flies: " + std::to_string(fliesKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].flyToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
			           -0.95f, 0.7f - numberOfTargets * 0.1f, 0.3f, 1.f, 1.f, 1.f, 1.f);
		}
		if (missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].chargerToKill != 0)
		{
			numberOfTargets++;

			std::string str = "Chargers: " + std::to_string(chargersKilled) + " / "
				+ std::to_string(missionSystem.enemyMissions[missionSystem.GetAcceptedMissionID()].chargerToKill);

			const char* goal = str.c_str();

			AEGfxPrint(fontID, goal,
			           -0.95f, 0.7f - numberOfTargets * 0.1f, 0.3f, 1.f, 1.f, 1.f, 1.f);
		}

		if (CheckMissionClear())
		{
			AEGfxPrint(fontID, "Mission Complete! Return to quest giver to claim your rewards!",
				-0.95f, 0.7f - (1+numberOfTargets) * 0.1f, 0.3f, 1.f, 1.f, 1.f, 1.f);
		}
		delete[] concatenatedString;
	}
}

bool MissionSystem::CheckMissionClear()
{
	size_t id = missionSystem.GetAcceptedMissionID();

	if (id != -1)
	{
	}

	switch (id)
	{
	case 0:
		if (fliesKilled >= enemyMissions[id].flyToKill)
			return true;
		break;
	case 1:
		if (slimesKilled >= enemyMissions[id].slimeToKill)
			return true;
		break;
	case 2:
		if (chargersKilled >= enemyMissions[id].chargerToKill)
			return true;
		break;
	case 3:
		if (fliesKilled >= enemyMissions[id].flyToKill)
			return true;
		break;
	case 4:
		if (slimesKilled >= enemyMissions[id].slimeToKill)
			return true;
		break;
	case 5:
		if (chargersKilled >= enemyMissions[id].chargerToKill)
			return true;
		break;
	default:
		return false;
	}

	return false;
}

int MissionSystem::FindItemID(const std::string& itemName)
{
	for (size_t i = 0; i < fullInventoryList.size(); ++i)
	{
		if (fullInventoryList[i].name == itemName)
		{
			return i;
		}
	}
	return -1; // Item not found
}

std::vector<std::string> MissionSystem::GetRewardsForMission(int reward1ID, int amountForReward1, int reward2ID, int amountForReward2)
{
	std::vector<std::string> rewards;
	std::string reward1 = fullInventoryList[reward1ID].name;
	reward1 += " x" + std::to_string(amountForReward1);
	std::string reward2 = fullInventoryList[reward2ID].name;
	reward2 += " x" + std::to_string(amountForReward2);
	rewards.push_back(reward1);
	rewards.push_back(reward2);
	return rewards;
}

void MissionSystem::CleanMemory()
{
	enemyMissions.clear();
}
