#pragma once

#include "P4S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P4S_Scene::init() {
    // FUERZAS
    // Esta será para gravedad
    // Parte 1
    sistemas.push_back(
        new ParticleSystem(
            2.0f, 4.0f,
            { 50.0f ,50.0f, 50.0f },
            Vector3D(0.0f, 1.0f, 0.0f),
            Vector3D(0.0f, 40.0f, 0.0f),
            { 1.0f, 0.0f, 0.5f, 1.0f },
            physx::PxSphereGeometry(2.0f),
            ParticleSystem::ForceType::SPRING,
            false
        )
    );   
    generadores.push_back(new GravForceGenerator(Vector3D(0, -9.0f, 0)));

    sistemas[0]->switchState();
    sistemas[0]->addGeneratorType(generadores[0]);

    generadores.push_back(new GravForceGenerator(Vector3D(2, -50.0f, 0)));
    //Particle* p2 = new Particle({ 30.0,10.0,0.0 });
    //p2->changeDeletability(false);
    //p2->setMass(1.0f);
    //springForce1 = new SpringForceGenerator(1, 10, p2);
    //particulas.push_back(p2);
    //sistemas[0]->addParticle(p2);
    Particle* p1 = new Particle({ -30.0,10.0,0.0 });
    p1->changeDeletability(false);
    particulas.push_back(p1);
    sistemas[0]->addGeneratorType(springForce1);
    generadores.push_back(springForce1);
    sistemas[0]->addParticle(p1);

    springForce2 = new SpringForceGenerator(1, 10, p1);
    sistemas[0]->addGeneratorType(springForce2);
    generadores.push_back(springForce2);

    // Otro muelle con un lado fijo
    Particle* p3 = new Particle({ -10.0,20.0,0.0 });
    Particle* p4 = new Particle({ -10.0,10.0,0.0 });

    particulas.push_back(p3);
    p3->changeDeletability(false);
    p4->changeDeletability(false);
    AnchoredSpringFG* f3 = new AnchoredSpringFG(1, 10, { 10.0, 20.0, 0.0 });
    sistemas[0]->addGeneratorType(f3);
    sistemas[0]->addParticle(p3);
    generadores.push_back(f3);

    tempForce = new WindForceGenerator(
        Vector3D(50, 0, 0),
        2.0f,
        Vector3D(-50, -30, -50),
        Vector3D(50, 50, 50));
    generadores.push_back(tempForce);
}

void P4S_Scene::update(double dt) {
    // Lógica/Integración del alumno (por ejemplo, movimiento simple)
    //for (Particle* part : particulas)
    //{
    //    part->integrate(dt);
    //}
    for (int i = 0; i < sistemas.size(); ++i)
    {
        if (sistemas[i]->isOn())
            sistemas[i]->integrate(dt);
    }

    if (windTimeStarted)
    {
        windTime += dt;
        if (windTime >= endWindTime)
        {
            windTime = 0;
            sistemas[0]->removeGeneratorType(tempForce);
            windTimeStarted = false;
        }
    }
}

void P4S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }

    //P4
    if (key == 'p' && !windTimeStarted)
    {
        sistemas[0]->addGeneratorType(tempForce);
        windTimeStarted = true;
    }

    if (key == '>' || key == '<')
    {
        if (typeVel < 2 && key == '>')
            typeVel++;
        if (typeVel > 0 && key == '<')
            typeVel--;

        for (Particle* part : particulas)
        {
            if (part)
                part->setTypeVel(typeVel);
        }

        cout << "VelType: " << typeVel<<"\n";
    }

    if (key == '+')
    {
        cout << "mas k" << "\n";
        springForce1->setK(springForce1->getK() + 15);
        springForce2->setK(springForce2->getK() + 15);
    }
    else if (key == '-')
    {
        cout << "menos k" << "\n";
        if (springForce1->getK() > 15)
        {
            springForce1->setK(springForce1->getK() - 15);
            springForce2->setK(springForce2->getK() - 15);
        }
    }
}

void P4S_Scene::cleanup() {
    particulas.clear();

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
    generadores.clear();
}