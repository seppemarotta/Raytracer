#ifndef _Ray_H
#define _Ray_H

#include <math.h>
#include "Vect.h"

class Ray{
	// origen y la direccion 
	Vect origin, direction;


public:
	Ray();
	Ray(Vect,Vect);
	// metodos
	Vect getRayOrigin(){ return origin; }
	Vect getRayDirection(){ return direction; }


};

Ray::Ray(){
	origin = Vect(0, 0, 0);
	direction = Vect(1, 0, 0);

}
// si les enviamos las coordenadas;
Ray::Ray(Vect o,Vect d){
	origin = o;
	direction = d;
}

#endif
