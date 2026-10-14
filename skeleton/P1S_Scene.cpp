#pragma once

#include "P1S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P1S_Scene::init() {
    //P1.1
    
    physx::PxShape* shape3 = CreateShape(physx::PxSphereGeometry(2.0f));
    m_renderParticle1 = new Particle(Vector3(0, 0, 0), Vector3(1, 0, 0), 1);
    particulas.push_back(m_renderParticle1);
    //P1.2
    escaladoSimulado();

    //Opcional

    physx::PxShape* shapeSuelo = CreateShape(physx::PxBoxGeometry(100.0f,2.0f,100.0f));
    m_transformSuelo = physx::PxTransform(physx::PxVec3(0.0f, -30.0f, 0.0f));
    // Se registra el RenderItem exactamente como en la plantilla original
    m_renderItemSuelo = new RenderItem(shapeSuelo, &m_transformSuelo, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    physx::PxShape* shapeSosten = CreateShape(physx::PxBoxGeometry(5.0f, 100.0f, 5.0f));
    m_transformSosten = physx::PxTransform(physx::PxVec3(-100.0f, -30.0f, -100.0f));
    m_renderItemSosten = new RenderItem(shapeSosten, &m_transformSosten, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    physx::PxShape* shapeDiana = CreateShape(physx::PxBoxGeometry(20.0f, 20.0f, 5.0f));
    m_transformDiana = physx::PxTransform(physx::PxVec3(-100.0f, 40.0f, -90.0f));
    m_transformDiana.rotate(physx::PxVec3(90.0f, 90.0f, 90.0f));
    m_renderItemDiana = new RenderItem(shapeDiana, &m_transformDiana, Vector4(0.0f, 1.0f, 1.0f, 1.0f));

    physx::PxShape* shapeDianaCentro = CreateShape(physx::PxBoxGeometry(5.0f, 5.0f, 1.0f));
    m_transformDianaCentro = physx::PxTransform(physx::PxVec3(-100.0f, 40.0f, -85.0f));
    m_transformDianaCentro.rotate(physx::PxVec3(90.0f, 90.0f, 90.0f));
    m_renderItemDianaCentro = new RenderItem(shapeDianaCentro, &m_transformDianaCentro, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void P1S_Scene::update(double dt) {
    m_renderParticle1->integrate(dt);
    
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
        mReal += 0.1f;
        escaladoSimulado();

    }
    else if (key == '-')
    {
        cout << "menos masa" << "\n";
        mReal -= 0.001f;
        if (mReal < 0.001f)
            mReal = 0.1f;
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
        nPart = new Particle(posIni, vel, mSim, ac, { 1.0f, 1.0f, 1.0f, 1.0f }, physx::PxBoxGeometry(2.0f, 2.0f, 2.0f));
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
    
    if (m_renderItemSuelo) {
        m_renderItemSuelo->release(); // Deregistra y destruye el item
        m_renderItemSuelo = nullptr;
    }
    if (m_renderItemSosten) {
        m_renderItemSosten->release(); // Deregistra y destruye el item
        m_renderItemSosten = nullptr;
    }
    if (m_renderItemDiana) {
        m_renderItemDiana->release(); // Deregistra y destruye el item
        m_renderItemDiana = nullptr;
    }
    if (m_renderItemDianaCentro) {
        m_renderItemDianaCentro->release(); // Deregistra y destruye el item
        m_renderItemDianaCentro = nullptr;
    }
}