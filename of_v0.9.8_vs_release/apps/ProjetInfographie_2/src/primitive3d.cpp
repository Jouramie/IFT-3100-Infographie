#include "primitive3d.h"
#include <algorithm>

primitive3d::primitive3d() : primitive3d{ nullptr, ofColor(255, 255, 255), ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive) : primitive3d{ primitive, ofColor(255, 255, 255), ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive, ofColor fill) : primitive3d{ primitive, fill, ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive, ofColor fill, ofMatrix4x4 matrix) : primitive{}, prim { primitive }, fillCol{ fill }
{
	transfoMatrix = matrix;
}

of3dPrimitive* primitive3d::getPrimitive() {
	return prim;
}

ofColor primitive3d::getFillColor() {
	return fillCol;
}

void primitive3d::draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) {

	/*ofPoint pointa = ofPoint(0, -200, 0);
	ofPoint pointb = ofPoint(100, 100, 0);
	ofPoint pointc = ofPoint(-100, 100, 0);

	ofTriangle(pointa, pointb, pointc);
	return;*/

	ofPushMatrix();
	ofTranslate(transfoMatrix.getTranslation());

	ofSetColor(fillCol);

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
		prim->drawWireframe();
	else
		prim->drawFaces();
	lightShader.end();
	lightShader.removeMaterial();
	ofPopMatrix();
}

bool primitive3d::intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) {

	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();
	
	return intersectsMesh(screenCoordinates, mesh, cam, toWorldSpace);
}