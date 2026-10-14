#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
using namespace std;

class P0S_Scene : public Scene {
public:
    explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override;

    void update(double dt) override;

    void keyPress(unsigned char key, const physx::PxTransform& camera) override;

    void cleanup() override;

private:
    //P0
    physx::PxTransform m_transform;
    RenderItem* m_renderItem{ nullptr };
    physx::PxTransform m_transform1;
    RenderItem* m_renderItem1{ nullptr };
    physx::PxTransform m_transform2;
    RenderItem* m_renderItem2{ nullptr };
    physx::PxTransform m_transform3;
    RenderItem* m_renderItem3{ nullptr };

};