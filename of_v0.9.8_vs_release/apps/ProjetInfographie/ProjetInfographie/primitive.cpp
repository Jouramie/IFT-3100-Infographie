#include "primitive.h"

primitive::primitive()
{
	prim = 0;
	fillCol = ofColor(255, 255, 255);
}

primitive::primitive(of3dPrimitive* primitive)
{
	prim = primitive;
	fillCol = ofColor(255, 255, 255);
}

primitive::primitive(of3dPrimitive* primitive, ofColor fill)
{
	prim = primitive;
	fillCol = fill;
}


of3dPrimitive* primitive::getPrimitive() {
	return prim;
}

ofColor primitive::getFillColor() {
	return fillCol;
}
