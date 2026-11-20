#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ForceGenerator.h"
#include <iostream>
#include <vector>
#include "GravForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"

using namespace std;


class P4S_Scene : public Scene {
public:
    explicit P4S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    //P1.2 Opcional
    physx::PxTransform m_transform;
    // P2
    physx::PxTransform m_transformPart;
    vector<Particle*> particulas;
    vector<ParticleSystem*> sistemas;

    // P3
    vector<ForceGenerator*> generadores;
    int generadorActual = -1;

    WindForceGenerator* tempForce = nullptr;
    SpringForceGenerator* springForce1 = nullptr;
    SpringForceGenerator* springForce2 = nullptr;
    float windTime = 0;
    float endWindTime = 3;
    bool windTimeStarted = false;

    int typeVel = 0;
};