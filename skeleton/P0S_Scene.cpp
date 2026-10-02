#pragma once

#include "P0S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
using namespace std;

void P0S_Scene::init() {
    //P0

    // Ejemplo: Creación de una esfera usando las utilidades de render existentes
    //physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
    //m_transform = physx::PxTransform(physx::PxVec3(10.0f, 0.0f, 0.0f));

    //// Se registra el RenderItem exactamente como en la plantilla original
    //m_renderItem = new RenderItem(shape, &m_transform, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

    //physx::PxShape* shape1 = CreateShape(physx::PxSphereGeometry(2.0f));
    //m_transform1 = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

    //// Se registra el RenderItem exactamente como en la plantilla original
    //m_renderItem1 = new RenderItem(shape1, &m_transform1, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

    //physx::PxShape* shape2 = CreateShape(physx::PxSphereGeometry(2.0f));
    //m_transform2 = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 10.0f));

    //// Se registra el RenderItem exactamente como en la plantilla original
    //m_renderItem2 = new RenderItem(shape2, &m_transform2, Vector4(0.0f, 0.0f, 1.0f, 1.0f));

    //physx::PxShape* shape3 = CreateShape(physx::PxSphereGeometry(2.0f));
    //m_transform3 = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));

    //// Se registra el RenderItem exactamente como en la plantilla original
    //m_renderItem3 = new RenderItem(shape3, &m_transform3, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    //P1.1

    //physx::PxShape* shape3 = CreateShape(physx::PxSphereGeometry(2.0f));
    m_renderParticle1 = new Particle(Vector3(0, 0, 0), Vector3(1, 0, 0), 1);
}

void P0S_Scene::update(double dt)  {
    // Lógica/Integración del alumno (por ejemplo, movimiento simple)
    m_renderParticle1->integrate(dt);
}

void P0S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera)  {
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }
}

void P0S_Scene::cleanup() {
    //P0
    if (m_renderItem) {
        m_renderItem->release(); // Deregistra y destruye el item
        m_renderItem = nullptr;
    }
    if (m_renderItem1) {
        m_renderItem1->release(); // Deregistra y destruye el item
        m_renderItem1 = nullptr;
    }
    if (m_renderItem2) {
        m_renderItem2->release(); // Deregistra y destruye el item
        m_renderItem2 = nullptr;
    }
    if (m_renderItem3) {
        m_renderItem3->release(); // Deregistra y destruye el item
        m_renderItem3 = nullptr;
    }
}