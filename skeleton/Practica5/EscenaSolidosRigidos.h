#pragma once
#include "SolidoRigido.h"
#include "SistemaSolidoRigido.h"

class EscenaSolidosRigidos {
public:
	EscenaSolidosRigidos();
	~EscenaSolidosRigidos();

	void init(PxPhysics* physics, PxScene* scene);
	void update(float t);
	void cleanup();
	void keyPress(unsigned char key);

protected:
	bool _activa;
	PxPhysics* _physics;
	PxScene* _scene;

	SistemaSolidoRigido* _sistema;
	SolidoRigido* _suelo;
	SolidoRigido* _bloqueado; // Actividad 3: solido con la traslacion bloqueada, inercia automatica de PhysX
	SolidoRigido* _bloqueadoInerciaManual; // Actividad 3: igual que _bloqueado pero con inercia calculada a mano
};
