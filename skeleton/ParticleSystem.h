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
	~ParticleSystem();

	void integrate(double t);

	void switchState(); // Intercambia entre estar parado o continuar
	bool isOn() { return !isStop; };

	void changeColor(Vector4 newCol) { color = newCol; }; // nuevo color a las nuevas partículas generadas después del cambio

	//P3
	void changeMass(float mass) { pMass += mass; };

	void setNExplParts(int n) { nExplParts = n; };

	void setGeneratorType(ForceGenerator* newForceGenerator, ForceType newType)
	{
		delete forceGen;

		typeF = newType;

		forceGen = newForceGenerator;
	}
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
	physx::PxGeometryHolder geoPart;

	ForceGenerator* forceGen = new GravForceGenerator(Vector3D{ 0.0f,-10.0f ,0.0f });

	void applyForces(Particle* particle);
	void generatePart();
	void updatePart(float t);

	ForceType typeF;
	bool activeExpl = false;
	int nExplParts = 500;
};