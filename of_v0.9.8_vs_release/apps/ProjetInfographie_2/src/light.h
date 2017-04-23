#include "primitive.h"

#pragma once
class light : public primitive {
public:
	light() : light{ nullptr, ofMatrix4x4() } {};
	light(ofLight* l) : light{ l, ofMatrix4x4() } {};
	light(ofLight* l, ofMatrix4x4 matrix) : li{ l }, primitive{ matrix }, diffuseColor{ 255, 255, 255 }, specularColor{ 255, 255, 255 } { };
	~light() { lightShader->removeLight(li); };

	void draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) override { };

	void setDiffuseColor(ofColor col) { diffuseColor = col; };
	void setSpecularColor(ofColor col) { specularColor = col; };
	void setLightShader(ofxShadersFX::Lighting::LightingShader* lightShader) { this->lightShader = lightShader; };

private:
	ofLight* li;
	ofColor diffuseColor;
	ofColor specularColor;
	ofxShadersFX::Lighting::LightingShader* lightShader;

protected:
	const ofVec3f getLocalPosition() const override { return li->getPosition(); };
};

