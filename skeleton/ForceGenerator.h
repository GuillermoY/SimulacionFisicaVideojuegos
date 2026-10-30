#pragma once
#include "Particle.h"

using namespace std;

class ForceGenerator
{
public:
	virtual ~ForceGenerator() = default;

	virtual void applyForceTo(Particle* p)=0;
};