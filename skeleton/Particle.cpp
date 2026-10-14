#include "Particle.h"

Particle::Particle(Vector3D Pos, Vector3D Vel, float masa, Vector3D accel, 
    Vector4 color, const physx::PxGeometry& geo)
    : vel(Vel),
    pose(physx::PxTransform(Pos)),
    pPos(Pos),
    ac(accel),
    mass(masa)
{
    physx::PxShape* shape = CreateShape(geo);

    renderItem = new RenderItem(shape, &pose,color);
    d = 0.96;
}

void Particle::integrate(double t)
{
    // Parte 1
    // pose.p += vel * t;
    //ac = vel * t;

    // Parte 2 y 3
    pose.p += vel * t;
    vel = (vel + ac * t);// *pow(d, t);

    // Parte 4: Verlet
    //Vector3 posA = pose.p;
    //posA = pose.p * 2.0f - pPos + ac * t * t;
    //pPos = pose.p;
    //vel = (pose.p - pPos) / (2.0f * t);
    //pose.p = posA;

    // P2
    tiempoVida -= t;
    if (tiempoVida <= 0)
        canBeDeleted = true;

    if (pose.p.x > 30.0f || pose.p.x < -30.0f ||
        pose.p.y > 50.0f || pose.p.y < -50.0f ||
        pose.p.z > 50.0f || pose.p.z < -50.0f)
        canBeDeleted = true;
}

Particle::~Particle()
{
    if (renderItem)
    {
        renderItem->release();
        renderItem = nullptr;
    }
}
