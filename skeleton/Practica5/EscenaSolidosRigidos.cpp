#include <ctype.h>
#include "EscenaSolidosRigidos.h"

EscenaSolidosRigidos::EscenaSolidosRigidos()
{
	_activa = false;
	_physics = nullptr;
	_scene = nullptr;
	_sistema = nullptr;
	_suelo = nullptr;
	_bloqueado = nullptr;
	_bloqueadoInerciaManual = nullptr;
}

EscenaSolidosRigidos::~EscenaSolidosRigidos()
{
}

void EscenaSolidosRigidos::init(PxPhysics* physics, PxScene* scene)
{
	_physics = physics;
	_scene = scene;
	_sistema = new SistemaSolidoRigido(physics, scene);
}

void EscenaSolidosRigidos::update(float t)
{
	if (!_activa) return;

	_sistema->update(t);
}

void EscenaSolidosRigidos::cleanup()
{
	delete _sistema; _sistema = nullptr;
	delete _suelo; _suelo = nullptr;
	delete _bloqueado; _bloqueado = nullptr;
	delete _bloqueadoInerciaManual; _bloqueadoInerciaManual = nullptr;
}

void EscenaSolidosRigidos::keyPress(unsigned char key)
{
	switch (toupper(key)) {
	case 'Y':
		_activa = !_activa;
		break;

	case 'V':
		if (!_activa) break;
		_sistema->activarViento(true, Vector3(20.0f, 0.0f, 0.0f));
		break;

	case 'T':
		if (!_activa) break;
		_sistema->activarTorbellino(true);
		break;

	case '1':
		if (!_activa) break;
			_sistema->activarGravedad(true);
		break;


	case '2':
		if (!_activa) break;
		_sistema->agregar(SistemaSolidoRigido::crearCajaDinamica(_physics, _scene, PxTransform(Vector3(0.0f, 20.0f, 0.0f)), Vector3(1.0f, 1.0f, 1.0f), 1.0f, Vector4(0.9f, 0.2f, 0.2f, 1.0f), 8.0f));
		break;

	case '3':
		if (!_activa) break;
		delete _suelo;
		_suelo = SistemaSolidoRigido::crearCajaEstatica(_physics, _scene, PxTransform(Vector3(0.0f, 0.0f, 0.0f)), Vector3(20.0f, 0.5f, 20.0f), Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		break;

	case '4':
		if (!_activa) break;
		_sistema->activarGeneracion(true);
		break;

	case '5':
		if (!_activa) break;
		_sistema->setDensidad(_sistema->getDensidad() * 1.5f);
		break;

	case '6':
		if (!_activa) break;
		_sistema->setDensidad(_sistema->getDensidad() / 1.5f);
		break;

	case '7':
		if (!_activa) break;
		delete _bloqueado;
		_bloqueado = SistemaSolidoRigido::crearCajaDinamica(_physics, _scene, PxTransform(Vector3(0.0f, 15.0f, 0.0f)), Vector3(3.0f, 0.3f, 1.0f), 1.0f, Vector4(0.2f, 0.9f, 0.2f, 1.0f));
		// No lo añadimos al sistema (_sistema->agregar), asi que no queda registrado con ningun generador
		// de fuerza: no recibira gravedad ni ninguna otra fuerza continua salvo lo que le apliquemos aqui.
		_bloqueado->bloquearTraslacion();
		break;

	case '8':
		if (!_activa || !_bloqueado) break;
		//_bloqueado->aplicarParPuro(Vector3(300.0f, 0.0f, 0.0f));
		_bloqueado->aplicarParPuro(Vector3(0.0f, 300.0f, 0.0f));
		//_bloqueado->aplicarParPuro(Vector3(0.0f, 0.0f, 300.0f));
		if (_bloqueadoInerciaManual) _bloqueadoInerciaManual->aplicarParPuro(Vector3(0.0f, 300.0f, 0.0f));
		break;
		// El eje que gira más rápido es el x, porque es el lado más corto
		// 
		// Veo que el sólido al pulsar 7 gira diferente al pulsar 9. Funciona mal
		// porque se supone que la 7 se calcula con Physics y el 9 como la inercia 
		// calculada manualmente y deberían resultar similar, sobre todo si es
		// con igual masa y dimensiones.
		//
	case '9':
		if (!_activa) break;
		delete _bloqueadoInerciaManual;
		{
			// Si calcularInerciaCaja esta bien, este solido girara igual que aquel.
			Vector3 mitades(3.0f, 0.3f, 1.0f);
			float masa = 7.2f;
			Vector3 inercia = SolidoRigido::calcularInerciaCaja(masa, mitades);
			_bloqueadoInerciaManual = SistemaSolidoRigido::crearCajaDinamicaInerciaManual(_physics, _scene, PxTransform(Vector3(10.0f, 15.0f, 0.0f)), mitades, masa, inercia, Vector4(0.9f, 0.9f, 0.2f, 1.0f));
		}
		// Tampoco se añade al sistema: sin gravedad ni ninguna otra fuerza continua salvo el par de la tecla 8.
		_bloqueadoInerciaManual->bloquearTraslacion();
		break;

	case '0':
		if (!_activa) break;
		delete _sistema;
		delete _suelo;
		delete _bloqueado;
		delete _bloqueadoInerciaManual;
		_sistema = new SistemaSolidoRigido(_physics, _scene);
		_suelo = nullptr;
		_bloqueado = nullptr;
		_bloqueadoInerciaManual = nullptr;
		break;

	default:
		break;
	}
}
