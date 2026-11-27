#include "SistemaSolidoRigido.h"

SistemaSolidoRigido::SistemaSolidoRigido(PxPhysics* physics, PxScene* scene) : _u(-1.0f, 1.0f)
{
	_physics = physics;
	_scene = scene;

	_generando = false;
	_frecuencia = 1.0f;
	_tiempoDesdeUltimo = 0.0f;
	_densidad = 1.0f;

	_gravedadActiva = false;
	_vientoActivo = false;
	_torbellinoActivo = false;

	_origen = Vector3(0.0f, 20.0f, 0.0f);
	_velocidadMedia = Vector3(0.0f, 0.0f, 0.0f);
}

SistemaSolidoRigido::~SistemaSolidoRigido()
{
	for (auto s : _solidos) delete s;
	_solidos.clear();
}

void SistemaSolidoRigido::update(float t)
{
	_tiempoDesdeUltimo += t;
	if (_generando && _tiempoDesdeUltimo > _frecuencia) {
		generadorSR();
		_tiempoDesdeUltimo = 0.0f;
	}

	// Aplica todas las fuerzas registradas (gravedad, viento, torbellino) ANTES de que main.cpp
	// llame a gScene->simulate(), para que PhysX las tenga en cuenta en este paso.
	_registro.updateForces();

	for (auto it = _solidos.begin(); it != _solidos.end();) {
		(*it)->actualizarVida(t);
		if ((*it)->debeMorir() || (*it)->getPosicion().y < -50.0f) {
			_registro.deleteSolidoRegistry(*it);
			delete *it;
			it = _solidos.erase(it);
		} else {
			++it;
		}
	}
}

void SistemaSolidoRigido::agregar(SolidoRigido* solido)
{
	_solidos.push_back(solido);
	if (_gravedadActiva) _registro.addRegistry(&_gravedad, solido);
	if (_vientoActivo) _registro.addRegistry(&_viento, solido);
	if (_torbellinoActivo) _registro.addRegistry(&_torbellino, solido);
}

void SistemaSolidoRigido::activarGeneracion(bool activa)
{
	_generando = activa;
}

void SistemaSolidoRigido::setDensidad(float densidad)
{
	_densidad = densidad > 0.05f ? densidad : 0.05f;
}

void SistemaSolidoRigido::activarGravedad(bool activa)
{
	_gravedadActiva = activa;

	_registro.deleteGenerador(&_gravedad);

	if (activa)
	{
		for (auto solido : _solidos)
			_registro.addRegistry(&_gravedad, solido);
	}
}

void SistemaSolidoRigido::activarViento(bool activa, Vector3 vel)
{
	_vientoActivo = activa;
	_viento.setVelocidad(vel);

	_registro.deleteGenerador(&_viento);

	if (activa)
	{
		for (auto solido : _solidos)
			_registro.addRegistry(&_viento, solido);
	}
}

void SistemaSolidoRigido::activarTorbellino(bool activa)
{
	_torbellinoActivo = activa;

	_registro.deleteGenerador(&_torbellino);

	if (activa)
	{
		for (auto solido : _solidos)
			_registro.addRegistry(&_torbellino, solido);
	}
}

void SistemaSolidoRigido::generadorSR()
{
	// Probad a cambiar estas dos constantes para ver como afecta.
	const float media = 0.0f;
	const float desviacion = 2.0f;
	std::normal_distribution<float> gauss(media, desviacion);

	Vector3 pos = _origen + Vector3(_u(_rng), 0.0f, _u(_rng)) * 5.0f;
	Vector3 vel = _velocidadMedia + Vector3(gauss(_rng), gauss(_rng), gauss(_rng));
	Vector4 color(0.2f, 0.6f, 1.0f, 1.0f);

	SolidoRigido* nuevo = crearCajaDinamica(_physics, _scene, PxTransform(pos), Vector3(1.0f, 1.0f, 1.0f), _densidad, color, 15.0f);
	nuevo->comprobarDinamico()->setLinearVelocity(vel);

	agregar(nuevo);
}

SolidoRigido* SistemaSolidoRigido::crearCajaDinamica(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, float densidad, Vector4 color, float vidaMax)
{
	// Creamos el solido rigido (SR) de Physx segun lo visto en la teoria.
	PxRigidDynamic* actor = physics->createRigidDynamic(pose);

	PxShape* forma = CreateShape(PxBoxGeometry(mitades));
	actor->attachShape(*forma);
	PxRigidBodyExt::updateMassAndInertia(*actor, densidad);
	
	//Práctica 5.1
	//actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false); // Si queremos usar la nuestra desactivar
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	

	scene->addActor(*actor);

	// Generamos un renderizado visual del SR dentro de nuestro espacio de renderizado.
	RenderItem* ri = new RenderItem(forma, actor, color);
	return new SolidoRigido(actor, ri, "caja", vidaMax);
}

SolidoRigido* SistemaSolidoRigido::crearCajaEstatica(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, Vector4 color)
{
	// Genero un solido rigido estatico (SR) de Physx segun lo visto en la teoria.
	PxRigidStatic* actor = physics->createRigidStatic(pose);
	PxShape* forma = CreateShape(PxBoxGeometry(mitades));
	actor->attachShape(*forma);
	scene->addActor(*actor);

	// Genero un renderizado visual del SR dentro de nuestro espacio de renderizado.
	RenderItem* ri = new RenderItem(forma, actor, color);
	return new SolidoRigido(actor, ri, "suelo", -1.0f);
}

SolidoRigido* SistemaSolidoRigido::crearCajaDinamicaInerciaManual(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, float masa, Vector3 inercia, Vector4 color, float vidaMax)
{
	PxRigidDynamic* actor = physics->createRigidDynamic(pose);
	PxShape* forma = CreateShape(PxBoxGeometry(mitades));
	actor->attachShape(*forma);
	actor->setMass(masa);
	actor->setMassSpaceInertiaTensor(inercia);
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	scene->addActor(*actor);

	RenderItem* ri = new RenderItem(forma, actor, color);
	return new SolidoRigido(actor, ri, "caja_inercia", vidaMax);
}
