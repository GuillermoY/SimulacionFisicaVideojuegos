#pragma once

#include "P5S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P5S_Scene::init() {
    //physx::PxShape* shapeSuelo = CreateShape(physx::PxBoxGeometry(50.0f, 50.0f, 50.0f));
    //m_transformAgua = physx::PxTransform(physx::PxVec3(0.0f, -30.0f, 0.0f));
    //// Se registra el RenderItem exactamente como en la plantilla original
    //m_renderItemAgua = new RenderItem(shapeSuelo, &m_transformAgua, Vector4(0.7f, 0.7f, 1.0f, 0.1f));


// Sistema visual del agua
    sistemas.push_back(
        new ParticleSystem(
            0, 0,
            { 0,0,0 },
            Vector3D(0, 0, 0),
            Vector3D(0, 0, 0),
            { 0,0,1,1 },
            physx::PxBoxGeometry(50.0f, 0.1f, 50.0f),
            ParticleSystem::ForceType::NONE,
            false
        )
    );

    Particle* water = new Particle(
        { 0,0,0 },
        { 0,0,0 },
        0.0f,
        { 0,0,0 },
        { 0,0.8f,1.0f,0.5f },
        physx::PxBoxGeometry(50.0f, 0.1f, 50.0f)
    );

    water->changeDeletability(false);
    sistemas.back()->addParticle(water);
    sistemas.back()->switchState();


    // Sistema del objeto flotante
    sistemas.push_back(
        new ParticleSystem(
            0, 0,
            { 0,0,0 },
            Vector3D(0, 0, 0),
            Vector3D(0, 0, 0),
            { 1,0,0,1 },
            physx::PxBoxGeometry(2, 2, 2),
            ParticleSystem::ForceType::NONE,
            false
        )
    );

    ParticleSystem* floatingSystem = sistemas.back();

    // Gravedad
    GravForceGenerator* gravityFloat = new GravForceGenerator(Vector3D(0, -9.8f, 0));

    generadores.push_back(gravityFloat);

    // Cubo de flotación
    // Geometría: semiextensiones 2, altura total 4.
    // Volumen: 4 * 4 * 4 = 64.
    floatingObject = new Particle(
        { 0,3,0 },
        { 0,0,0 },
        32000.0f,
        { 0,0,0 },
        { 1,0,0,1 },
        physx::PxBoxGeometry(2, 2, 2)
    );

    floatingObject->changeDeletability(false);

    // Generador de empuje
    buoyancyForce = new BuoyancyForceGenerator(
        4.0,     // Altura del agua
        64.0f,    // Volumen del objeto
        1000.0f,  // Densidad del agua
        water
    );

    floatingSystem->addParticle(floatingObject);
    floatingSystem->addGeneratorType(gravityFloat);
    floatingSystem->addGeneratorType(buoyancyForce);

    generadores.push_back(buoyancyForce);

    floatingSystem->switchState();
}

void P5S_Scene::update(double dt) {
    for (int i = 0; i < sistemas.size(); ++i)
    {
        if (sistemas[i]->isOn())
            sistemas[i]->integrate(dt);
    }
}

void P5S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    // volumen
    if (key == 'v')
    {
        buoyancyForce->setVolume(buoyancyForce->getVolume() + 5.0);

        cout << "Volumen: " << buoyancyForce->getVolume() << "\n";
    }

    if (key == 'V')
    {
        buoyancyForce->setVolume(max(0.1, buoyancyForce->getVolume() - 5.0)
        );

        cout << "Volumen: " << buoyancyForce->getVolume() << "\n";
    }
    if (key == 'm')
    {
        floatingObject->setMass(floatingObject->getMass() + 1000.0f);
        cout << "Masa: " << floatingObject->getMass()<<"\n";
    }

    if (key == 'M')
    {
        floatingObject->setMass(max(1.0f, floatingObject->getMass() - 1000.0f));

        cout << "Masa: " << floatingObject->getMass() << "\n";

    }
}

void P5S_Scene::cleanup() {

    for (ParticleSystem* partS : sistemas)
    {
        delete partS;
    }
    sistemas.clear();

    for (ForceGenerator* generador : generadores)
    {
        if (generador)
            delete generador;
    }

    particulas.clear();
    generadores.clear();
}