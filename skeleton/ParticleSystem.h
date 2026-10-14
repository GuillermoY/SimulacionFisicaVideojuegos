#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "Particle.h"
#include <vector>
#include <random>
using namespace std;

class ParticleSystem
{
public:
	ParticleSystem(float velMin, float velMax, float distPos, Vector3D dir, Vector3D pos, 
		Vector4 color = { 1.0f, 0.0f, 0.5f, 1.0f });
	~ParticleSystem();

	void integrate(double t);

	void switchState(); // Intercambia entre estar parado o continuar
	bool isOn() { return !isStop; };
private:
	float velMax;
	float velMin;
	float distPos;

	Vector3D dir;
	Vector3D pos;

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> uniforme{ -1.0f, 1.0f };
	std::normal_distribution<float> gaussiana{ 0.0f, 1.0f };

	vector<Particle*> particles;

	Vector3D grav = {0,-10.0f,0};

	int distr = 0;
	
	Vector4 color;

	bool isStop = true;
};