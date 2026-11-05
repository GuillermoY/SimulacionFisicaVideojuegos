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

    // FUERZAS
    // Esta será para gravedad
    Particle* p1 = new Particle({}, {}, 40.0f, {}, { 0.75f, 0.80f, 0.85f, 0.25f }, PxSphereGeometry(1.0f), false);
    particulas.push_back(p1);
    sistemas.push_back(
        new ParticleSystem(
            2.0f, 4.0f,
            { 50.0f ,50.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            p1
        )
    );    
    // Esta será para Viento
    Particle* p2 = new Particle({}, {}, 5.0f, {}, { 0.75f, 0.80f, 0.85f, 0.25f }, PxSphereGeometry(1.0f), false);
    particulas.push_back(p2);
    sistemas.push_back(
        new ParticleSystem(
            0.0f, 1.0f, 
            { 50.0f ,30.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            p2
        )
    );    
    sistemas[1]->defineLimits({ 1000.0f,1000.0f,1000.0f },
        { -1000.0f,-1000.0f,-1000.0f });
    // Esta será para Torbellino
    Particle* p3 = new Particle({}, {}, 5.0f, {}, { 0.1f, 0.1f, 0.85f, 0.6f }, PxSphereGeometry(1.0f), false);
    particulas.push_back(p3); 
    sistemas.push_back(
        new ParticleSystem(
            0.0f, 1.0f, 
            { 50.0f ,10.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            p3
        )
    );    
    sistemas[2]->defineLimits({ 1000.0f,1000.0f,1000.0f },
        { -1000.0f,-1000.0f,-1000.0f });
    // Esta será para Explosión
    Particle* p4 = new Particle({}, {}, 1.0f, {}, { 0.8f, 0.8f, 0.1f, 0.8f }, PxSphereGeometry(1.0f), false);
    particulas.push_back(p4);
    sistemas.push_back(
        new ParticleSystem(
            0.0f, 1.0f, 
            { 50.0f ,50.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            p4
        )
    );
    sistemas[3]->setNExplParts(1000);
    sistemas[3]->defineLimits({ 1000.0f,1000.0f,1000.0f },
                                { -1000.0f,-1000.0f,-1000.0f });

    // Configuración de fuerzas 0 gravedad 1 viento 2 torbellino 3 explosión
    generadores.push_back(new GravForceGenerator(Vector3D(0, -9.8f, 0)));
    generadores.push_back(new WindForceGenerator(
        Vector3D(15, 0, 0),
        2.0f,
        Vector3D(-50, -30, -50),
        Vector3D(50, 50, 50)));
    generadores.push_back(new WhirlWindForceGenerator(
        Vector3D(10, 0, 0),
        1.5f,
        Vector3D(-50, -30, -50),
        Vector3D(50, 50, 50),
        Vector3D(10, 0, 0)));
    generadores.push_back(new ExplosionForceGenerator(
        50.0f,
        600.0f,
        2.0f,
        Vector3D(30, 30, 30)));

    sistemas[0]->addGeneratorType(
        generadores[0],
        ParticleSystem::ForceType::GRAVITY
    );

    sistemas[1]->addGeneratorType(
        generadores[1],
        ParticleSystem::ForceType::WIND
    );

    sistemas[2]->addGeneratorType(
        generadores[2],
        ParticleSystem::ForceType::WHIRLWIND
    );

    sistemas[3]->addGeneratorType(
        generadores[3],
        ParticleSystem::ForceType::EXPLOSION
    );
    sistemas[3]->addGeneratorType(
        generadores[2],
        ParticleSystem::ForceType::WHIRLWIND
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
    if (key == '>')
    {
        if (generadorActual+1 < sistemas.size())
            generadorActual++;

        cout << generadorActual << "\n";
        for (int i = 0; i < sistemas.size(); ++i)
        {
            if (sistemas[i]->isOn())
                sistemas[i]->switchState();
        }
        sistemas[generadorActual]->switchState();
    }
    else if (key == '<')
    {
        if (generadorActual > 0)
            generadorActual--;
        else
            generadorActual = 0;

        for (int i = 0; i < sistemas.size(); ++i)
        {
            if (sistemas[i]->isOn())
                sistemas[i]->switchState();
        }
        sistemas[generadorActual]->switchState();

        cout << generadorActual << "\n";
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

    for (ForceGenerator* generador : generadores)
    {
        delete generador;
    }
}