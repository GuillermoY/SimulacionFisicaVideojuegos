#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
	Vector4 color, float infGrav)
	: velMax(velMax), velMin(velMin), distPos(distPos), dir(dir), pos(pos), color(color),
	infGrav(infGrav)
{

	gravGenerator = new GravForceGenerator(Vector3D{ 0.0f,-10.0f ,0.0f });
	windGenerator = new WindForceGenerator(Vector3D{ 1.0f,10.0f ,1.0f }, 2);
	whirlWindGenerator = new WhirlWindForceGenerator(
		Vector3D(10, 100, 10),
		2.0f,                   
		Vector3D(-20, 0, -20),  
		Vector3D(20, 40, 20)
	);
	explosionGenerator = new ExplosionForceGenerator(
		500.0f,
		25.0f,
		1.0f,
		Vector3D(10, 0, 10)
	);
	distr = 0; // 2 tipos, 0 uniforme o 1 Gaussiana
}


void ParticleSystem::integrate(double t)
{
	Vector3D vel;
	Vector3D posA;

	const float velMedia = (velMin + velMax) /2;

	if (distr == 0)
	{
		// Distribución uniforme entre -1 y 1
		const float velDif = (velMax - velMin)/2;
		vel = {
			dir.x * velMedia + uniforme(rng) * velDif,
			dir.y * velMedia + uniforme(rng) * velDif,
			dir.z * velMedia + uniforme(rng) * velDif
		};
		posA = { pos.x + uniforme(rng) * distPos.x,
				pos.y + uniforme(rng) * distPos.y,
				pos.z + uniforme(rng) * distPos.z };
	}
	else
	{
		// Distribución gaussiana entre 0 y 1
		float desviacion = (velMax - velMin) / 6.0f;
		vel = {
			dir.x * velMedia + gaussiana(rng) * desviacion,
			dir.y * velMedia + gaussiana(rng) * desviacion,
			dir.z * velMedia + gaussiana(rng) * desviacion
		};
		posA = { pos.x + gaussiana(rng) * distPos.x,
				pos.y + gaussiana(rng) * distPos.y,
				pos.z + gaussiana(rng) * distPos.z };
	}
	particles.push_back(new Particle(posA, vel, pMass, 0, color));
	auto it = particles.begin();
	if(explosionGenerator)
		explosionGenerator->integrate(t);
	while (it != particles.end())
	{
		Particle* p = *it;
		//if (gravGenerator)
		//	gravGenerator->applyForceTo(p);
		if (windGenerator)
			windGenerator->applyForceTo(p);
		//if (whirlWindGenerator)
		//	whirlWindGenerator->applyForceTo(p);
		//if (explosionGenerator)
		//	explosionGenerator->applyForceTo(p);
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
	
	if (gravGenerator)
		delete gravGenerator;

	if (windGenerator)
		delete windGenerator;
	
	if (whirlWindGenerator)
		delete whirlWindGenerator;

	if (explosionGenerator)
		delete explosionGenerator;
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
