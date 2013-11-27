#ifndef _Triangle_H
#define _Triangle_H


#include <math.h>
#include "Color.h"
#include "Vect.h"
#include "Object.h"
class Triangle :public Object{

	//Puntos del triangylo
	Vect A;
	Vect B;
	Vect C;
	//Orienta el plano
	Vect normal;
	//distancia del centro del plano al centro de la escena
	double distance;
	Color color;


public:
	Triangle();
	Triangle(Vect, Vect ,Vect , Color);
	// metodos
	Vect getTriangleNormal(){ 

		Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
		Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
		normal = CA.crossProduct(BA).normalize();
		return normal; 
	}
	double getTriangleDistance(){
		normal = getTriangleNormal();
		distance = normal.dotProduct(A);
		return distance; 
	}
	virtual Color getColor(){ return color; }
	virtual Vect getNormalAt(Vect point)
	{
		normal = getTriangleNormal();
		return normal;
	}

	virtual double findIntersection(Ray ray)
	{
		Vect ray_direction = ray.getRayDirection();
		Vect ray_origin = ray.getRayOrigin();
		normal = getTriangleNormal();
		distance = getTriangleDistance();

		double a = ray_direction.dotProduct(normal);
		if (a == 0)
		{
			// el rayo es paralelo al plano
			return -1;
		}
		else
		{
			// producto punto (normal (�) origen.rayo-(distancia*normal))
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			double distance2plane = -1 * b / a;
			//distancia entre el rayo de origen por la interseccion

			double Qx = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
			double Qy = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
			double Qz = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();

			Vect Q(Qx, Qy, Qz);

			Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
			Vect QA(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
			double test1 = (CA.crossProduct(QA)).dotProduct(normal);

			Vect BC(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
			Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
			double test2 = (BC.crossProduct(QC)).dotProduct(normal);

			Vect AB (A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
			Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
			double test3 = (AB.crossProduct(QB)).dotProduct(normal);

			

			if ((test1 >= 0) && (test2 >= 0) && (test3 >= 0))
			{
				//Dentro del triangulo
				return -1 * b / a;
			}
			else
			{
				return -1;
			}
		}
	}

};

Triangle::Triangle(){
	// x es arriba y derecha izq y z prof
	A = Vect(1, 0, 0);
	B = Vect(0, 1, 0);
	C = Vect(0, 0, 1);
	color = Color(0.5, 0.5, 0.5, 0);
}
// si les enviamos las coordenadas;
Triangle::Triangle(Vect pointA, Vect pointB, Vect pointC, Color colorValue){
	A = pointA;
	B = pointB;
	C = pointC;
	color = colorValue;
}

#endif
