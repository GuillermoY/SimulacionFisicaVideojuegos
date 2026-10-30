#pragma once
#include "Particle.h"
#include "ForceGenerator.h"

using namespace std;

class ExplosionForceGenerator : public ForceGenerator
{
public:
	ExplosionForceGenerator(float R,
		float K, float t, Vector3D centro = { 0,0,0 }) :
		 R(R), centro(centro), K(K), t(t) {
	};

	~ExplosionForceGenerator() override = default;

	void integrate(double dt)
	{
		if (activo)
		{
			e = dt;
			if (e >= 4 * t)
				activo = false;
		}
	}

	void applyForceTo(Particle* p) override
	{
		Vector3D pPos = p->getPos();
		float r = sqrt(pow((pPos.x - centro.x),2) + pow((pPos.y - centro.y), 2) + pow((pPos.z - centro.z), 2));
		if (r < R)
		{

			p->applyForce(K / (R * R) * (pPos.z - centro.z) * exp(-e / t));
		}
		else
			p->applyForce(0);
	}

protected:
	float R;
	float K;
	float t;
	float e=0;
	Vector3D centro;

	bool activo = true;
};