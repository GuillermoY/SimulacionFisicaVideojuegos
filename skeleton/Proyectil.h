#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "Particle.h"

class Proyectil
{
public:
    enum class ProjectileType
    {
        BOX,
        SPHERE
    };

    Proyectil(Vector3D pos, Vector3D dir, float velReal, ProjectileType projType);
    ~Proyectil();

    void integrate(double t);

    void setVel(float vel) { vReal = vel; escaladoSimulado(); };
    float getVel() { return vReal;};
    void setMass(float mass) { mReal = mass; escaladoSimulado(); };
    float getMass() { return mReal; };

    Particle* getParticle() { return p; }

private:
    Particle* p;

    float mReal;
    float vReal;
    float gReal;

    float mSim;
    float vSim;
    float gSim;
    Vector3D dir;

    ProjectileType pType;
    void escaladoSimulado();

};
