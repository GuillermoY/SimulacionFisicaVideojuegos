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
    //P1.2 Opcional
    physx::PxTransform m_transform;
    RenderItem* m_renderItem{ nullptr };

    physx::PxTransform m_transformSuelo;
    RenderItem* m_renderItemSuelo{ nullptr };

    physx::PxTransform m_transformSosten;
    RenderItem* m_renderItemSosten{ nullptr };

    physx::PxTransform m_transformDiana;
    RenderItem* m_renderItemDiana{ nullptr };

    physx::PxTransform m_transformDianaCentro;
    RenderItem* m_renderItemDianaCentro{ nullptr };

    //P1.1
    Particle* m_renderParticle1{ nullptr };

    //P1.2
    float mReal = 0.005f;
    float mSim=0;

    float vReal = 250.0f;
    float vSim=80.0f;
    float gSim=0;
    float gReal = -9.81f;
    vector<Particle*> particulas;

};