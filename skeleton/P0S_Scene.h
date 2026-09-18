#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include <vector>

class P0S_Scene : public Scene {
public:
    explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

    void init() override {
        // Ejemplo: Creación de una esfera usando las utilidades de render existentes
        physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform = physx::PxTransform(physx::PxVec3(10.0f, 0.0f, 0.0f));

        // Se registra el RenderItem exactamente como en la plantilla original
        m_renderItem = new RenderItem(shape, &m_transform, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

        physx::PxShape* shape1 = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform1 = physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f));

        // Se registra el RenderItem exactamente como en la plantilla original
        m_renderItem1 = new RenderItem(shape1, &m_transform1, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

        physx::PxShape* shape2 = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform2 = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 10.0f));

        // Se registra el RenderItem exactamente como en la plantilla original
        m_renderItem2 = new RenderItem(shape2, &m_transform2, Vector4(0.0f, 0.0f, 1.0f, 1.0f));

        physx::PxShape* shape3 = CreateShape(physx::PxSphereGeometry(2.0f));
        m_transform3 = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));

        // Se registra el RenderItem exactamente como en la plantilla original
        m_renderItem3 = new RenderItem(shape3, &m_transform3, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    void update(double dt) override {
        // Lógica/Integración del alumno (por ejemplo, movimiento simple)
        //m_transform.p.y -= static_cast<float>(9.8 * dt);
    }

    void keyPress(unsigned char key, const physx::PxTransform& camera) override {
        if (key == 'r' || key == 'R') {
            m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
        }
    }

    void cleanup() override {
        if (m_renderItem) {
            m_renderItem->release(); // Deregistra y destruye el item
            m_renderItem = nullptr;
        }
        if (m_renderItem1) {
            m_renderItem1->release(); // Deregistra y destruye el item
            m_renderItem1 = nullptr;
        }
        if (m_renderItem2) {
            m_renderItem2->release(); // Deregistra y destruye el item
            m_renderItem2 = nullptr;
        }
        if (m_renderItem3) {
            m_renderItem3->release(); // Deregistra y destruye el item
            m_renderItem3 = nullptr;
        }
    }

private:
    physx::PxTransform m_transform;
    RenderItem* m_renderItem{ nullptr };
    physx::PxTransform m_transform1;
    RenderItem* m_renderItem1{ nullptr };
    physx::PxTransform m_transform2;
    RenderItem* m_renderItem2{ nullptr };
    physx::PxTransform m_transform3;
    RenderItem* m_renderItem3{ nullptr };
};