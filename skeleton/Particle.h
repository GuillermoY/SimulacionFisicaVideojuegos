#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

class Particle
{
public:
    Particle(Vector3D Pos, Vector3D Vel, float masa = 1, Vector3D accel = (0, 20, 0),
        Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f }, const physx::PxGeometry& geom = physx::PxSphereGeometry(2.0f),
        bool render = true, int typeVel = 1);
    Particle(bool render);
    Particle(const Particle& other, bool render = false);
    ~Particle();

    void integrate(double t);
    bool canDelete() { return canBeDeleted; };
    void changeDeletability(bool canDelete) { isDeletable = canDelete; };

    void defineLimits(Vector3 maxLimit, Vector3 minLimit) { minLimt = minLimit; maxLimt = maxLimit; }
    
    //P3 
    void applyForce(Vector3D force);
    float getMass() { return mass; }
    void setMass(float newMass) { mass = newMass;     
    if (mass != 0)
        inv_mass = 1 / mass; }
    Vector3 getVel() { return vel; }
    void setVel(Vector3 newVel) { vel = newVel; }
    Vector3D getPos() { return pose.p; }
    void setPos(Vector3D newPos) { pose.p = newPos; pPos = newPos; }
    void setAccel(Vector3D newAccel) { ac = newAccel; }

private:
    // P1.1
    Vector3 vel;

    physx::PxTransform pose; // A render item le pasamos la dirección de este pose, para actualizarse automáticamente
    RenderItem* renderItem;

    Vector3D ac;  // vector aceleración
    double d; // vector damping
    // Para Verlet
    Vector3D pPos;// previa posición
    int typeVel; // tipo de fórmula de la velocidad

    Vector3 maxLimt = { 30.0f,50.0f,50.0f };
    Vector3 minLimt = { -30.0f,-50.0f,-50.0f };
    // P1.2
    float mass;

    Vector4 color;

    // P2

    float tiempoVida = 10.0f;
    bool canBeDeleted = false;
    bool isDeletable = true;

    // P3
    float inv_mass;

    const physx::PxGeometryHolder geo;

    Vector3D accAccel;
    void clearAccForce() { accAccel = { 0,0,0 }; };

};
