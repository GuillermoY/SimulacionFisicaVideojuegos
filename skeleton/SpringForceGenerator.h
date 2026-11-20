#pragma once
#include "Particle.h"
#include "ForceGenerator.h"
#include "core.hpp"

using namespace std;

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other)
		: _k(k), _resting_length(resting_length), _other(other) {
		_updates = true;
	};

	virtual ~SpringForceGenerator() {};

	inline void setK(double K) { _k = K; }
	inline double getK() { return _k; }

	void applyForceTo(Particle* particle) override
	{
		Vector3 relative_pos_vector = _other->getPos() - particle->getPos();
		Vector3 force;

		const float length = relative_pos_vector.normalize();
		const float delta_x = length - _resting_length;

		force = relative_pos_vector * delta_x * _k;

		particle->applyForce(force);
	}
protected:
	double _k; // Elastic Coeff.
	double _resting_length;
	Particle* _other;
};