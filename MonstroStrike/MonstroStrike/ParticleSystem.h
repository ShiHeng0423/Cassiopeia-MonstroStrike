#pragma once
#include <AEEngine.h>
#include <vector>

enum {
	MAX_PARTICLE_COUNT = 1000
};

enum PARTICLE_TYPE {
	TEST = 0,
	TEST_2
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

	PARTICLE_TYPE particleType;

	AEMtx33 transformation;
	AEMtx33 scale;
	AEMtx33 rotation;
	AEMtx33 translation;


	void Update();
};

void ParticleLoad();

void ParticleInitialize();

void ParticleUpdate();

void ParticleEmit(s8 amount, f32 posX, f32 posY, f32 sizeX, f32 sizeY, f32 initialRadian, PARTICLE_TYPE particlePurpose);

void ParticlesDraw(AEGfxVertexList& mesh);

void ParticlesFree();

void ParticlesAddTexture(const char* fileName);

void ParticlesDeactivate(int index);

int GetActiveParticleCount();

size_t GetInactiveParticleCount();