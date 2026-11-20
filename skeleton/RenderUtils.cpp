#include <vector>
#include <algorithm>

#include "PxPhysicsAPI.h"

#include "core.hpp"
#include "RenderUtils.hpp"
#include "SceneManager.h"

using namespace physx;

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);	
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);
extern void specialKeyPress(int key, const PxTransform& camera);
extern void keyUp(unsigned char key, const PxTransform& camera);
extern void specialKeyUp(int key, const PxTransform& camera);
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

std::vector<const RenderItem*> gRenderItems;
std::vector<const ModelRenderItem*> gModelRenderItems; // Para los modelos 3D

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

bool keys[256] = { false };
bool specialKeys[256] = { false };

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera*	sCamera;

void motionCallback(int x, int y)
{
	sCamera->handleMotion(x, y);
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    keys[key] = true;

    if (!sCamera->handleKey(key, x, y))
        keyPress(key, sCamera->getTransform());
}

void keyboardUpCallback(unsigned char key, int x, int y)
{
	keys[key] = false;

	if (!sCamera->handleKey(key, x, y))
		keyUp(key, sCamera->getTransform());
}

void specialKeyboardCallback(int key, int x, int y)
{
	if (key < 256)
		specialKeys[key] = true;

	if (!sCamera->handleKey(key, x, y))
		specialKeyPress(key, sCamera->getTransform());
}

void specialKeyboardUpCallback(int key, int x, int y)
{
	if (key < 256)
		specialKeys[key] = false;

	if (!sCamera->handleKey(key, x, y))
		specialKeyUp(key, sCamera->getTransform());
}

void mouseCallback(int button, int state, int x, int y)
{
	sCamera->handleMouse(button, state, x, y);
}

void idleCallback()
{
	glutPostRedisplay();
}

float stepTime = 0.0f;
//#define FIXED_STEP

void renderCallback()
{
	double t = GetCounter();
#ifdef FIXED_STEP
	if (t < (1.0f / 30.0f))
	{
		fprintf(stderr, "Time: %f\n", stepTime);
		stepTime += t;
	}
	else
		stepTime = 1.0f / 30.0f;

	if (stepTime >= (1.0f / 30.0f))
	{
		stepPhysics(true, stepTime);
		stepTime = 0.0f;
	}
#else
	stepPhysics(true, t);
#endif

	startRender(sCamera->getEye(), sCamera->getDir());

	//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
	for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
	{
		const RenderItem* obj = (*it);
		auto objTransform = obj->transform;
		if (!objTransform)
		{
			auto actor = obj->actor;
			if (actor)
			{
				renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
		}
		renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
	}

	for (const ModelRenderItem* obj : gModelRenderItems)
	{
		if (!obj || !obj->model || !obj->model->IsLoaded())
			continue;

		glPushMatrix();

		physx::PxTransform transform = obj->transform ? *obj->transform : physx::PxTransform(physx::PxIdentity);

		physx::PxMat44 matrix(transform);
		// reinterpret_cast convierte un valor de puntero nulo al valor de puntero nulo del tipo de destino.
		// glMultMatrixf() espera un puntero a float
		glMultMatrixf(reinterpret_cast<const float*>(&matrix));

		glScalef(obj->scale.x,
			obj->scale.y,
			obj->scale.z);

		obj->model->Draw();

		glPopMatrix();
	}
	//PxScene* scene;
	//PxGetPhysics().getScenes(&scene, 1);
	//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//if (nbActors)
	//{
	//	std::vector<PxRigidActor*> actors(nbActors);
	//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
	//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//}
	SceneManager::instance().render();


	finishRender();
}


void exitCallback(void)
{
	delete sCamera;
	cleanupPhysics(true);
}
}


void RegisterModelRenderItem(const ModelRenderItem* item)
{
	if (item)
		gModelRenderItems.push_back(item);
}

void DeregisterModelRenderItem(const ModelRenderItem* item)
{
	auto it = std::find(gModelRenderItems.begin(), gModelRenderItems.end(), item);

	if (it != gModelRenderItems.end())
		gModelRenderItems.erase(it);
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	gRenderItems.erase(it);
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f,-0.2f,-0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos");
	setupDefaultRenderState();
	glEnable(GL_TEXTURE_2D);

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialKeyboardCallback);
	glutSpecialUpFunc(specialKeyboardUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	motionCallback(0,0);


	atexit(exitCallback);

	initPhysics(true);
	glutMainLoop();
}


double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}