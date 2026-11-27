#include "VientoSR.h"

VientoSR::VientoSR(Vector3 vel)
{
	_vel = vel;
}

void VientoSR::updateForce(SolidoRigido* solido)
{
	// TODO Actividad 1/2.

	PxRigidDynamic* d = solido->comprobarDinamico();
	if (!d) return;

	d->addForce(_vel);
}
