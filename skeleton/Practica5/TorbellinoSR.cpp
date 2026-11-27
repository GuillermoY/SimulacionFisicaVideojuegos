#include "TorbellinoSR.h"

TorbellinoSR::TorbellinoSR(Vector3 centro, float intensidad)
{
	_centro = centro;
	_intensidad = intensidad;
}

void TorbellinoSR::updateForce(SolidoRigido* solido)
{
	// TODO Actividad 1/2.
    PxRigidDynamic* d = solido->comprobarDinamico();
    if (!d) return;

    Vector3 pos = solido->getPosicion();

    // Posición relativa al centro
    float x = pos.x - _centro.x;
    float z = pos.z - _centro.z;

    // Dirección tangencial al giro alrededor del eje Y
    Vector3 fuerza(-z, 0.0f, x);

    float longitud = sqrt(x * x + z * z);

    if (longitud > 0.001f)
    {
        fuerza = fuerza * (1.0f / longitud);
    }

    // Componente vertical ascendente
    fuerza.y = 1.0f;

    // Aplicar intensidad
    fuerza = fuerza * _intensidad;

    d->addForce(fuerza);
}
