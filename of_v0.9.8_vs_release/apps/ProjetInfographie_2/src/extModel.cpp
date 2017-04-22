#include "extModel.h"

extModel::extModel() : extModel{ nullptr } { }

extModel::extModel(ofxAssimpModelLoader* mod) : extModel{ mod, ofMatrix4x4() } { }

extModel::extModel(ofxAssimpModelLoader* mod, ofMatrix4x4 transfo) : primitive{ transfo }, model{ mod } { }

ofxAssimpModelLoader* extModel::getModel() {
	return model;
}

void extModel::draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) {

	ofPushMatrix();

	ofTranslate(transfoMatrix.getTranslation());

	ofQuaternion rotation = transfoMatrix.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofScale(transfoMatrix.getScale());

	if (wireframe || selected.get())
		model->drawWireframe();
	else
		model->drawFaces();

	ofPopMatrix();
}