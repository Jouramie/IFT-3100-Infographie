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
	void draw(bool wireframe = false) override;

protected:
	const ofVec3f getLocalPosition() const override { return model->getPosition(); }
private:
	ofxAssimpModelLoader* model;
};