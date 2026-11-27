#include "GravedadSR.h"

GravedadSR::GravedadSR(Vector3 g)
{
	_g = g;
}

void GravedadSR::updateForce(SolidoRigido* solido)
{
	PxRigidDynamic* d = solido->comprobarDinamico();
	if (!d) return;
	// F = m * g
	d->addForce(_g * d->getMass());
}
