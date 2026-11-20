#include "PTest_Scene.h"
#include <string>
void PTest_Scene::init()
{
    cam = GetCamera();
    if (cam)
        cam->setMove(DEBUG);

    m_testModel = new Model();
    m_testModel->Load("Models/Untitled.obj");
    m_modelTransform = physx::PxTransform(
        physx::PxVec3(0.0f, 1.0f, 0.0f)
    );
    m_modelRenderItem = new ModelRenderItem(
        m_testModel,
        &m_modelTransform,
        physx::PxVec3(10.0f)
    );

    player = new Player(std::string("Models/Charact.obj"), { 0,10,-10 }, 0, 0.5f);
    layout = new LayoutGenerator(player);

    Snippets::initCustomFont("Fonts/Oxblood.ttf", 24.0f);
}

void PTest_Scene::render()
{
    Snippets::drawCustomText("Hola me llamo Guille", 20.0f, 40.0f, {1,0,1});
}

void PTest_Scene::update(double dt)
{
    player->update(dt);
    if (!DEBUG)
        cam->setTransform(player->getTransform(), 5.0f, 4.0f, dt);
    layout->update(dt);
    //updateTerrain(deltaTime);
}

void PTest_Scene::keyPress(unsigned char key, const physx::PxTransform& camera)
{
    //std::cout << "Tecla recibida: " << key << std::endl;
    if (key == '.')
        changeDebug();

    player->checkPressedInputs(key);
}
void PTest_Scene::specialKeyPress(int key, const physx::PxTransform& camera)
{
    //std::cout << "Tecla recibida: " << key << std::endl;
    player->checkSpecialPressedInputs(key);
}

void PTest_Scene::keyUp(unsigned char key, const physx::PxTransform& camera)
{
    player->checkUpInputs(key);
}

void PTest_Scene::specialKeyUp(int key, const physx::PxTransform& camera)
{
    player->checkSpecialUpInputs(key);
}

//void PTest_Scene::updateTerrain(float dt)
//{
//    float movement = m_speed * dt;
//
//    for (size_t i = 0; i < m_terrain.size(); i++)
//    {
//        TerrainSegment& segment = m_terrain[i];
//
//        float newZ = segment.z - movement;
//
//        while (newZ < -m_segmentLength)
//        {
//            newZ += m_segmentLength * m_numSegments;
//        }
//
//        moveSegment(segment, newZ);
//    }
//}

void PTest_Scene::cleanup()
{
    if (GetCamera())
        GetCamera()->setMove(true);

    delete m_testModel;
    m_testModel = nullptr;

    delete m_modelRenderItem;
    m_modelRenderItem = nullptr;

    delete player;
    player = nullptr;

    delete layout;
    layout = nullptr;
}