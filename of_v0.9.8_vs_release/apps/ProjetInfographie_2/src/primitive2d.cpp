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

void primitive2d::draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader)
{
	ofSetLineWidth(1.0);
	prim->setStrokeWidth(strokeThickness);

	//ofSetColor(strokeCol);

	ofPushMatrix();

	ofTranslate(transfoMatrix.getTranslation());

	ofQuaternion rotation = transfoMatrix.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofScale(transfoMatrix.getScale());
	//mat.begin();
	lightShader.useMaterial(&mat);
	lightShader.begin();
	if (wireframe || selected.get())
		prim->setFilled(false);
	else
		prim->setFilled(true);
	prim->draw();
	//mat.end();
	lightShader.end();
	lightShader.removeMaterial();
	ofPopMatrix();
}

vector<hit> primitive2d::intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) {

	//ofMatrix4x4 toWorldSpace = transfoMatrix;
	//ofMesh mesh = prim->();

	return vector<hit>();// primitive::intersectsMesh(screenCoordinates, mesh, cam, toWorldSpace);
}
