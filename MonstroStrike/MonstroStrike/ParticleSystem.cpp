#include "ParticleSystem.h"
#include <iostream>
#include <random>
#include "CSVMapLoader.h"

namespace {
	std::vector<AEGfxTexture*> particleTextureList;

	Particles allParticles[MAX_PARTICLE_COUNT];

	std::vector<int> inactiveParticles;

	int activeCount = 0;
}

void Particles::Update()
{
	position.x += velocity.x * (f32)AEFrameRateControllerGetFrameTime();
	position.y += velocity.y * (f32)AEFrameRateControllerGetFrameTime();

	//std::cout << "Active Particles: " << GetActiveParticleCount() << std::endl;
}

void ParticleLoad()
{
	//Load all textures that will be used
	ParticlesAddTexture("Assets/StardustParticle.png");
	ParticlesAddTexture("Assets/PlanetTexture.png");
}

void ParticleInitialize()
{
	//Set all active particles to false first
	for (int i = 0; i < MAX_PARTICLE_COUNT; ++i)
	{
		allParticles[i].translation = { 0 };
		allParticles[i].rotation = { 0 };
		allParticles[i].scale = { 0 };
		allParticles[i].transformation = { 0 };
		allParticles[i].textureIndex = 0;
		allParticles[i].particleType = ParticleType::TEST;
		allParticles[i].rotate = 0.f;

		allParticles[i].maximumLifeTime = 1.f;
		allParticles[i].lifeTime = 1.f;
		allParticles[i].alpha = 1.f;
		
		allParticles[i].active = false;
		inactiveParticles.push_back(i); //Add to inactive particles
	}
}

void ParticleUpdate()
{
	//std::cout << "Active Particles: " << GetActiveParticleCount() << std::endl;
	//std::cout << "Inactive Particles: " << GetInactiveParticleCount() << std::endl;

	for (int i = 0; i < MAX_PARTICLE_COUNT; ++i)
	{
		if (allParticles[i].active)
		{
			allParticles[i].Update(); //Positional update

			AEMtx33Rot(&allParticles[i].rotation, allParticles[i].rotate);

			AEMtx33Scale(&allParticles[i].scale, allParticles[i].particleSize.x, allParticles[i].particleSize.y);

			AEMtx33Trans(&allParticles[i].translation, allParticles[i].position.x, allParticles[i].position.y);

			allParticles[i].transformation = { 0 };
			AEMtx33Concat(&allParticles[i].transformation, &allParticles[i].rotation, &allParticles[i].scale);
			AEMtx33Concat(&allParticles[i].transformation, &allParticles[i].translation, &allParticles[i].transformation);

			if (allParticles[i].lifeTime > 0.f)
			{
				allParticles[i].lifeTime -= (f32)AEFrameRateControllerGetFrameTime();
				allParticles[i].alpha -= (f32)AEFrameRateControllerGetFrameTime();
				//allParticles[i].rotate += 0.04f; //Just for testing
			}
			else
			{
				ParticlesDeactivate(i); 
			}

			if (allParticles[i].alpha <= 0.f)
			{
				ParticlesDeactivate(i);
			}
		}
	}
}

void ParticleEmit(s16 amount, f32 posX, f32 posY, f32 sizeX, f32 sizeY, f32 initialRadian, ParticleType particlePurpose, Player* player)
{

	f32 angle = 0.f;
	f32 speed = 0.f;

	f32 radians = 0.f;

	for (int i = 0; i < amount; ++i)
	{
		int index = inactiveParticles.back();
		inactiveParticles.pop_back(); //Remove particle from behind

		//Initial positions
		allParticles[index].position.x = posX;
		allParticles[index].position.y = posY;

		//Initial size
		allParticles[index].particleSize.x = sizeX;
		allParticles[index].particleSize.y = sizeY;
		
		//Rotation
		allParticles[index].rotate = initialRadian;

		//Type of particle
		allParticles[index].particleType = particlePurpose;

		//Setting initial velocities and textures index (Texture index maybe can just base on particle type)
		switch (allParticles[index].particleType)
		{
		case ParticleType::TEST:

			angle = static_cast<f32>(rand() % 360);
			speed = static_cast<f32>(rand() % 60 + 10);

			radians = angle * 3.14159f / 180.f;

			allParticles[index].velocity.x = speed * cos(radians);
			allParticles[index].velocity.y = speed * sin(radians);

			allParticles[index].textureIndex = 0;
			break;
		case ParticleType::PARTICLE_TRAILING:
			{
			f32 offsetDistance = player->obj.img.scale.x * 0.2f;

			// Assuming player's velocity is stored in playerVelocityX and playerVelocityY
			f32 offsetX = player->velocity.x * offsetDistance;

			// Initial positions with offset
			allParticles[index].position.x = player->obj.pos.x - offsetX;
			allParticles[index].position.y = player->obj.pos.y;

			allParticles[index].textureIndex = 0; 
			}	
			break;
		case ParticleType::PARTICLE_JUMP:
		{
			// Calculate offset distance from the player position
			f32 offsetDistance = player->obj.img.scale.x * 0.5f;

			allParticles[index].velocity.x = (AERandFloat() * 2.0f - 1.0f) * (GRID_SIZE * 2.f); //Split

			// Initial positions with offset
			allParticles[index].position.x = player->obj.pos.x;
			allParticles[index].position.y = player->obj.pos.y - offsetDistance;

			allParticles[index].textureIndex = 0;
		}
		break;
		default:
			break;
		}

		AEMtx33Rot(&allParticles[index].rotation, allParticles[index].rotate);

		AEMtx33Scale(&allParticles[index].scale, allParticles[index].particleSize.x, allParticles[index].particleSize.y);

		AEMtx33Trans(&allParticles[index].translation, allParticles[index].position.x, allParticles[index].position.y);

		AEMtx33Concat(&allParticles[index].transformation, &allParticles[index].rotation, &allParticles[index].scale);
		AEMtx33Concat(&allParticles[index].transformation, &allParticles[index].translation, &allParticles[index].transformation);

		activeCount++;
		allParticles[index].active = true;
	}
}

void ParticlesDraw(AEGfxVertexList& mesh)
{
	//Go through all the particles
	for (int i = 0; i < MAX_PARTICLE_COUNT; ++i)
	{
		if (allParticles[i].active)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransparency(allParticles[i].alpha);
			//Draw based on texture index
			AEGfxTextureSet(particleTextureList[allParticles[i].textureIndex], 0, 0); //Maybe can try particleTextureList[allParticles->particleType]
			AEGfxSetTransform(allParticles[i].transformation.m);
			AEGfxSetColorToMultiply(0.38f, 0.96f, 0.88f, 1.f);
			AEGfxMeshDraw(&mesh, AE_GFX_MDM_TRIANGLES);
			AEGfxSetTransparency(1.0f);
			AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);

		}
	}
}

void ParticlesFree()
{
	for (AEGfxTexture* texture : particleTextureList)
	{
		AEGfxTextureUnload(texture);
	}

	particleTextureList.clear();
	inactiveParticles.clear();
}

void ParticlesAddTexture(const char* fileName)
{
	AEGfxTexture* newTexture;
	newTexture = AEGfxTextureLoad(fileName);
	particleTextureList.push_back(newTexture);
}

void ParticlesDeactivate(int index)
{
	if (index >= 0 && index < MAX_PARTICLE_COUNT)
	{
		if (allParticles[index].active)
		{
			allParticles[index].active = false;

			allParticles[index].translation = { 0 };
			allParticles[index].rotation = { 0 };
			allParticles[index].scale = { 0 };
			allParticles[index].transformation = { 0 };
			allParticles[index].textureIndex = 0;
			allParticles[index].particleType = ParticleType::TEST;
			allParticles[index].alpha = 1.f;
			allParticles[index].rotate = 0.f;
			allParticles[index].velocity = { 0.f };

			allParticles[index].lifeTime = allParticles[index].maximumLifeTime;

			inactiveParticles.push_back(index);
			activeCount--;
		}
	}
}

//Get Set
int GetActiveParticleCount()
{
	return activeCount;
}

size_t GetInactiveParticleCount()
{
	return inactiveParticles.size();
}
