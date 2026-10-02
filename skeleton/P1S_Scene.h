#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include <iostream>
#include <vector>

using namespace std;


class P1S_Scene : public Scene {
public:
    explicit P1S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

    void escaladoSimulado();

    void shoot(unsigned char key, const physx::PxTransform& camera);
private:
    //P0
    physx::PxTransform m_transform;
    RenderItem* m_renderItem{ nullptr };

    //P1.1
    Particle* m_renderParticle1{ nullptr };

    //P1.2
    float mReal = 0.005f;
    float mSim=0;

    float vReal = 250.0f;
    float vSim=80.0f;
    float gSim=0;
    float gReal = -9000.81f;
    vector<Particle*> particulas;

};