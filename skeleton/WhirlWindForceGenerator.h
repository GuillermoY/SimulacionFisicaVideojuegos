#pragma once
#include "Particle.h"
#include "WindForceGenerator.h"

using namespace std;

class WhirlWindForceGenerator : public WindForceGenerator
{
public:
	WhirlWindForceGenerator(Vector3D windVel, float k1,
		Vector3D minArea = { 0,10,0 }, Vector3D maxArea = { 10,40,10 }, Vector3D centro = {0,0,0}) :
		WindForceGenerator(windVel, k1, minArea, maxArea), centro(centro) {
	};

	~WhirlWindForceGenerator() override = default;

	Vector3D getWindVel(Vector3D pos) override
	{
		return Vector3D{ -k1 * (pos.z - centro.z),
			k1 * (50 - (pos.y - centro.y)),
			k1 * (pos.x - centro.x),
		};
	};

private:
	Vector3D centro;
};