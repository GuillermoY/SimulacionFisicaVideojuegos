#pragma once
#include <map>
#include "GeneradorFuerzaSR.h"
#include "SolidoRigido.h"

typedef std::pair<GeneradorFuerzaSR*, SolidoRigido*> RFSRPair;

// Empareja generadores de fuerza con los solidos que deben recibirlos (igual que ParticleForceRegistry
// para particulas). Permite que cada solido reciba de forma independiente solo las fuerzas que le
// correspondan, aunque varios solidos compartan el mismo generador (p.ej. el mismo viento).
class RegistroFuerzasSR : public std::multimap<GeneradorFuerzaSR*, SolidoRigido*> {
public:
	void updateForces() {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForce(it->second);
		}
	}

	void addRegistry(GeneradorFuerzaSR* gen, SolidoRigido* solido) {
		insert(RFSRPair(gen, solido));
	}

	void deleteSolidoRegistry(SolidoRigido* solido) {
		for (auto it = begin(); it != end(); ) {
			if (it->second == solido) it = erase(it);
			else it++;
		}
	}

	void deleteGenerador(GeneradorFuerzaSR* gen) {
		for (auto it = begin(); it != end();) {
			if (it->first == gen) it = erase(it);
			else it++;
		}
	}
};
