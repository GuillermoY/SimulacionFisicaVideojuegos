#include <ctype.h>
#include "EscenaJoints.h"

EscenaJoints::EscenaJoints()
{
	_activa = false;
	_physics = nullptr;
	_scene = nullptr;
	_techo = nullptr;
	_brazo = nullptr;
	_bisagra = nullptr;
}

EscenaJoints::~EscenaJoints()
{
}

void EscenaJoints::init(PxPhysics* physics, PxScene* scene)
{
	_physics = physics;
	_scene = scene;
}

void EscenaJoints::update(float t)
{
	// La bisagra y la gravedad ya las gestiona PhysX solo cada gScene->simulate(): no hace falta
	// aplicar ninguna fuerza a mano aqui.
	PX_UNUSED(t);
}

void EscenaJoints::cleanup()
{
	destruir();
}

void EscenaJoints::keyPress(unsigned char key)
{
	if (toupper(key) != 'U') return;

	_activa = !_activa;
	if (_activa) crear();
	else destruir();
}

void EscenaJoints::crear()
{
	destruir();

	// Techo: un solido estatico que hace de punto de anclaje fijo de la bisagra.
	PxTransform poseTecho(Vector3(0.0f, 30.0f, -10.0f));
	PxShape* formaTecho = CreateShape(PxBoxGeometry(0.5f, 0.5f, 0.5f));
	PxRigidStatic* actorTecho = _physics->createRigidStatic(poseTecho);
	actorTecho->attachShape(*formaTecho);
	_scene->addActor(*actorTecho);
	_techo = new SolidoRigido(actorTecho, new RenderItem(formaTecho, actorTecho, Vector4(0.6f, 0.6f, 0.6f, 1.0f)), "techo");

	// Brazo: una caja dinamica alargada a lo largo del eje Z, con un extremo junto al techo.
	Vector3 mitadesBrazo(0.25f, 0.25f, 2.0f);
	PxTransform poseBrazo(poseTecho.p + Vector3(0.0f, 0.0f, mitadesBrazo.z));
	PxShape* formaBrazo = CreateShape(PxBoxGeometry(mitadesBrazo));
	PxRigidDynamic* actorBrazo = _physics->createRigidDynamic(poseBrazo);
	actorBrazo->attachShape(*formaBrazo);
	PxRigidBodyExt::updateMassAndInertia(*actorBrazo, 1.0f);
	_scene->addActor(*actorBrazo);
	_brazo = new SolidoRigido(actorBrazo, new RenderItem(formaBrazo, actorBrazo, Vector4(0.9f, 0.6f, 0.1f, 1.0f)), "brazo");

	// Bisagra (PxRevoluteJoint): el eje de giro es el eje X local de los dos frames, que aqui coincide
	// con el eje X del mundo (por eso no hace falta rotar los frames, solo desplazarlos). El punto de
	// giro es el propio techo (origen del actor) y, en el brazo, su extremo mas cercano al techo
	// (a -mitadesBrazo.z de su centro, ya que el brazo esta centrado en su propio origen).
	PxTransform frameTecho(Vector3(0.0f, 0.0f, 0.0f));
	PxTransform frameBrazo(Vector3(0.0f, 0.0f, -mitadesBrazo.z));
	_bisagra = PxRevoluteJointCreate(*_physics, actorTecho, frameTecho, actorBrazo, frameBrazo);
}

void EscenaJoints::destruir()
{
	if (_bisagra) { _bisagra->release(); _bisagra = nullptr; }
	delete _brazo; _brazo = nullptr;
	delete _techo; _techo = nullptr;
}
