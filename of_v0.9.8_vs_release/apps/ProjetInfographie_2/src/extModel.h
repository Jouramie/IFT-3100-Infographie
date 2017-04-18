#pragma once

#include "ofMain.h"
#include "ofxRay.h"
#include <algorithm>
#include "ofxAssimpModelLoader.h"
#include "primitive.h"

class extModel : public primitive
{
public:

	extModel();
	extModel(ofxAssimpModelLoader* mod);
	extModel(ofxAssimpModelLoader* mod, ofMatrix4x4 transfoMatrix);
	ofxAssimpModelLoader* getModel();
	void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) override;

	bool calcTriangleIntersection(ofRay ray, float *result) const;
	bool inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const;
	bool checkIntersectionPlaneAndLine(ofRay ray, float *result) const;
	bool checkIntersectionTriangleRay(ofRay ray, ofPoint* inter);

protected:
	const ofVec3f getLocalPosition() const override { return model->getPosition(); }
private:
	ofxAssimpModelLoader* model;
};