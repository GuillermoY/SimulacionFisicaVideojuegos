#pragma once
#include "SolidoRigido.h"

// Base de los generadores de fuerza para solidos rigidos (equivalente a ForceGenerator para particulas).
class GeneradorFuerzaSR {
public:
	virtual void updateForce(SolidoRigido* solido) = 0;
	virtual ~GeneradorFuerzaSR() {}
};
