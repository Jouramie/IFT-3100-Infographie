#pragma once

#include "ofxRay.h"
#include "ccamera.h"
#include "ofxShadersFX.h"

class primitive
{
public:
	primitive() : primitive{ ofMatrix4x4() } { }
	primitive(ofMatrix4x4 matrix) : transfoMatrix{ matrix }, selected{ false } { }

	virtual void draw(ofxShadersFX::Lighting::LightingShader& lightShader) { draw(false, lightShader); }
	virtual void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) = 0;
	const ofVec3f getGlobalPosition() const;

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

	bool intersectsMesh(const ofVec2f &screenCoordinates, const ofMesh &mesh, const ofCamera &cam, const ofMatrix4x4 &toWorldSpace);
	virtual bool intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) { return false; };
	bool calcTriangleIntersection(const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, ofRay ray, float *result) const;
	bool calcPlaneIntersection(const ofVec3f &planeOrigin, const ofVec3f &planeNormal, ofRay ray, float *result) const;

	void setMaterial(const ofMaterial& mat) { this->mat = mat; };
	ofMaterial getMaterial() const { return mat; };

protected:
	virtual const ofVec3f getLocalPosition() const = 0;
	ofMatrix4x4 transfoMatrix;
	string name;
	ofMaterial mat;
};

