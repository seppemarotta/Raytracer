#ifndef _Light_H
#define _Light_H

#include <math.h>
#include "Color.h"

class Light{
	// origen y la direccion 
	Vect position;
	Color color;


public:
	Light();
	Light(Vect, Color);
	// metodos
	Vect getLightPosition(){ return position; }
	Color getLightColor(){ return color; }


};

Light::Light(){
	position = Vect(0, 0, 0);
	color = Color(1, 1, 1,0);
}
// si les enviamos las coordenadas;
Light::Light(Vect p, Color c){
	position = p;
	color = c;
}

#endif
