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

	bool getActivo() { return activo; };

	void reinicio() { e = 0; activo = true; };

	void integrate(double dt) override
	{
		if (!activo)
			return;
		e += dt;
		// NO es necesario, es natural de la fórmula que usa la exponencial
		//if (e >= 4 * t)
		//	activo = false;
	}

	void applyForceTo(Particle* p) override
	{
		if (!activo)
			return;
		Vector3D pPos = p->getPos();
		float r = sqrt(pow((pPos.x - centro.x),2) + pow((pPos.y - centro.y), 2) + pow((pPos.z - centro.z), 2));
		if (r < R)
		{

			p->applyForce({ K / (R) * (pPos.x - centro.x) * exp(-e / t),
				K / (R) * (pPos.y - centro.y) * exp(-e / t) ,
				K / (R) * (pPos.z - centro.z) * exp(-e / t) });
		}
	}

protected:
	float R;
	float K;
	float t;
	float e=0;
	Vector3D centro;

	bool activo = true;
};