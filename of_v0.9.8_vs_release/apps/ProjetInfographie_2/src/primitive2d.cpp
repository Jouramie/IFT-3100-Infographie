#include "primitive2d.h"

primitive2d::primitive2d()
{
	prim = 0;
	fillCol = ofColor(255, 255, 255);
}

primitive2d::primitive2d(ofPath* primitive)
{
	prim = primitive;
	fillCol = ofColor(255, 255, 255);
}

primitive2d::primitive2d(ofPath* primitive, ofColor fill)
{
	prim = primitive;
	fillCol = fill;
}


ofPath* primitive2d::getPrimitive2d() {
	return prim;
}

ofColor primitive2d::getFillColor() {
	return fillCol;
}
