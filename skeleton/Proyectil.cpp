#include "Proyectil.h"

Proyectil::Proyectil(Vector3D pos, Vector3D direc, float velReal, ProjectileType projType)
    : vReal(velReal), p(nullptr)
{
    dir = direc.normalize();
    if (projType == ProjectileType::BOX)
    {
        mReal = 0.01f;
        gReal = -9.8f;
    }
    else
    {
        mReal = 5.0f;
        gReal = -9.8f;
    }


    vSim = 20.0f;


    escaladoSimulado();

    Vector3D vel = dir * vSim;
    Vector3D ac(0.0f, gSim, 0.0f);

    if (projType == ProjectileType::BOX)
    {
        p = new Particle(pos, vel, mSim, ac, { 1.0,1.0,1.0,1.0 }, physx::PxBoxGeometry(2.0f, 1.0f, 1.0f));
    }
    else
    {
        p = new Particle(pos, vel, mSim, ac, { 1.0,1.0,1.0,1.0 }, physx::PxSphereGeometry(2.0f));
    }
}

Proyectil::~Proyectil()
{
    delete p;
}

void Proyectil::integrate(double t)
{
    if (p)
        p->integrate(t);
}

void Proyectil::escaladoSimulado()
{
    mSim = mReal * (vReal / vSim) * (vReal / vSim);
    gSim = gReal * (vSim / vReal) * (vSim / vReal);
    if (p)
    {
        p->setMass(mSim);
        p->setAccel({ 0,gSim,0 });
        p->setVel(dir * vSim);
    }
}