#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

class Particle
{
public:
    Particle(Vector3D Pos, Vector3D Vel, float masa=1, Vector3D accel = (0,20,0), const physx::PxGeometry& geo = physx::PxSphereGeometry(2.0f));
    ~Particle();

    void integrate(double t);

private:
    Vector3 vel;

    physx::PxTransform pose; // A render item le pasamos la dirección de este pose, para actualizarse automáticamente
    RenderItem* renderItem;

    Vector3D ac;  // vector aceleración
    double d; // vector damping
    // Para Verlet
    Vector3 pPos;// previa posición

    float mass;
    float simMass;
};
