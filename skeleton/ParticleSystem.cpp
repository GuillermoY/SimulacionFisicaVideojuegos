#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
	Vector4 color, const physx::PxGeometry& geo, ForceType type)
	: velMax(velMax), velMin(velMin), distPos(distPos), dir(dir), pos(pos), color(color),
	geoPart(physx::PxGeometryHolder(geo)), pModelo(nullptr)
{
	if (type == ForceType::NONE)
		forceGen.clear();
	distr = 0; // 2 tipos, 0 uniforme o 1 Gaussiana
}

ParticleSystem::ParticleSystem(float velMin, float velMax, Vector3D distPos, Vector3D dir, Vector3D pos,
	Particle* model, ForceType type)
	: velMax(velMax), velMin(velMin), distPos(distPos), dir(dir), pos(pos), pModelo(new Particle(*model))
{
	pModelo->changeDeletability(false);
	hasP = true;
	if (type == ForceType::NONE)
		forceGen.clear();
	distr = 0; // 2 tipos, 0 uniforme o 1 Gaussiana
}


void ParticleSystem::integrate(double t)
{
	if (isStop)
		return;

	for (int i = 0; i < forceGen.size(); ++i)
	{
		if (typeF[i] == ForceType::EXPLOSION)
		{
			if (!activeExpl)
			{
				ExplosionForceGenerator* explosion = static_cast<ExplosionForceGenerator*>(forceGen[i]);

				explosion->reinicio();

				activeExpl = true;

				for (int i = 0; i < nExplParts; ++i)
					generatePart();
			}
		}
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

	if (hasP)
	{
		Particle* p = new Particle(*pModelo, true);
		//p->defineLimits(maxLimt, minLimt);
		p->setPos(posA);
		p->setVel(vel);
		p->defineLimits(maxLimt, minLimt);
		particles.push_back(p);
	}
	else
	{
		Particle* p = new Particle(posA, vel, pMass, 0, color, geoPart.any());
		p->defineLimits(maxLimt,minLimt);
		particles.push_back(p);
	}
}

void ParticleSystem::updatePart(float t)
{
	auto it = particles.begin();
	int i = 0;
	for (ForceGenerator* f : forceGen)
	{
		if (f && typeF[i] == ForceType::EXPLOSION)
			f->integrate(t);
		++i;
	}

	while (it != particles.end())
	{
		Particle* p = *it;

		for (ForceGenerator* f : forceGen)
		{
			if (f)
				f->applyForceTo(p);
		}

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
	delete pModelo;
	for (ForceGenerator* f : forceGen)
	{
		delete f;
	}
	forceGen.clear();

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
