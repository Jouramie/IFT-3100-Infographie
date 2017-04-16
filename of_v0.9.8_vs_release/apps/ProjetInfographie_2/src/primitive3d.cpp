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

void primitive3d::setMirror(bool mirror) {
	if (mirror)
	{
		isGlass = false;
	}
	isMirror = mirror;
}

void primitive3d::setGlass(bool glass) {
	if (glass)
	{
		isMirror = false;
	}
	isGlass = glass;
}

void primitive3d::draw(bool wireframe) {

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


	if (wireframe || selected.get())
		prim->drawWireframe();
	else
		prim->drawFaces();

	ofPopMatrix();
}

bool primitive3d::prepareGlass(const ofCamera &cam) {//, const scene * scn) {
	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<ofMeshFace> allFaces = mesh.getUniqueFaces();

	int a = ofGetWidth();
	int b = ofGetWindowWidth();
	int c = ofGetScreenWidth();
	int d = ofGetViewportWidth();
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < ofGetHeight(); j++) {
			ofVec2f coords = ofVec2f(i, j);
			vector<int>* hits = new vector<int>();

			ofVec3f screenToWorld = cam.screenToWorld(ofVec3f(coords.x, coords.y, 0.0));
			ofRay ray(cam.getPosition(), screenToWorld - cam.getPosition());

			if (intersectsMesh(ray, mesh, toWorldSpace, hits)) {
				for (int k = 0; k < hits->size(); k++) {
					ofMeshFace face = allFaces[(*hits)[k]];

					if (isMirror) {
						ofVec3f normal = face.getFaceNormal();
						ofVec3f transmi = ray.getTransmissionVector();
						ofRay reflectedRay = ofRay(ray.getStart(), (transmi + 2 * (transmi * normal)) * (normal));
						
						ofColor col = ofGetBackgroundColor();

						getColorOfRay(reflectedRay, &col);
						
						face.setColor(0, col);
					}
				}
			}
		}
	}
	return true;
}

bool primitive3d::intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) {

	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<int>* hits = new vector<int>();

	ofVec3f screenToWorld = cam.screenToWorld(ofVec3f(screenCoordinates.x, screenCoordinates.y, 0.0));
	ofRay ray(cam.getPosition(), screenToWorld - cam.getPosition());
	
	return intersectsMesh(ray, mesh, toWorldSpace, hits);
}

bool primitive3d::getColorOfRay(ofRay ray, ofColor * hit) {
	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<int>* hits = new vector<int>();

	vector<ofMeshFace> allFaces = mesh.getUniqueFaces();

	bool hasHit = intersectsMesh(ray, mesh, toWorldSpace, hits);

	if (hasHit)
	{
		*hit = allFaces[(*hits)[0]].getColor(0);
	}
	return hasHit;
}