#include "primitive2d.h"

primitive2d::primitive2d() : primitive2d{ nullptr, ofColor(255, 255, 255), ofColor(255, 255, 255) }
{

}

primitive2d::primitive2d(ofPath* primitive) : primitive2d{ primitive, ofColor(255, 255, 255), ofColor(255, 255, 255) }
{

}

primitive2d::primitive2d(ofPath* primitive, ofColor fill) : primitive2d{ primitive, fill, ofColor(255, 255, 255) }
{

}

primitive2d::primitive2d(ofPath* primitive, ofColor fill, ofColor stroke) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeCol{ stroke }
{

}


ofPath* primitive2d::getPrimitive2d() {
	return prim;
}

ofColor primitive2d::getFillColor() {
	return fillCol;
}

ofColor primitive2d::getStrokeColor() {
	return strokeCol;
}

void primitive2d::draw(bool wireframe)
{
	ofSetLineWidth(1.0);
	prim->setStrokeWidth(3);
	if (wireframe) {
		prim->setFilled(false);
	}
	else {
		prim->setFilled(true);
	}
	prim->draw();
}
