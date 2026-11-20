#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d);

	virtual void updateForce(Particle* p)
	{
		float h = p->getPos().y;
		float h0 = _liquid_particle->getPos().y;

		Vector3 f(0, 0, 0);
		float immersed = 0.0;
		if (h - h0 > _height * 0.5)
			immersed = 0.0;
		else if (h0 - h > _height * 0.5)
			immersed = 1.0f;
		else
			immersed = (h0 - h) / _height + 0.5;

		f.y = _liquid_density * _volume * immersed * _gravity;

		p->applyForce(f);
	}

	virtual ~BuoyancyForceGenerator();
protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle; 
};