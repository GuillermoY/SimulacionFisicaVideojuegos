#pragma once

#include "P1S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P1S_Scene::init() {
    //P1.2
    escaladoSimulado();
}

void P1S_Scene::update(double dt) {
    // Lógica/Integración del alumno (por ejemplo, movimiento simple)
    for (Particle* part : particulas)
    {
        part->integrate(dt);
    }
}

void P1S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }
    //P1.2
    if (key == '+')
    {
        cout << "mas masa" << "\n";
        mReal += 0.001f;
        escaladoSimulado();

    }
    else if (key == '-')
    {
        cout << "menos masa" << "\n";
        mReal -= 0.001f;
        if (mReal < 0.001f)
            mReal = 0.001f;
        escaladoSimulado();
    }

    if (key == 'M')
    {
        cout << "mas vel" << "\n";
        vReal += 10.0f;
        escaladoSimulado();
    }
    else if (key == 'm')
    {
        cout << "menos vel" << "\n";
        vReal -= 10.0f;

        if (vReal < 10.0f)
            vReal = 10.0f;
        escaladoSimulado();
    }

    if (key == 'c' || key == 'b')
    {
        shoot(key, camera);
    }
}

void P1S_Scene::shoot(unsigned char key, const physx::PxTransform& camera)
{
    Vector3D posIni = camera.p;
    Vector3D posDir = camera.q.rotate(physx::PxVec3(0.0f, 0.0f, -1.0f));
    posDir = posDir.normalize();

    Vector3D vel = posDir * vSim;
    Vector3D ac(0.0f, gSim, 0.0f);

    Particle* nPart;
    if (key == 'b')
    {
        nPart = new Particle(posIni, vel, mSim, ac, physx::PxBoxGeometry(2.0f, 2.0f, 2.0f));
    }
    else
    {
        nPart = new Particle(posIni, vel, mSim, ac);
    }
    particulas.push_back(nPart);
}

void P1S_Scene::escaladoSimulado()
{
    mSim = mReal * (vReal / vSim) * (vReal / vSim);
    gSim = gReal * (vSim / vReal) * (vSim / vReal);
}

void P1S_Scene::cleanup() {
    if (m_renderItem) {
        m_renderItem->release(); // Deregistra y destruye el item
        m_renderItem = nullptr;
    }
    for (Particle* part : particulas)
    {
        delete part;
    }
    particulas.clear();
}