#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

class Particle
{
public:
    Particle(Vector3D Pos, Vector3D Vel, float masa=1, Vector3D accel = (0,20,0), 
        Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f }, const physx::PxGeometry& geo = physx::PxSphereGeometry(2.0f));
    ~Particle();

    void integrate(double t);
    bool canDelete() { return canBeDeleted; };
private:
    // P1.1
    Vector3 vel;

    physx::PxTransform pose; // A render item le pasamos la dirección de este pose, para actualizarse automáticamente
    RenderItem* renderItem;

    Vector3D ac;  // vector aceleración
    double d; // vector damping
    // Para Verlet
    Vector3D pPos;// previa posición

    // P1.2
    float mass;

    // P2

    float tiempoVida = 3.0f;
    bool canBeDeleted = false;
};
