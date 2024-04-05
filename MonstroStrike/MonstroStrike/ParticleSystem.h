/*!************************************************************************
  \file					ParticleSystem.h
  \project name			Monstrostrike
  \primary author		Johny Yong Jun Siang (100%)
  \brief				Header file containing the declaration of a particle system for the Monstrostrike project.
  
  This header file defines the structure and functions related to the particle system. It provides functionalities for particle
  initialization, updating, emission, rendering, and memory management. Utilizing object pooling techniques, the particle system 
  efficiently manages the lifecycle of particles to minimize memory allocation overhead and improve performance.
  
All content © 2024 DigiPen Institute of Technology Singapore. All
rights reserved.
**************************************************************************/
#pragma once
#include <AEEngine.h>
#include <vector>
#include "Player.h"

enum {
	MAX_PARTICLE_COUNT = 1000
};

enum ParticleType {
	ENEMY_DEATH_EFFECT = 0,
	PARTICLE_TRAILING,
	PARTICLE_JUMP,
};

struct Particles
{
	AEVec2 position;
	AEVec2 velocity;
	AEVec2 particleSize;
	f32 rotate;

	bool active;

	f32 lifeTime;
	f32 maximumLifeTime;

	f32 alpha;
	int textureIndex;

	ParticleType particleType;

	AEMtx33 transformation;
	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;


	void Update();
};

void ParticleLoad();

void ParticleInitialize();

void ParticleUpdate();

void ParticleEmit(s16 amount, f32 posX, f32 posY, f32 sizeX, f32 sizeY, f32 initialRadian, ParticleType particlePurpose, Player* player);

void ParticlesDraw(AEGfxVertexList& mesh);

void ParticlesFree();

void ParticlesAddTexture(const char* fileName);

void ParticlesDeactivate(int index);

int GetActiveParticleCount();

size_t GetInactiveParticleCount();