#pragma once
#include <vector>
#include <random>
#include "SolidoRigido.h"
#include "GravedadSR.h"
#include "VientoSR.h"
#include "TorbellinoSR.h"
#include "RegistroFuerzasSR.hpp"

// Gestiona un conjunto de solidos rigidos: los genera automaticamente, les activa las fuerzas
// del sistema y los elimina cuando mueren. Es el equivalente a vuestro ParticleSystem.
class SistemaSolidoRigido {
public:
	SistemaSolidoRigido(PxPhysics* physics, PxScene* scene);
	~SistemaSolidoRigido();

	// Genera (si toca), aplica fuerzas y limpia los solidos muertos. Llamadlo cada frame antes de gScene->simulate().
	void update(float t);

	// Añade un solido creado fuera del sistema (p.ej. con una tecla) para que el sistema lo gestione:
	// lo registra con la gravedad y/o el viento SOLO si estan activos para el sistema (por defecto,
	// ninguno lo esta: un solido nuevo usa la gravedad automatica de PhysX hasta que activeis la manual).
	void agregar(SolidoRigido* solido);

	void activarGeneracion(bool activa);
	void setDensidad(float densidad);
	inline float getDensidad() const { return _densidad; }

	// La gravedad se trata como cualquier otro generador de fuerza: activarla aqui solo la registra
	// para los solidos nuevos que se añadan al sistema; sois vosotros quienes debeis aseguraros de no
	// tener a la vez la gravedad automatica de PhysX (PxActorFlag::eDISABLE_GRAVITY) y esta activas,
	// o el solido caera con el doble de aceleracion.
	void activarGravedad(bool activa);
	void activarViento(bool activa, Vector3 vel = Vector3(20.0f, 0.0f, 0.0f));

	void activarTorbellino(bool activa);

	inline const std::vector<SolidoRigido*>& getSolidos() const { return _solidos; }

	// Creacion de solidos rigidos (los generadores de fuerza solo se encargan de aplicarles fuerzas).
	static SolidoRigido* crearCajaDinamica(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, float densidad, Vector4 color, float vidaMax = -1.0f);
	static SolidoRigido* crearCajaEstatica(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, Vector4 color);
	static SolidoRigido* crearCajaDinamicaInerciaManual(PxPhysics* physics, PxScene* scene, PxTransform pose, Vector3 mitades, float masa, Vector3 inercia, Vector4 color, float vidaMax = -1.0f);

protected:
	// Genera un nuevo solido en una posicion con dispersion uniforme y con una velocidad que sigue una
	// distribucion gaussiana (media y desviacion tipica definidas como constantes dentro de la funcion).
	void generadorSR();

	PxPhysics* _physics;
	PxScene* _scene;

	std::vector<SolidoRigido*> _solidos;

	RegistroFuerzasSR _registro;
	GravedadSR _gravedad;
	VientoSR _viento;
	TorbellinoSR _torbellino;

	bool _generando;
	float _frecuencia; // segundos entre generaciones
	float _tiempoDesdeUltimo;
	float _densidad;

	bool _gravedadActiva;
	bool _vientoActivo;
	bool _torbellinoActivo;

	Vector3 _origen;
	Vector3 _velocidadMedia;

	std::mt19937 _rng;
	std::uniform_real_distribution<float> _u;
};
