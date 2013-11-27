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
	virtual Color getColor(){ return Color(0.0,0.0,0.0,0); }
	virtual Vect getNormalAt(Vect intersection_position)
	{
		return Vect(0, 0, 0);
	}
	virtual double findIntersection(Ray ray)
	{
		return 0;
	}

};

Object::Object(){}
#endif
