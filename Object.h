#ifndef _OBJECT_H
#define _OBJECT_H


#include <math.h>
#include "Ray.h"
#include "Color.h"
#include "Vect.h"
class Object{
	// centro y radio 
public:
	Object();
	// metodos
	Color getObjectColor(){ return Color(0.0,0.0,0.0,0); }
	double findIntersection(Ray ray)
	{
		return 0;
	}

};

Object::Object(){}
#endif
