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
        generadores.push_back(new GravForceGenerator(Vector3D(0, -1.0f, 0)));

        sistemas[0]->switchState();
        sistemas[0]->addGeneratorType(generadores[0]);

        Particle* p1 = new Particle({ -10.0,10.0,0.0 }, { 0,0,0 });
        p1->setColor({ 1, 0 ,0,1 });
        Particle* p2 = new Particle({ 10.0,10.0,0.0 }, { 0,0,0 });
        p2->setColor({ 0, 0 ,1,1 });

        p1->changeDeletability(false);
        p2->changeDeletability(false);

        springForce1 = new SpringForceGenerator(1, 10, p2);
        springForce2 = new SpringForceGenerator(1, 10, p1);

        sistemas[0]->addParticle(p1);
        sistemas[0]->addParticle(p2);

        sistemas[0]->addGeneratorType(springForce1);
        sistemas[0]->addGeneratorType(springForce2);

        generadores.push_back(springForce1);
        generadores.push_back(springForce2);

        // Otro muelle con un lado fijo
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
        Particle* p3 = new Particle({ -10.0,20.0,0 }, { 0,0,0 }, 0.85f);
        p3->setColor({ 0, 1 ,0,1 });

        p3->changeDeletability(false);
        AnchoredSpringFG* f3 = new AnchoredSpringFG(1, 10, { 10.0, 20.0, 0.0 });
        sistemas[1]->addGeneratorType(f3);
        sistemas[1]->addGeneratorType(generadores[0]);

        sistemas[1]->addParticle(p3);
        sistemas[1]->switchState();
        generadores.push_back(f3);

        // Viento temporal

        tempForce = new WindForceGenerator(
            Vector3D(50, 0, 0),
            2.0f,
            Vector3D(-50, -30, -50),
            Vector3D(50, 50, 50));
        generadores.push_back(tempForce);
    }

    void P4S_Scene::update(double dt) {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
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
                for (ParticleSystem* sistema : sistemas)
                    sistema->removeGeneratorType(tempForce);
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
            for (ParticleSystem* sistema : sistemas)
                sistema->addGeneratorType(tempForce);

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
            for (ForceGenerator* f : generadores)
            {
                SpringForceGenerator* spring =
                    dynamic_cast<SpringForceGenerator*>(f);

                if (spring)
                    spring->setK(spring->getK() + 15);
            }
        }
        else if (key == '-')
        {
            cout << "menos k" << "\n";
            for (ForceGenerator* f : generadores)
            {
                SpringForceGenerator* spring =
                    dynamic_cast<SpringForceGenerator*>(f);

                if (spring && spring->getK() > 15)
                    spring->setK(spring->getK() - 15);
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