#pragma once

#include "ofxRay.h"
#include "ccamera.h"

class primitive
{
public:
	primitive() : primitive{ ofMatrix4x4() } { }
	primitive(ofMatrix4x4 matrix) : transfoMatrix{ matrix }, selected{ false } { }

	virtual void draw() { draw(false); }
	virtual void draw(bool wireframe) = 0;
	const ofVec3f getGlobalPosition() const;

	bool getSelected() { return selected; }
	void setSelected(bool selected) { this->selected = selected; }
	void changeSelected() {	selected = !selected; }

	string getName() { return name; }

	void setName(string n) {
		name = n;
		selected.setName(n);
	}

	ofMatrix4x4 getTransfo() { return transfoMatrix; }
	void setTransfo(ofMatrix4x4 matrix) { this->transfoMatrix = matrix; }
	ofParameter<bool> selected;

	bool intersectsMesh(ofRay ray, const ofMesh &mesh, const ofMatrix4x4 &toWorldSpace, vector<int> *meshHit);
	virtual bool intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) { return false; };
	virtual bool getColorOfRay(ofRay ray, ofColor * hit) { return false; };
	bool calcTriangleIntersection(const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, ofRay ray, float *result) const;
	bool calcPlaneIntersection(const ofVec3f &planeOrigin, const ofVec3f &planeNormal, ofRay ray, float *result) const;

protected:
	virtual const ofVec3f getLocalPosition() const = 0;
	ofMatrix4x4 transfoMatrix;
	string name;
};

