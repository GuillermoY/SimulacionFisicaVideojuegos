#pragma once

#include "P5S_Scene.h"
#include "RenderUtils.hpp"
#include <iostream>
#include <vector>
#include "Vector3D.h"
using namespace std;

void P5S_Scene::init() {

}

void P5S_Scene::update(double dt) {
   
}

void P5S_Scene::keyPress(unsigned char key, const physx::PxTransform& camera) {
    if (key == 'r' || key == 'R') {
        m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
    }

    //P4
    if (key == 'p' && !windTimeStarted)
    {
        sistemas[0]->addGeneratorType(tempForce);
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

        cout << "VelType: " << typeVel << "\n";
    }

    if (key == '+')
    {
        cout << "mas k" << "\n";
        springForce1->setK(springForce1->getK() + 15);
        springForce2->setK(springForce2->getK() + 15);
    }
    else if (key == '-')
    {
        cout << "menos k" << "\n";
        if (springForce1->getK() > 15)
        {
            springForce1->setK(springForce1->getK() - 15);
            springForce2->setK(springForce2->getK() - 15);
        }
    }
}

void P5S_Scene::cleanup() {
   
}