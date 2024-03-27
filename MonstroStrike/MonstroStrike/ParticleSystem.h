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