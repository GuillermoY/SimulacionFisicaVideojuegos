#include "SolidoRigido.h"

SolidoRigido::SolidoRigido(PxRigidActor* actor, RenderItem* renderItem, const std::string& nombre, float vidaMax)
{
	_actor = actor;
	_renderItem = renderItem;
	_nombre = nombre;
	_vida = 0.0f;
	_vidaMax = vidaMax;
}

SolidoRigido::~SolidoRigido()
{
	_renderItem->release();
	_actor->release();
}

void SolidoRigido::aplicarParPuro(Vector3 par)
{
	PxRigidDynamic* d = comprobarDinamico();
	if (d) d->addTorque(par);
}

void SolidoRigido::bloquearTraslacion()
{
	PxRigidDynamic* d = comprobarDinamico();
	if (d) d->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_X | PxRigidDynamicLockFlag::eLOCK_LINEAR_Y | PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
}

Vector3 SolidoRigido::calcularInerciaCaja(float masa, Vector3 mitades)
{
	// TODO Actividad 2: usa la formula del cuboide solido del Anexo (a partir de las semi-medidas "mitades") para
	// devolver el tensor de inercia (Ix,Iy,Iz). De momento devuelve un valor cualquiera para que compile.
	float x = mitades.x;
	float y = mitades.y;
	float z = mitades.z;

	float Ix = (masa / 3.0f) * (y * y + z * z);
	float Iy = (masa / 3.0f) * (x * x + z * z);
	float Iz = (masa / 3.0f) * (x * x + y * y);

	return Vector3(Ix, Iy, Iz);
	//return Vector3(1.0, 1.0, 1.0);
}
