#pragma once
#include "Particle.h"
#include "ForceGenerator.h"

using namespace std;

class GravForceGenerator : public ForceGenerator
{
public:
	GravForceGenerator(Vector3D grav) : grav(grav) {};

	~GravForceGenerator() override = default;


	void applyForceTo(Particle* p) override
	{
		p->applyForce(grav * p->getMass());
	}
private:
	Vector3D grav;
};