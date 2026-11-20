#include "SceneManager.h"
#include "P0S_Scene.h"
#include "P1S_Scene.h"
#include "P2S_Scene.h"
#include "P3S_Scene.h"
#include "P4S_Scene.h"
#include "P5S_Scene.h"
#include "PTest_Scene.h"
#include <iostream>

// Solicita el cambio de escena. Si el nombre no está registrado, se escribe
// un error en stderr y la solicitud se ignora.
void SceneManager::changeScene(const std::string& name) {
    if (m_factories.find(name) == m_factories.end()) {
        std::cerr << "[SceneManager Error] Escena no registrada: " << name << "\n";
        return;
    }

    m_pendingSceneName = name;
    m_hasPendingChange = true;
}

// Realiza la transición a la escena pendiente: limpia la escena actual,
// crea la nueva usando la fábrica y la inicializa.
void SceneManager::applyPendingSceneChange() {
    if (!m_hasPendingChange) return;

    if (m_currentScene) {
        m_currentScene->cleanup();
    }


    m_currentScene = m_factories[m_pendingSceneName]();
    if (m_currentScene) {
        m_currentScene->init();
        std::cout << "[SceneManager] Escena cargada: " << m_pendingSceneName << "\n";
    }

    m_hasPendingChange = false;
    m_pendingSceneName.clear();
}

// Llamada por frame: aplica el cambio pendiente antes de actualizar la escena
// para evitar modificar la escena mientras está siendo usada.
void SceneManager::update(double dt) {
    if (m_hasPendingChange) {
        applyPendingSceneChange();
    }

    if (m_currentScene) {
        m_currentScene->update(dt);
    }
}

void SceneManager::render() {
    if (m_currentScene) {
        m_currentScene->render();
    }
}

// Reenvía eventos de teclado a la escena actual. Mantener este método simple
// permite centralizar atajos globales si se desea en el futuro.
void SceneManager::keyPress(unsigned char key, const physx::PxTransform& cameraTransform) {
    // Teclas globales de navegación entre prácticas
    if (key == 'V') {
        changeScene("EscenaVacia");
        return; // Consumimos el evento para que no interfiera con la escena
    }
        
    if (key == '0') {
        changeScene("P0S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == '1') {
        changeScene("P1S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == '2') {
        changeScene("P2S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == '3') {
        changeScene("P3S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == '4') {
        changeScene("P4S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == '5') {
        changeScene("P5S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    else if (key == 'g') {
        changeScene("PTest_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }


    // Si no es una tecla de navegación global, se la pasamos a la escena activa
    if (m_currentScene != nullptr) {
        m_currentScene->keyPress(key, cameraTransform);
    }
}

void SceneManager::keyUp(unsigned char key, const physx::PxTransform& cameraTransform) {
    // Si no es una tecla de navegación global, se la pasamos a la escena activa
    if (m_currentScene != nullptr) {
        m_currentScene->keyUp(key, cameraTransform);
    }
}

void SceneManager::specialKeyPress(int key, const physx::PxTransform& cameraTransform) {
    if (m_currentScene != nullptr) {
        m_currentScene->specialKeyPress(key, cameraTransform);
    }
}

void SceneManager::specialKeyUp(int key, const physx::PxTransform& cameraTransform) {
    if (m_currentScene != nullptr) {
        m_currentScene->specialKeyUp(key, cameraTransform);
    }
}