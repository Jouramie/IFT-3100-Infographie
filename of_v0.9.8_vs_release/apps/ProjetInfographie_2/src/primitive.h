#pragma once

#include "ofxRay.h"
#include "ccamera.h"
#include "ofxShadersFX.h"

struct hit {
	int faceIndex;
	float distance;
};

struct by_distance {
	bool operator()(hit const &a, hit const &b) {
		return a.distance < b.distance;
	}
};

class primitive
{
public:
	primitive() : primitive{ ofMatrix4x4() } { }
	primitive(ofMatrix4x4 matrix) : transfoMatrix{ matrix }, selected{ false } { }

	virtual void draw(ofxShadersFX::Lighting::LightingShader& lightShader) { draw(false, lightShader); }
	virtual void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) { };
	const ofVec3f getGlobalPosition() const;

	virtual void shouldPrepare() { };

	bool getSelected() { return selected; }
	void setSelected(bool selected) { this->selected = selected; }
	void changeSelected() { selected = !selected; }

	string getName() { return name; }

	void setName(string n) {
		name = n;
		selected.setName(n);
	}

	ofMatrix4x4 getTransfo() const { return transfoMatrix; }
	void setTransfo(const ofMatrix4x4& matrix) { this->transfoMatrix = matrix; }
	ofParameter<bool> selected;

	bool intersectsMesh(ofRay ray, const ofMesh &mesh, const ofMatrix4x4 &toWorldSpace, vector<hit> *meshHit);
	virtual vector<hit> intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) { return vector<hit>(); };
	virtual bool getColorOfRay(ofRay ray, ofColor * hit) { return false; };
	bool calcTriangleIntersection(const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, ofRay ray, float *result) const;
	bool calcPlaneIntersection(const ofVec3f &planeOrigin, const ofVec3f &planeNormal, ofRay ray, float *result) const;
	virtual vector<ofRay> prepareGlass(const ofCamera cam, vector<primitive*> otherPrims, ofColor backgroundCol) { return vector<ofRay>(); };
	virtual ofMesh getMesh() { return ofMesh(); };

	virtual bool isGlassy() { return false; };
	virtual bool isCubeOrSphere() { return false; };

	void setMaterial(const ofMaterial& mat) { this->mat = mat; };
	ofMaterial getMaterial() const { return mat; };

protected:
	virtual const ofVec3f getLocalPosition() const { return ofVec3f(); };
	ofMatrix4x4 transfoMatrix;
	string name;
	ofMaterial mat;
};

