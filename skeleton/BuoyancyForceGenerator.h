#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d, Particle* liquidPart) :
		_height(h), _volume(V), _liquid_density(d), _liquid_particle(liquidPart)
	{
	};

	virtual void applyForceTo(Particle* p) override
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

	float getVolume() { return _volume; };
	void setVolume(float newVol) { _volume = newVol; };

	virtual ~BuoyancyForceGenerator() { };
protected:
	float _height = 0;
	float _volume = 0;
	float _liquid_density = 0;
	float _gravity = 9.8f;

	Particle* _liquid_particle = nullptr;
};

