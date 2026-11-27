#include <ctype.h>
#include <cmath>
#include "EjerciciosCuaterniones.h"

EjerciciosCuaterniones::EjerciciosCuaterniones()
{
	_activa = false;
	_physics = nullptr;
	_scene = nullptr;
}

EjerciciosCuaterniones::~EjerciciosCuaterniones()
{
}

void EjerciciosCuaterniones::init(PxPhysics* physics, PxScene* scene)
{
	_physics = physics;
	_scene = scene;
}

void EjerciciosCuaterniones::cleanup()
{
	limpiar();
}

void EjerciciosCuaterniones::limpiar()
{
	for (auto m : _marcadores) delete m;
	_marcadores.clear();
}

// Coloca una flecha en la orientacion "respuesta". Verde si coincide con "esperado", rojo si no (recordad
// que un cuaternion y su opuesto representan la misma rotacion). Si esta mal, añade una flecha
// semitransparente en la orientacion correcta para poder comparar. A partir del ejercicio 2, todos parten
// del mismo punto y tambien se dibuja su orientacion inicial (identidad) en gris, para comparar antes/despues.
void EjerciciosCuaterniones::comprobarRotacion(int numero, int subindice, PxQuat respuesta, PxQuat esperado)
{
	PxQuat r = respuesta.getNormalized();
	PxQuat e = esperado.getNormalized();
	bool correcto = fabsf(r.dot(e)) > 0.999f;

	// El ejercicio 1 muestra sus tres sub-casos (1a/1b/1c) a la vez, cada uno en su propio hueco por
	// subindice. Del 2 en adelante, todos usan el mismo punto: la escena se resetea entre teclas, asi que
	// no hace falta separarlos, y usar siempre el mismo sitio hace mas facil comparar.
	Vector3 base = (numero == 1) ? Vector3(5.0f, 5.0f + subindice * 3.0f, 0.0f) : Vector3(0.0f, 10.0f, 0.0f);
	Vector4 color = correcto ? Vector4(0.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	if (numero != 1) {
		crearFlecha(base, PxQuat(PxIdentity), Vector4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	crearFlecha(base, r, color);

	if (!correcto) {
		crearFlecha(base, e, Vector4(0.0f, 0.0f, 1.0f, 0.35f));
	}
}

void EjerciciosCuaterniones::crearFlecha(Vector3 pos, PxQuat orientacion, Vector4 color)
{
	PxTransform poseVara(pos, orientacion.getNormalized());

	PxRigidStatic* vara = _physics->createRigidStatic(poseVara);
	PxShape* formaVara = CreateShape(PxBoxGeometry(0.15f, 0.15f, 1.5f));
	vara->attachShape(*formaVara);
	_scene->addActor(*vara);
	_marcadores.push_back(new SolidoRigido(vara, new RenderItem(formaVara, vara, color), "flecha"));

	PxTransform poseAleta = poseVara.transform(PxTransform(Vector3(0.35f, 0.0f, 1.1f)));
	PxRigidStatic* aleta = _physics->createRigidStatic(poseAleta);
	PxShape* formaAleta = CreateShape(PxBoxGeometry(0.5f, 0.12f, 0.3f));
	aleta->attachShape(*formaAleta);
	_scene->addActor(*aleta);
	_marcadores.push_back(new SolidoRigido(aleta, new RenderItem(formaAleta, aleta, color), "flecha_aleta"));
}

// Igual que comprobarRotacion pero comparando una posicion (Actividad 4, ejercicio 9).
void EjerciciosCuaterniones::comprobarPosicion(int numero, int subindice, Vector3 respuesta, Vector3 esperado)
{
	bool correcto = (respuesta - esperado).magnitude() < 0.1f;
	Vector4 color = correcto ? Vector4(0.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	Vector3 offset(numero * 5.0f, 0.0f, subindice * 8.0f);

	PxRigidStatic* actor = _physics->createRigidStatic(PxTransform(respuesta + offset));
	PxShape* forma = CreateShape(PxSphereGeometry(0.5f));
	actor->attachShape(*forma);
	_scene->addActor(*actor);
	_marcadores.push_back(new SolidoRigido(actor, new RenderItem(forma, actor, color), "ejercicio"));

	if (!correcto) {
		PxRigidStatic* fantasma = _physics->createRigidStatic(PxTransform(esperado + offset));
		PxShape* formaFantasma = CreateShape(PxSphereGeometry(0.5f));
		fantasma->attachShape(*formaFantasma);
		_scene->addActor(*fantasma);
		_marcadores.push_back(new SolidoRigido(fantasma, new RenderItem(formaFantasma, fantasma, Vector4(0.0f, 0.0f, 1.0f, 0.35f)), "esperado"));
	}
}

void EjerciciosCuaterniones::keyPress(unsigned char key)
{
	const PxQuat ROT_30_X(PxPi / 6.0f, Vector3(1.0f, 0.0f, 0.0f));

	switch (toupper(key)) {
	case 'C':
		_activa = !_activa;
		break;

	case '1':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(1, 0, ejercicio1a(), PxQuat(PxPi / 6.0f, Vector3(1.0f, 0.0f, 0.0f)));
		comprobarRotacion(1, 1, ejercicio1b(), PxQuat(PxPi / 3.0f, Vector3(0.0f, 1.0f, 0.0f)));
		comprobarRotacion(1, 2, ejercicio1c(), PxQuat(PxPi, Vector3(0.0f, 0.0f, 1.0f)));
		break;

	case '2':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(2, 0, ejercicio2(), ROT_30_X);
		break;

	case '3':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(3, 0, ejercicio3(), ROT_30_X);
		break;

	case '4':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(4, 0, ejercicio4(), ROT_30_X);
		break;

	case '5':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(5, 0, ejercicio5(), ROT_30_X);
		break;

	case '6':
		if (!_activa) break;
		limpiar();
		comprobarRotacion(6, 0, ejercicio6(), PxQuat(PxPi / 4.0f, Vector3(0.0f, 0.0f, 1.0f)));
		break;

	case '7':
	{
		if (!_activa) break;
		limpiar();
		PxQuat esperado7 = PxQuat(50.0f * PxPi / 180.0f, Vector3(1.0f, 0.0f, 0.0f)) * PxQuat(25.0f * PxPi / 180.0f, Vector3(0.0f, 1.0f, 0.0f));
		comprobarRotacion(7, 0, ejercicio7(), esperado7);
		break;
	}

	case '8':
	{
		if (!_activa) break;
		limpiar();
		PxQuat esperado8 = PxQuat(50.0f * PxPi / 180.0f, Vector3(1.0f, 0.0f, 0.0f)) * PxQuat(25.0f * PxPi / 180.0f, Vector3(0.0f, 1.0f, 0.0f));
		comprobarRotacion(8, 0, ejercicio8(), esperado8);
		break;
	}

	case '9':
	{
		if (!_activa) break;
		limpiar();
		PxQuat q(0.0f, 0.22f, 0.0f, 0.98f); // x,y,z,w segun el constructor de PxQuat
		Vector3 esperado9_1 = q.getNormalized().rotate(Vector3(10.0f, 20.0f, 30.0f));
		comprobarPosicion(9, 0, ejercicio9_1(), esperado9_1);
		comprobarPosicion(9, 1, ejercicio9_2(), Vector3(0.0f, 0.0f, 0.0f));
		break;
	}

	default:
		break;
	}
}

// ------------------------------------------------------------------
// TODO: implementad cada ejercicio. De momento devuelven la identidad (sin rotacion), por lo que se pintaran en rojo.
// ------------------------------------------------------------------

PxQuat EjerciciosCuaterniones::ejercicio1a()
{
	// TODO: matriz de rotacion de 30 grados respecto al eje "x".
	float a = PxPi / 6.0f; // 30 grados
	PxMat33 m(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, cos(a), sin(a)), Vector3(0.0f, -sin(a), cos(a))); // Cambia la matriz a la que hayas calculado
	return PxQuat(m);
}

PxQuat EjerciciosCuaterniones::ejercicio1b()
{
	// TODO: matriz de rotacion de 60 grados respecto al eje "y".
	float a = PxPi / 3.0f; // 60 grados
	PxMat33 m(Vector3(cos(a), 0.0f, -sin(a)), Vector3(0.0f, 1.0f, 0.0f), Vector3(sin(a), 0.0f, cos(a))); // Cambia la matriz a la que hayas calculado
	return PxQuat(m); // Cambia PxIdentity por m cuando la tengas definida (ver ejemplo arriba)
}

PxQuat EjerciciosCuaterniones::ejercicio1c()
{
	// TODO: matriz de rotacion de 180 grados respecto al eje "z".
	float a = PxPi; // 180 grados
	PxMat33 m(Vector3(cos(a), sin(a), 0.0f), Vector3(-sin(a), cos(a), 0.0f), Vector3(0.0f, 0.0f, 1.0f)); // Cambia la matriz a la que hayas calculado
	return PxQuat(m); // Cambia PxIdentity por m cuando la tengas definida (ver ejemplo arriba)
}

PxQuat EjerciciosCuaterniones::ejercicio2()
{
	// TODO: angulos de Euler intrinsecos z-x-z equivalentes al ejercicio 1a.
	float z1 = 0.0f, x = PxPi / 6.0f, z2 = 0.0f; //Cambiar a los ángulos de rotación correspondientes
	// Aquí los cuaterniones se definen como pares de rotación (w, (nx,ny,nz)) como los vistos en clase.
	// Los ejes ya son los de la convencion Z-X-Z; PxQuat exige que el eje sea unitario incluso con angulo 0,
	// por eso no pueden ser (0,0,0).
	PxQuat qz1(z1, Vector3(0.0f, 0.0f, 1.0f));
	PxQuat qx(x, Vector3(1.0f, 0.0f, 0.0f));
	PxQuat qz2(z2, Vector3(0.0f, 0.0f, 1.0f));
	// Si tenemos rotaciones intrínsecas z-x-z, la rotación final es qz1 * qx * qz2 (el orden de multiplicación es importante). 
	return qz1 * qx * qz2;
}

PxQuat EjerciciosCuaterniones::ejercicio3()
{
	// TODO: eje y angulo de rotacion equivalentes al ejercicio 1a.
	// Se define PxQuat(ángulo, eje)
	return PxQuat(PxPi / 6.0f, Vector3(1.0f, 0.0f, 0.0f)); //Cambiar PxIdentity por el cuaternion definido.
}

PxQuat EjerciciosCuaterniones::ejercicio4()
{
	// TODO: cuaternion (calculado a mano con la formula del angulo mitad) equivalente al ejercicio 1a.
	// En este ejercicio construiremos el cuaternión usando directamente la definición de cuaternión.
	// PxQuat(q.x, q.y, q.z, cos(theta/2.0f))
	float a = PxPi / 6.0f; // 30 grados
	return PxQuat(sin(a / 2.0f) , 0.0f, 0.0f, cos(a / 2.0f)); //Cambiar PxIdentity por el cuaternion definido.
}

PxQuat EjerciciosCuaterniones::ejercicio5()
{
	// TODO: angulos de Euler intrinsecos z-y-z equivalentes al ejercicio 1a (Ver ejercicio 2)
	// Para ello hay que girar 90 sobre el eje z y luego sus 30 grados
	float z1 = -PxPi / 2.0f, y = PxPi / 6.0f, z2 = PxPi / 2.0f;
	PxQuat qz1(z1, Vector3(0.0f, 0.0f, 1.0f));
	PxQuat qx(y, Vector3(0.0f, 1.0f, 0.0f));
	PxQuat qz2(z2, Vector3(0.0f, 0.0f, 1.0f));
	return PxQuat(qz1 * qx * qz2); //Cambiar PxIdentity por el cuaternion definido.
}

PxQuat EjerciciosCuaterniones::ejercicio6()
{
	// TODO: cuaternion equivalente a la matriz de rotacion R del enunciado.
	// Una forma de hacerlo sería construir directamente un cuaternión con dicha matriz de rotación como en el ejercicio 1.
	// Sin embargo en este ejercicio el objetivo es calcular el cuaternión a mano y luego comprobarlo aquí.
	// PxQuat(q.x, q.y, q.z, cos(theta/2.0f))
	float a = (PxPi / 4.0f)/2; // 45 grados, como es de 45 da igual sin que cos

	return PxQuat(0.0f, 0.0f, sin(a), cos(a)); //Cambiar PxIdentity por el cuaternion definido.
}

PxQuat EjerciciosCuaterniones::ejercicio7()
{
	// TODO: matriz de rotacion extrinseca: 25 grados respecto al eje "y" seguido de 50 grados respecto al eje "x".
	// Viendo los apuntes una matriz de rotación extrínsica es equivalente a R' = R2*R1
	// Definir las matrices de rotación como el Ejercicio 1 y luego:
	float a = 25.0f * PxPi / 180.0f; // 25 grados
	PxMat33 m1(Vector3(cos(a), 0.0f, -sin(a)), Vector3(0.0f, 1.0f, 0.0f), Vector3(sin(a), 0.0f, cos(a))); // Cambia la matriz a la que hayas calculado
	float b = 50.0f * PxPi / 180.0f; // 50 grados
	PxMat33 m2(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, cos(b), sin(b)), Vector3(0.0f, -sin(b), cos(b))); // Cambia la matriz a la que hayas calculado

	PxMat33 mTotal = m2 * m1; //Usar mTotal para construir el cuaternión final.
	return PxQuat(mTotal);
}

PxQuat EjerciciosCuaterniones::ejercicio8()
{
	// TODO: cuaternion que representa la misma rotacion que el ejercicio 7.
	float a = 25.0f * PxPi / 180.0f; // 25 grados
	float b = 50.0f * PxPi / 180.0f; // 50 grados
	PxQuat ay(a, Vector3(0, 1, 0));
	PxQuat bx(b, Vector3(1, 0, 0));
	return bx * ay;
}
#include <iostream>
Vector3 EjerciciosCuaterniones::ejercicio9_1()
{
	// TODO: donde queda P=(10,20,30) tras rotarlo con el cuaternion Q=(0.98,0,0.22,0) del enunciado (w,x,y,z).

	PxQuat ay({ 0.98,0,0.22,0 });
	ay.normalize();
	PxVec3 P = { 10,20,30 };
	PxVec3 a = ay.rotate(P);
	//std::cout << a.x << " " << a.y << " " << a.z << "\n";

	return a; //Devolver P rotado que resulta en {21.8636, -20, -22.8469}
}

Vector3 EjerciciosCuaterniones::ejercicio9_2()
{
	// TODO: donde queda P=(0,0,0) tras la misma rotacion? Piensa por que pasa esto.
	PxQuat ay({ 0.98,0,0.22,0 });
	ay.normalize();
	PxVec3 P = { 0,0,0 };

	return ay.rotate(P); //Devolver P rotado, que es {0,0,0}
}
