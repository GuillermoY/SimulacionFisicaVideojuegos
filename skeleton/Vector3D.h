#include "foundation/PxMat33.h"
#include <vector>
#include <cmath>

using namespace physx;

class Vector3D
{
public:
	float x, y, z;
	Vector3D(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {};
	Vector3D(physx::PxVec3 val) : x (val.x), y(val.y), z(val.z) {};

	float magnitude() const // módulo
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3D normalize() const // normalización
	{
		float mag = magnitude();
		Vector3D res;
		res.x = x / mag;
		res.y = y / mag;
		res.z = z / mag;
		return res;
	}

	float dot(const Vector3D& v) const // Producto escalar
	{
		return v.x * x + v.y * y + v.z * z;
	}

	Vector3D cross(const Vector3D& v) const // Producto vectorial
	{
		Vector3D res;
		res.x = y * v.z - z * v.y;
		res.y = z * v.x - x * v.z;
		res.z = x * v.y - y * v.x;
		return res;
	}

	Vector3D operator=(Vector3D const& b) const
	{
		return b;
	}

	Vector3D operator+(Vector3D const& b) const
	{
		Vector3D res;
		res.x = x + b.x;
		res.y = y + b.y;
		res.z = z + b.z;
		return res;
	}

	Vector3D operator-(Vector3D const& b) const
	{
		Vector3D res;
		res.x = x - b.x;
		res.y = y - b.y;
		res.z = z - b.z;
		return res;
	}

	Vector3D operator*(Vector3D const& b) const // Por un escalar
	{
		float escalar = dot(b);
		Vector3D res;
		res.x = x * escalar;
		res.y = y * escalar;
		res.z = z * escalar;
		return res;
	}

	Vector3D& operator+=(Vector3D const& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		
		return *this;
	}

	operator physx::PxVec3() const
	{
		return physx::PxVec3(x,y,z);
	}
};