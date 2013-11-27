#ifndef _Cylinder_H
#define _Cylinder_H


#include <math.h>
#include "Color.h"
#include "Vect.h"
#include "Object.h"

class Cylinder :public Object{

	// centro y radio 
	Vect center;
	double altura;
	double radius;
	Color color;


public:
	Cylinder();
	Cylinder(Vect,double, double, Color);
	// metodos
	Vect getCylinderCenter(){ return center; }
	double getCylinderAltura(){ return altura; }
	double getCylinderRadius(){ return radius; }
	virtual Color getColor(){ return color; }
	virtual Vect getNormalAt(Vect point)
	{
		// la normal siempre apunta hacia afuera del centro de la esfera
		Vect normal_Vect = Vect(point.getVectX() - center.getVectX(), 0, point.getVectZ() - center.getVectZ()).normalize();
		return normal_Vect;
	}
	virtual double findIntersection(Ray ray)
	{
		Vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		Vect ray_direction = ray.getRayDirection();
		ray_direction.normalize();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();
		

		Vect Cylinder_center = center;
		double Cylinder_center_x = Cylinder_center.getVectX();
		double Cylinder_center_y = Cylinder_center.getVectY();
		double Cylinder_center_z = Cylinder_center.getVectZ();
		// esto se refiere a esto .
		//El polinomio cuadrático P(x) = ax2 + bx + c 
		// normalizado
		double a = ray_direction_x*ray_direction_x + ray_direction_z*ray_direction_z;
		double b = (ray_origin_x - Cylinder_center_x)*ray_direction_x + (ray_origin_z - Cylinder_center_z)*ray_direction_z;
		double c = pow((ray_origin_x - Cylinder_center_x), 2) + pow((ray_origin_z - Cylinder_center_z),2) - pow(radius,2);

		double discriminant = b*b - a*c ;

		double tNear = -1;
		double fShit = 0;
		double tHit[2];
		double sHit[2];
		int n = 0;

		if (discriminant > 0)
		{
			// el radio intersecto a la esfera, tiene solución.
			/*
			si lo intersecta, va a tener 2 puntos de interseccion , tenemos
			que hallar ambos
			*/
			double uplim = altura + Cylinder_center_y;
			double lowlim = Cylinder_center_y;
			// primera raiz
			double root_1 = ((-1 * b - sqrt(discriminant)) / a) - 0.000001;
			double yHit = ray_origin_y + ray_direction_y*root_1;

			if (root_1 > 0 && yHit >= lowlim && yHit <= uplim)
			{
				tHit[n] = root_1;
				sHit[n] = 0;
				n++;
				// la primera raiz es la menor
			}
			else if (yHit >= lowlim && yHit <= uplim){
				// la segunda tiene que ser la menor
				double root_2 = ((sqrt(discriminant) - b) / a) - 0.000001;
				tHit[n] = root_2;
				sHit[n] = 0;
				n++;
			}
		}

		//Prueba en las tapas

		//En y = 0
	    /*	double tb = (Cylinder_center_y - ray_origin_y) / ray_direction_y; 
		if ((pow((ray_origin_x + ray_direction_x*tb - Cylinder_center_x), 2) + pow((ray_origin_z + ray_origin_z*tb - Cylinder_center_z), 2)) < pow(radius, 2))
		{
			tHit[n] = tb;
			sHit[n] = 1;
			n++;
		}

		//En y = altura
		double tc = (altura + Cylinder_center_y - ray_origin_y) / ray_direction_y;
		if ((pow((ray_origin_x + ray_direction_x*tc - Cylinder_center_x), 2) + pow((ray_origin_z + ray_origin_z*tc - Cylinder_center_z), 2)) < pow(radius, 2))
		{
			tHit[n] = tc;
			sHit[n] = 2;
			n++;
		}
		*/

		if (n == 0 && tNear == -1)
			return -1;

		if (n == 1)
		{
			tNear = tHit[0];
			fShit = sHit[0];
		}
		else
		{
			if (tHit[0] > tHit[1])
			{
				tNear = tHit[1];
				fShit = sHit[1];
			}
			else
			{
				tNear = tHit[0];
				fShit = sHit[0];
			}
		}

		return tNear;
	}

};

Cylinder::Cylinder(){
	center = Vect(0, 0, 0);
	radius = 1.0;
	color = Color(0.5, 0.5, 0.5, 0);
}
// si les enviamos las coordenadas;
Cylinder::Cylinder(Vect centerValue,double height, double radiusValue, Color colorValue){
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
	altura = height;
}

#endif
