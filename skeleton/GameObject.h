#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

#include "Model.h"
#include <string>
#include <iostream>
class GameObject
{
public:

    GameObject(std::string model, Vector3D pos, float rotY, float scale) {
        m_mainModel = new Model();
        m_mainModel->Load(model);
        m_Yrotation = rotY;

        float radianes = rotY * (PxPi / 180.0f);
        physx::PxQuat rotacion(radianes, physx::PxVec3(0.0f, 1.0f, 0.0f));
        physx::PxVec3 posicion(pos.x,pos.y,pos.z);
        m_Transform = physx::PxTransform(posicion,rotacion);

        m_renderItem = new ModelRenderItem(
            m_mainModel,
            &m_Transform,
            physx::PxVec3(scale)
        );
    }
    virtual ~GameObject() {
        delete m_renderItem;
        m_renderItem = nullptr;

        delete m_mainModel;
        m_mainModel = nullptr;
    };

    virtual void update(float dt) {};

    physx::PxTransform getTransform() { return m_Transform; };
    void setTransform(physx::PxTransform transform) { m_Transform = transform; };

protected:

    physx::PxTransform m_Transform;
    float m_Yrotation;
    Model* m_mainModel;
    ModelRenderItem* m_renderItem = nullptr;
};