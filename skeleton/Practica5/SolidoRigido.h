#pragma once
#include <string>
#include <PxPhysicsAPI.h>
#include "../RenderUtils.hpp"
#include "../core.hpp"

using namespace physx;

// Envoltorio sobre un actor de PhysX (estatico o dinamico) que le añade dibujado y tiempo de vida.
class SolidoRigido {
public:
	SolidoRigido(PxRigidActor* actor, RenderItem* renderItem, const std::string& nombre = "solido", float vidaMax = -1.0f);
	virtual ~SolidoRigido();

	inline bool esDinamico() const { return _actor->is<PxRigidDynamic>() != nullptr; }
	inline PxRigidDynamic* comprobarDinamico() const { return _actor->is<PxRigidDynamic>(); }
	inline PxRigidActor* getActor() const { return _actor; }
	inline Vector3 getPosicion() const { return _actor->getGlobalPose().p; }

	inline void actualizarVida(float t) { _vida += t; }
	// Si vidaMax es negativa el solido no muere nunca por tiempo (p.ej. un suelo estatico).
	inline bool debeMorir() const { return _vidaMax >= 0.0f && _vida > _vidaMax; }

	// Aplica un par puro (no produce fuerza lineal). Solo tiene efecto si el solido es dinamico.
	void aplicarParPuro(Vector3 par);

	// Bloquea la traslacion del solido dejando libre la rotacion (aisla el efecto de un par).
	void bloquearTraslacion();

	static Vector3 calcularInerciaCaja(float masa, Vector3 mitades);

	std::string _nombre;

protected:
	PxRigidActor* _actor;
	RenderItem* _renderItem;
	float _vida;
	float _vidaMax;
};
