#pragma once
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "Particle.h"
#include <vector>
#include <random>
#include "GravForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlWindForceGenerator.h"
#include "ExplosionForceGenerator.h"

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
		Vector4 color = { 1.0f, 0.0f, 0.5f, 1.0f }, float infGrav=1);
	~ParticleSystem();

	void integrate(double t);

	void switchState(); // Intercambia entre estar parado o continuar
	bool isOn() { return !isStop; };

	//P3
	void changeMass(float mass) { pMass += mass; };
private:
	float velMax;
	float velMin;
	Vector3D distPos;

	Vector3D dir;
	Vector3D pos;

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> uniforme{ -1.0f, 1.0f };
	std::normal_distribution<float> gaussiana{ 0.0f, 1.0f };

	vector<Particle*> particles;


	float infGrav = 1.0f;

	int distr = 0;
	
	Vector4 color;

	bool isStop = true;

	//P3
	float pMass=1.0f;

	GravForceGenerator* gravGenerator = nullptr;
	WindForceGenerator* windGenerator = nullptr;
	WindForceGenerator* whirlWindGenerator = nullptr;
	ExplosionForceGenerator* explosionGenerator = nullptr;
};