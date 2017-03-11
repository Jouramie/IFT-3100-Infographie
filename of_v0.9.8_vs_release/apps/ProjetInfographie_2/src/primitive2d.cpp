#include "primitive2d.h"

primitive2d::primitive2d() : primitive2d{ nullptr, ofColor(255, 255, 255) }
{

}

primitive2d::primitive2d(ofPath* primitive) : primitive2d{ primitive, ofColor(255, 255, 255) }
{

}

primitive2d::primitive2d(ofPath* primitive, ofColor fill) : primitive{}, prim{ primitive }, fillCol{ fill }
{

}


ofPath* primitive2d::getPrimitive2d() {
	return prim;
}

ofColor primitive2d::getFillColor() {
	return fillCol;
}

void primitive2d::draw(bool wireframe)
{
	ofSetLineWidth(1.0);
	prim->draw();
}
