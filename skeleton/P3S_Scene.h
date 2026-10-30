#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ForceGenerator.h"
#include <iostream>
#include <vector>
#include "GravForceGenerator.h"

using namespace std;


class P3S_Scene : public Scene {
public:
    explicit P3S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    //P1.2 Opcional
    physx::PxTransform m_transform;

    physx::PxTransform m_transformSuelo;
    RenderItem* m_renderItemSuelo{ nullptr };

    //P1.2
    float mReal = 0.005f;
    float mSim = 0;

    float vReal = 250.0f;
    float vSim = 80.0f;
    float gSim = 0;
    float gReal = -10.0f;
    vector<Particle*> particulas;

    // P2
    physx::PxTransform m_transformPart;
    vector<ParticleSystem*> sistemas;

    // P3
    ForceGenerator* generador;
};