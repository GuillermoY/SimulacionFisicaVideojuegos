#include "Particle.h"

Particle::Particle(Vector3D Pos, Vector3D Vel, float masa, Vector3D accel, const physx::PxGeometry& geo)
    : vel(Vel),
    pose(physx::PxTransform(Pos)),
    pPos(Pos),
    ac(accel),
    mass(masa)
{
    physx::PxShape* shape = CreateShape(geo);

    renderItem = new RenderItem(shape, &pose,Vector4(1.0f, 0.0f, 1.0f, 1.0f));
    d = 0.96;
}

void Particle::integrate(double t)
{
    // Parte 1
    // pose.p += vel * t;
    //ac = vel * t;

    // Parte 2 y 3
    pose.p += vel * t;
    vel = (vel+ac * t) * pow(d,t);

    // Parte 4: Verlet
    //Vector3 posA = pose.p;
    //posA = pose.p * 2.0f - pPos + ac * t * t;
    //pPos = pose.p;
    //vel = (pose.p - pPos) / (2.0f * t);
    //pose.p = posA;
}

Particle::~Particle()
{
    if (renderItem)
    {
        renderItem->release();
        renderItem = nullptr;
    }
}
