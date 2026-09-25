#include "RenderUtils.hpp"
#include "Vector3D.h"


class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel);
	~Particle();

	void integrate(double t);
private:
	Vector3 vel;
	physx::PxTransform pose; // A render item le pasamos la dirección de este pose, para actualizarse automáticamente
	RenderItem* renderItem;

	Vector3 ac; // vector aceleración
	double d; // vector damping
	// Para Verlet
	Vector3 pPos; // previa posición
};