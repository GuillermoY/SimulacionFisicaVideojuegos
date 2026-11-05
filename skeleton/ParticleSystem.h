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
	enum class ForceType
	{
		NONE,
		GRAVITY,
		WIND,
		WHIRLWIND,
		EXPLOSION
	};


	ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
		Vector4 color = { 1.0f, 0.0f, 0.5f, 1.0f }, const physx::PxGeometry& geo = physx::PxSphereGeometry(2.0f),
		ForceType type = ForceType::GRAVITY);
	ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos, Particle* p, 
		ForceType type = ForceType::GRAVITY);
	~ParticleSystem();

	void integrate(double t);

	void switchState(); // Intercambia entre estar parado o continuar
	bool isOn() { return !isStop; };

	void changeColor(Vector4 newCol) { color = newCol; }; // nuevo color a las nuevas partículas generadas después del cambio

	//P3
	void changeMass(float mass) { pMass += mass; };

	void setNExplParts(int n) { nExplParts = n; };

	void addGeneratorType(ForceGenerator* newForceGenerator, ForceType newType)
	{
		typeF.push_back(newType);

		forceGen.push_back(newForceGenerator);
	}
	void changeDistr() {
		if (distr == 0)distr = 1;
		else distr = 0;
	};
	void defineLimits(Vector3 maxLimit, Vector3 minLimit) { maxLimt = maxLimit; minLimt = minLimit; }

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
	float pMass = 1.0f;
	Vector3 maxLimt = { 30.0f,50.0f,50.0f };
	Vector3 minLimt = { -30.0f,-50.0f,-50.0f };
	physx::PxGeometryHolder geoPart;

	vector<ForceGenerator*> forceGen;

	void generatePart();
	void updatePart(float t);

	vector<ForceType> typeF;
	bool activeExpl = false;
	int nExplParts = 500;

	Particle* pModelo;
	bool hasP=false;
};