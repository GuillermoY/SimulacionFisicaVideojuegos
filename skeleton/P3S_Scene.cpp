#pragma once

#include "P3S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P3S_Scene::init() {
    //Opcional

    physx::PxShape* shapeSuelo = CreateShape(physx::PxBoxGeometry(100.0f, 2.0f, 100.0f));
    m_transformSuelo = physx::PxTransform(physx::PxVec3(0.0f, -30.0f, 0.0f));
    // Se registra el RenderItem exactamente como en la plantilla original
    m_renderItemSuelo = new RenderItem(shapeSuelo, &m_transformSuelo, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    //P3
    

    //P2
    // Fuego: partículas que ascienden desde un punto
    sistemas.push_back(
        new ParticleSystem(
            10.0f, 30.0f,
            0.0f,
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(-15.0f, -10.0f, 0.0f),
            Vector4(1.0f, 0.25f, 0.0f, 1.0f)
        )
    );

    // Niebla: partículas lentas y con poca variación, sin influencia de la gravedad
    sistemas.push_back(
        new ParticleSystem(
            0.0f, 1.0f,
            { 50.0f ,50.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            Vector4(0.75f, 0.80f, 0.85f, 0.25f),
            0
        )
    );

    // Lluvia: partículas que caen hacia abajo
    sistemas.push_back(
        new ParticleSystem(
            20.0f, 30.0f,
            0.0f,
            Vector3D(-1.0f, 0.1f, 0.0f),
            Vector3D(-15.0f, -10.0f, 0.0f),
            Vector4(0.2f, 0.5f, 1.0f, 1.0f)
        )
    );
}

void P3S_Scene::update(double dt) {
    // Lógica/Integración del alumno (por ejemplo, movimiento simple)
    for (Particle* part : particulas)
    {
        part->integrate(dt);
    }
    for (int i = 0; i < sistemas.size(); ++i)
    {
        if (sistemas[i]->isOn())
            sistemas[i]->integrate(dt);
    }
}

void P3S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }
    //P2
    if (key == 'f')
    {
        sistemas[0]->switchState();
    }
    else if (key == 'n')
    {
        sistemas[1]->switchState();
    }
    else if (key == 'l')
    {
        sistemas[2]->switchState();
    }
    //P3
    if (key == '+')
    {
        cout << "mas masa" << "\n";
        for (int i = 0; i < sistemas.size(); ++i)
        {
            if (sistemas[i]->isOn())
                sistemas[i]->changeMass(0.01f);
        }
    }
    else if (key == '-')
    {
        cout << "menos masa" << "\n";
        for (int i = 0; i < sistemas.size(); ++i)
        {
            if (sistemas[i]->isOn())
                sistemas[i]->changeMass(-0.01f);
        }
    }
}

void P3S_Scene::cleanup() {
    for (Particle* part : particulas)
    {
        delete part;
    }
    particulas.clear();

    if (m_renderItemSuelo) {
        m_renderItemSuelo->release(); // Deregistra y destruye el item
        m_renderItemSuelo = nullptr;
    }
    for (ParticleSystem* partS : sistemas)
    {
        delete partS;
    }
    sistemas.clear();

    //delete generador;
}