#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

#include "Model.h"
#include "GameObject.h"
#include <string>
#include <iostream>
class Player : public GameObject
{
public:

    explicit Player(std::string model, Vector3D pos, float rotY, float scale) :
        GameObject(model, pos, rotY, scale) {
    };
    ~Player() {};

    void update(float dt) override;
    void checkPressedInputs(unsigned char key);
    void checkSpecialPressedInputs(int key);
    void checkUpInputs(unsigned char key);
    void checkSpecialUpInputs(int key);

    float getSpeed() { return currSpeed; };
    
    physx::PxTransform getTransform() { return m_Transform; };
private:
    bool keys[256] = {false};
    bool specialKeys[256] = { false };

    void move(float dt);

    const float RUN_SPEED = 5;
    const float MAX_SPEED = 30;
    const float TURN_SPEED = 3;
    const float TIMETOMAX_SPEED = 30000;

    float currtime = 0;
    float currSpeed = RUN_SPEED;
    float speedIncrease = 0.3;
    float currTurnSpeed = 0;

    float maxRot = 0.4f;
};