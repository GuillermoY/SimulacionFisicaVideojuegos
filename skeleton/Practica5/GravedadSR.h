#pragma once
#include "GeneradorFuerzaSR.h"

// Aplica la gravedad "a mano" (F = m*g) a los solidos dinamicos que la tengan registrada. 
class GravedadSR : public GeneradorFuerzaSR {
public:
	GravedadSR(Vector3 g = Vector3(0.0f, -9.8f, 0.0f));

	void updateForce(SolidoRigido* solido) override;

	inline void setGravedad(Vector3 g) { _g = g; }
	inline Vector3 getGravedad() const { return _g; }

protected:
	Vector3 _g;
};
