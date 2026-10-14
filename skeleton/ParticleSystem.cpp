#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float velMin, float velMax, float distPos, Vector3D dir, Vector3D pos, Vector4 color)
	: velMax(velMax), velMin(velMin), distPos(distPos), dir(dir), pos(pos), color(color)
{
	distr = 0; // 2 tipos, 0 uniforme o 1 Gaussiana
}


void ParticleSystem::integrate(double t)
{
	Vector3D vel;
	Vector3D posA;

	float velMedia = (velMin + velMax) * 0.5f;

	if (distr == 0)
	{
		// Distribución uniforme entre -1 y 1
		vel = {
			dir.x * velMedia + uniforme(rng) * (velMax - velMin) * 0.5f,
			dir.y * velMedia + uniforme(rng) * (velMax - velMin) * 0.5f,
			dir.z * velMedia + uniforme(rng) * (velMax - velMin) * 0.5f
		};
		posA = { pos.x + uniforme(rng) * distPos,
				pos.y + uniforme(rng) * distPos,
				pos.z + uniforme(rng) * distPos };
	}
	else
	{
		// Distribución gaussiana
		float desviacion = (velMax - velMin) / 6.0f;
		vel = {
			dir.x * velMedia + gaussiana(rng) * desviacion,
			dir.y * velMedia + gaussiana(rng) * desviacion,
			dir.z * velMedia + gaussiana(rng) * desviacion
		};
		posA = { pos.x + gaussiana(rng) * distPos,
				pos.y + gaussiana(rng) * distPos,
				pos.z + gaussiana(rng) * distPos };
	}
	particles.push_back(new Particle(posA, vel, 1.0f, grav, color));
	auto it = particles.begin();
	while (it != particles.end())
	{
		Particle* p = *it;
			
		p->integrate(t);

		if (p->canDelete())
		{
			delete p;
			it = particles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : particles)
	{
		delete p;
	}
	particles.clear();
}

void ParticleSystem::switchState()
{
	isStop = !isStop;
	if (isStop)
	{
		for (Particle* p : particles)
		{
			delete p;
		}
		particles.clear();
	}
}
