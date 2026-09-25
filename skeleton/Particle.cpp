#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel) : vel(Vel), pose(physx::PxTransform(Pos)), pPos(Pos)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
	renderItem = new RenderItem(shape, &pose, Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	ac = Vector3(0, 20, 0);
	d = 0.6;
}

void Particle::integrate(double t)
{
	// Parte 1
	// pose.p += vel * t;
	//ac = vel * t;

	// Parte 2 y 3
	//pose.p += vel;
	//vel = (ac * t) * d;

	// Parte 4: Verlet
	Vector3 posA = pose.p;
	posA = pose.p * 2.0f - pPos + ac * t * t;
	pPos = pose.p;
	vel = (pose.p - pPos) / (2.0f*t);
	pose.p = posA;
}

Particle::~Particle()
{
	if (renderItem) {
		renderItem->release();
		renderItem = nullptr;
	}
}