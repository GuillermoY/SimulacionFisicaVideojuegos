#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ForceGenerator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "GravForceGenerator.h"
#include "SpringForceGenerator.h"
#include "BuoyancyForceGenerator.h"

using namespace std;


class P5S_Scene : public Scene {
public:
    explicit P5S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:

    BuoyancyForceGenerator* buoyancyForce = nullptr;
    Particle* floatingObject = nullptr;

    vector<Particle*> particulas;
    vector<ParticleSystem*> sistemas;
    vector<ForceGenerator*> generadores;
};