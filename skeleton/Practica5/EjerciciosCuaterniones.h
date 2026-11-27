#pragma once
#include <vector>
#include <PxPhysicsAPI.h>
#include "../RenderUtils.hpp"
#include "../core.hpp"
#include "SolidoRigido.h"

using namespace physx;

// Modulo independiente y autoincluido para practicar cuaterniones (no depende del resto de la Practica 5).
// Se activa con la tecla C. Con la escena activa, cada tecla numerica (1-9) resetea la escena (borra los
// marcadores del ejercicio anterior) y comprueba la solucion de ese ejercicio: crea un solido con la
// orientacion/posicion que devuelva vuestra funcion, en verde si es correcta y en rojo si no lo es (en ese
// caso aparece ademas un solido semitransparente en la orientacion/posicion correcta).
class EjerciciosCuaterniones {
public:
	EjerciciosCuaterniones();
	~EjerciciosCuaterniones();

	void init(PxPhysics* physics, PxScene* scene);
	void cleanup();
	void keyPress(unsigned char key);

	// TODO: completa cada ejercicio (ver el enunciado de la Practica 5). Devuelve el cuaternion que da la solucion.
	static PxQuat ejercicio1a();
	static PxQuat ejercicio1b();
	static PxQuat ejercicio1c();
	static PxQuat ejercicio2();
	static PxQuat ejercicio3();
	static PxQuat ejercicio4();
	static PxQuat ejercicio5();
	static PxQuat ejercicio6();
	static PxQuat ejercicio7();
	static PxQuat ejercicio8();
	static Vector3 ejercicio9_1();
	static Vector3 ejercicio9_2();

protected:
	void comprobarRotacion(int numero, int subindice, PxQuat respuesta, PxQuat esperado);
	void comprobarPosicion(int numero, int subindice, Vector3 respuesta, Vector3 esperado);

	void crearFlecha(Vector3 pos, PxQuat orientacion, Vector4 color);

	void limpiar();

	bool _activa;
	PxPhysics* _physics;
	PxScene* _scene;
	std::vector<SolidoRigido*> _marcadores;
};
