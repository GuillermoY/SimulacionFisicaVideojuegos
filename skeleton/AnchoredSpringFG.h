#pragma once
#include "SpringForceGenerator.h"
#include "core.hpp"

using namespace std;

class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(double k, double resting, const Vector3D& anchor_pos) :
		SpringForceGenerator(k, resting, nullptr) {
		_other = new Particle(anchor_pos, { 0,0,0 }, 1e6, { 0,0,0 },
			{1.0f,1.0f,1.0f,1.0f}, physx::PxBoxGeometry(2.0f,2.0f,2.0f));
		_other->changeDeletability(false);
	}

	virtual ~AnchoredSpringFG() { delete _other; };
	
};