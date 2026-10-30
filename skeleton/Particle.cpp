#include "Particle.h"

Particle::Particle(Vector3D Pos, Vector3D Vel, float masa, Vector3D accel, 
    Vector4 color, const physx::PxGeometry& geo, int typeVel)
    : vel(Vel),
    pose(physx::PxTransform(Pos)),
    pPos(Pos),
    ac(accel),
    mass(masa),
    typeVel(typeVel)
{
    clearAccForce();
    inv_mass = 0;
    if (mass != 0)
        inv_mass = 1 / mass;

    physx::PxShape* shape = CreateShape(geo);

    renderItem = new RenderItem(shape, &pose,color);
    d = 0.96;
}

void Particle::integrate(double t)
{
    if (mass <= 0.0f)
        return;

    if (typeVel == 0)    // Parte 1.01
    {
        pose.p += vel * t;
        ac = vel * t;
    }
    else if (typeVel == 1) // Parte 1.02 y 1.03
    {
        pose.p += vel * t;
        vel = (vel + (ac + accAccel) * t);// *pow(d, t);
    }
    else if (typeVel == 2)    // Parte 1.04: Verlet
    {
        Vector3 posActual = pose.p;
        Vector3 posNueva = posActual * 2.0f - pPos + (ac + accAccel) * t * t;

        vel = (posNueva - pPos) / (2.0f * t);
        pPos = posActual;
        pose.p = posNueva;
    }

    // P2
    tiempoVida -= t;
    if (tiempoVida <= 0)
        canBeDeleted = true;

    if (pose.p.x > 30.0f || pose.p.x < -30.0f ||
        pose.p.y > 50.0f || pose.p.y < -50.0f ||
        pose.p.z > 50.0f || pose.p.z < -50.0f)
        canBeDeleted = true;

    clearAccForce();
}

Particle::~Particle()
{
    if (renderItem)
    {
        renderItem->release();
        renderItem = nullptr;
    }
}

//P3
void Particle::applyForce(Vector3D force)
{
    if (mass <= 0.0f)
        return;

    // Leyes del Movimiento de Newton
    accAccel += force * inv_mass; 
}