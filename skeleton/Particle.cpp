#include "Particle.h"
#include <iostream>
Particle::Particle(Vector3D Pos, Vector3D Vel, float masa, Vector3D accel, 
    Vector4 color, const physx::PxGeometry& geom, bool render, int typeVel)
    : vel(Vel), typeVel(typeVel),
    pose(physx::PxTransform(Pos)), pPos(Pos),
    ac(accel),
    mass(masa),
    geo(physx::PxGeometryHolder(geom)),
    color(color)
{
    clearAccForce();
    inv_mass = 0;
    if (mass != 0)
        inv_mass = 1 / mass;
    
    renderItem = nullptr;

    if (render) {
        physx::PxShape* shape = CreateShape(geom);
        renderItem = new RenderItem(shape, &pose, color);
    }

    d = 0.96;
}

Particle::Particle(const Particle& p, bool render) :
    vel(p.vel), typeVel(p.typeVel),
    pose(p.pose), pPos(p.pPos),
    ac(p.ac),
    mass(p.mass), inv_mass(p.inv_mass),
    geo(p.geo),
    tiempoVida(p.tiempoVida),
    maxLimt(p.maxLimt), minLimt(p.minLimt),
    color(p.color)
{
    clearAccForce();
    renderItem = nullptr;
    if (render)
    {
        physx::PxShape* shape = CreateShape(p.geo.any());
        renderItem = new RenderItem(shape, &pose, p.color);
    }
    d = 0.96;
}

Particle::Particle(bool render) : 
vel(0.0f), typeVel(1), 
pose(physx::PxTransform({0,0,0})), pPos(Vector3D()),
ac(Vector3D()),
mass(1.0f),
geo(physx::PxSphereGeometry(2.0f)){
    clearAccForce();
    inv_mass = 0;
    if (mass != 0)
        inv_mass = 1 / mass;
    renderItem = nullptr;

    if (render) {
        physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));

        renderItem = new RenderItem(shape, &pose, { 1.0f,1.0f,1.0f,1.0f });
    }
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
    if (tiempoVida <= 0 && isDeletable)
        canBeDeleted = true;

    if ((pose.p.x > maxLimt.x || pose.p.x < minLimt.x ||
        pose.p.y > maxLimt.y || pose.p.y < minLimt.y ||
        pose.p.z > maxLimt.z || pose.p.z < minLimt.z) && isDeletable)
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