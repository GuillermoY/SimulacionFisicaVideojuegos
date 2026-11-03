#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
	Vector4 color, const physx::PxGeometry& geo, ForceType type)
	: velMax(velMax), velMin(velMin), distPos(distPos), dir(dir), pos(pos), color(color),
	geoPart(physx::PxGeometryHolder(geo)), typeF(type)
{
	if (type == ForceType::NONE)
		forceGen = nullptr;
	distr = 0; // 2 tipos, 0 uniforme o 1 Gaussiana
}


void ParticleSystem::integrate(double t)
{
	if (isStop)
		return;

	if (typeF == ForceType::EXPLOSION)
	{
		if (!activeExpl)
		{
			ExplosionForceGenerator* explosion = static_cast<ExplosionForceGenerator*>(forceGen);

			explosion->reinicio();

			activeExpl = true;

			for (int i = 0; i < nExplParts; ++i)
				generatePart();
		}

		updatePart(t);
		return;
	}

	generatePart();
	updatePart(t);
}
void ParticleSystem::generatePart()
{
	Vector3D vel;
	Vector3D posA;

	const float velMedia = (velMin + velMax) / 2;

	if (distr == 0)
	{
		// Distribución uniforme entre -1 y 1
		const float velDif = (velMax - velMin) / 2;
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

	particles.push_back(new Particle(posA, vel, pMass, 0, color, geoPart.any()));
}

void ParticleSystem::updatePart(float t)
{
	auto it = particles.begin();

	if (forceGen && typeF == ForceType::EXPLOSION)	
		forceGen->integrate(t);

	while (it != particles.end())
	{
		Particle* p = *it;

		if (forceGen)
			forceGen->applyForceTo(p);

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
	
	delete forceGen;
	forceGen = nullptr;

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

		activeExpl = false;
	}
}
