#ifndef _PLANE_H
#define _PLANE_H


#include <math.h>
#include "Color.h"
#include "Vect.h"
#include "Object.h"
class Plane :public Object{

	//Orienta el plano
	Vect normal;
	//distancia del centro del plano al centro de la escena
	double distance;
	Color color;


public:
	Plane();
	Plane(Vect, double, Color);
	// metodos
	Vect getPlaneNormal(){ return normal; }
	double getPlaneDistance(){ return distance; }
	virtual Color getColor(){ return color; }
	virtual Vect getNormalAt( Vect point )
	{
		// no importa donde intercepte siempre va a ser la misma normal al punto x
		return normal;
	}

	virtual double findIntersection(Ray ray)
	{
		Vect ray_direction = ray.getRayDirection();

		double a = ray_direction.dotProduct(normal);
		if (a == 0)
		{
			// el rayo es paralelo al plano
			return -1;
		}
		else
		{
			// producto punto (normal (°) origen.rayo-(distancia*normal))
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			//distancia entre el rayo de origen por la interseccion
			return -1 * b / a;
		}
	}

};

Plane::Plane(){
	// x es arriba y derecha izq y z prof
	normal = Vect(1, 0, 0);
	distance = 0;
	color = Color(0.5, 0.5, 0.5, 0);
}
// si les enviamos las coordenadas;
Plane::Plane(Vect normalValue, double distanceValue, Color colorValue){
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
