#include "Player.h"
#include <cmath>

using namespace std;

void Player::update(float dt)
{
    move(dt);
    currtime = dt;
}

void Player::move(float dt)
{
    currTurnSpeed = 0;

    if (keys['a'] || keys['A'] ||
        specialKeys[GLUT_KEY_LEFT])
    {
        currTurnSpeed = TURN_SPEED;
    }
    if (keys['d'] || keys['D'] ||
        specialKeys[GLUT_KEY_RIGHT])
    {
        currTurnSpeed = -TURN_SPEED;
    }


    if (currTurnSpeed != 0)
    {
        m_Yrotation += currTurnSpeed * dt;
    }
    else
    {
        m_Yrotation += (m_Yrotation / 2) * -TURN_SPEED * dt;
    }

    if (m_Yrotation > maxRot)
        m_Yrotation = maxRot;
    if (m_Yrotation < -maxRot)
        m_Yrotation = -maxRot;

    m_Transform.p.x += currTurnSpeed * TURN_SPEED * dt;
    if (currtime < TIMETOMAX_SPEED)
    {
        currSpeed += speedIncrease * dt;
    }
    m_Transform.p.z += currSpeed * dt;
    m_Transform.q = physx::PxQuat(
        m_Yrotation -95 * physx::PxPi / 180.0f,
        physx::PxVec3(0, 1, 0)
    );
}

void Player::checkPressedInputs(unsigned char key)
{
    keys[key] = true;
}

void Player::checkUpInputs(unsigned char key)
{
    keys[key] = false;

    //switch (key)
    //{
    //case 'a':
    //case 'A':
    //case 'd':
    //case 'D':
    //    currTurnSpeed = 0;
    //    break;
    //}
}


void Player::checkSpecialPressedInputs(int key)
{
    specialKeys[key] = true;


    //switch (key)
    //{
    //    case GLUT_KEY_UP:
    //        currTurnSpeed = TURN_SPEED;
    //        break;

    //    case GLUT_KEY_RIGHT:
    //        currTurnSpeed = -TURN_SPEED;
    //        break;
    //}
}


void Player::checkSpecialUpInputs(int key)
{
    specialKeys[key] = false;


    //switch (key)
    //{
    //case GLUT_KEY_LEFT:
    //case GLUT_KEY_RIGHT:
    //    currTurnSpeed = 0;
    //    break;
    //}
}