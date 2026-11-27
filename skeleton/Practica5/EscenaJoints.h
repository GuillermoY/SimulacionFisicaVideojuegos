#pragma once
#include "SolidoRigido.h"

// Ejemplo basico de Joints en PhysX: un "techo" fijo y un "brazo" dinamico colgando de el, unidos por
// una bisagra (PxRevoluteJoint) que le deja girar libremente sobre un eje. La gravedad hace que el
// brazo oscile como un pendulo/puerta.
// Se activa/desactiva con la tecla U.
class EscenaJoints {
public:
	EscenaJoints();
	~EscenaJoints();

	void init(PxPhysics* physics, PxScene* scene);
	void update(float t);
	void cleanup();
	void keyPress(unsigned char key);

protected:
	void crear();
	void destruir();

	bool _activa;
	PxPhysics* _physics;
	PxScene* _scene;

	SolidoRigido* _techo;
	SolidoRigido* _brazo;
	PxRevoluteJoint* _bisagra;
};
