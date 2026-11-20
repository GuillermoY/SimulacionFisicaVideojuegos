#pragma once

#include "PxPhysicsAPI.h"
#include "Scene.h"
#include "RenderUtils.hpp"
#include "Model.h"
#include "Player.h"
#include "GameObject.h"
#include "LayoutGenerator.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>

class PTest_Scene : public Scene
{
public:

    explicit PTest_Scene(std::string name): Scene(std::move(name)) {}

    void init() override;
    void update(double dt) override;
    void keyPress(unsigned char key, const physx::PxTransform& camera) override;
    void specialKeyPress(int key, const physx::PxTransform& camera) override;
    void keyUp(unsigned char key, const physx::PxTransform& camera) override;
    void specialKeyUp(int key, const physx::PxTransform& camera) override;
    void cleanup() override;
    void render() override;

private:
    void changeDebug() { DEBUG = !DEBUG; };

    float m_targetRotation = 0.0f;
    float m_currentRotation = 0.0f;

    float m_rotationSpeed = 5.0f;
    float m_maxRotation = 15.0f;

    bool DEBUG = false;

    Model* m_testModel = nullptr;
    ModelRenderItem* m_modelRenderItem = nullptr;
    physx::PxTransform m_modelTransform;

    Model* m_playerModel = nullptr;
    Player* player;
    Camera* cam;

    LayoutGenerator* layout;
};