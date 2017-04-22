#include "primitiveTopo.h"

primitiveTopo::primitiveTopo() : primitiveTopo{ nullptr, ofColor(255, 255, 255), ofColor(255, 255, 255) }
{

}

primitiveTopo::primitiveTopo(ofxBezierSurface* primitive) : primitiveTopo{ primitive, ofColor(255, 255, 255), ofColor(255, 255, 255) }
{

}

primitiveTopo::primitiveTopo(ofxBezierSurface* primitive, ofColor fill) : primitiveTopo{ primitive, fill, ofColor(255, 255, 255) }
{

}

primitiveTopo::primitiveTopo(ofxBezierSurface* primitive, ofColor fill, ofColor stroke) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeCol{ stroke }
{

}

primitiveTopo::primitiveTopo(ofxBezierSurface * primitive, ofColor fill, ofParameter<float> strokeThickness) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeThickness{ strokeThickness }
{

}

primitiveTopo::primitiveTopo(ofxBezierSurface * primitive, ofColor fill, ofColor stroke, ofParameter<float> strokeThickness) : primitive{}, prim{ primitive }, fillCol{ fill }, strokeCol{ stroke }, strokeThickness{ strokeThickness }
{

}


ofxBezierSurface* primitiveTopo::getprimitiveTopo() {
	return prim;
}

ofColor primitiveTopo::getFillColor() {
	return fillCol;
}

ofColor primitiveTopo::getStrokeColor() {
	return strokeCol;
}

void primitiveTopo::draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader)
{
	ofSetLineWidth(1.0);

	ofSetColor(getStrokeColor());

	ofPushMatrix();

	ofTranslate(transfoMatrix.getTranslation());

	ofQuaternion rotation = transfoMatrix.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofScale(transfoMatrix.getScale());

	lightShader.useMaterial(&mat);
	lightShader.begin();
	if (wireframe || selected.get())
		prim->drawWireframe();
	else
		prim->draw();
	lightShader.end();
	lightShader.removeMaterial();

	ofPopMatrix();
}