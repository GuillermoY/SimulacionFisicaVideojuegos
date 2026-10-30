#pragma once
#include "Particle.h"
#include "ForceGenerator.h"

using namespace std;

class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(Vector3D windVel, float k1,
	Vector3D minArea = {0,10,0}, Vector3D maxArea={10,40,10}) :
	windVel(windVel), k1(k1), minArea(minArea), maxArea(maxArea) {};

	~WindForceGenerator() override = default;


	void applyForceTo(Particle* p) override
	{
		if (p->getPos().x > minArea.x && p->getPos().x < maxArea.x &&
			p->getPos().y > minArea.y && p->getPos().y < maxArea.y &&
			p->getPos().z > minArea.z && p->getPos().z < maxArea.z)
			p->applyForce(k1*(getWindVel(p->getPos()) - p->getVel()));
	}

	virtual Vector3D getWindVel(Vector3D pos) { return windVel; };
protected:
	Vector3D windVel;
	float k1;

	Vector3D minArea, maxArea;
};