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

primitive2d::primitive2d(ofPath * primitive, ofColor fill, ofParameter<float> strokeThickness) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeThickness{ strokeThickness }
{

}

primitive2d::primitive2d(ofPath * primitive, ofColor fill, ofColor stroke, ofParameter<float> strokeThickness) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeCol{ stroke }, strokeThickness{ strokeThickness }
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
	prim->setStrokeWidth(strokeThickness);

	//ofSetColor(strokeCol);

	ofPushMatrix();

	ofScale(transfoMatrix.getScale());

	ofQuaternion rotation = transfoMatrix.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofTranslate(transfoMatrix.getTranslation());

	if (wireframe || selected)
		prim->setFilled(false);
	else
		prim->setFilled(true);
	prim->draw();

	ofPopMatrix();
}
