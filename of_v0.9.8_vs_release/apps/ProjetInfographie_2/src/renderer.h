#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "ofxOpenCv.h"
#include "primitive3d.h"
#include <limits>
#include "scene.h"
#include "primitive2d.h"
#include "ccamera.h"
#include "extModel.h"


class renderer
{
public:

	renderer();

	ofColor background;
	ofColor stroke;
	ofColor fill;
	ofParameter<float> strokeThickness;

	void setup();
	void update();
	void draw();
	void imageExport(const string name, const string extension) const;
	void checkFilters();
	void sceneTranslate(float x, float y, float z);
	void sceneRotate(float angle, float centerX, float centerY, float centerZ);
	void sceneScale(float scaleX, float scaleY, float scaleZ);
	//2D primitives
	ofParameter<bool> createSquare(float x, float y, float width, float height);
	ofParameter<bool> createSquare(float x, float y, float width, float height, ofColor fillColor, ofColor strokeColor);
	ofParameter<bool> createCircle(float x, float y, float r1, float r2);
	ofParameter<bool> createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor);
	ofParameter<bool> createLine(float x, float y, float xDelta, float yDelta);
	ofParameter<bool> createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor);
	ofParameter<bool> createTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	ofParameter<bool> createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor);
	ofParameter<bool> createPoint(float x, float y, float radius);
	ofParameter<bool> createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor);

	ofParameter<bool> createCube(int x, int y, int z, int w, int h, int d);
	ofParameter<bool> createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol);
	ofParameter<bool> createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);
	ofParameter<bool> createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);
	ofParameter<bool> createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);

	bool importModel(string path, ofParameter<bool>* selectedHandler);

	void clearPrimitives();
	void changeWireFrameMode();
	void setWireFrameMode(bool wf);
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }
	void setCamera(ccamera * cam) { this->cam = cam; }

	void addBlur();
	void removeBlur();
	void addInvert();
	void removeInvert();
	void addDilate();
	void removeDilate();

	~renderer();

private:

	//ofEasyCam mainCam;
	bool wireFrame;
	//Filters
	bool isFiltered;
	bool blur;
	bool invert;
	bool dilate;
	//Transformations
	bool translate;
	bool rotate;
	bool scale;
	//Translations
	float deltaX, deltaY, deltaZ; 
	//Rotation;
	float theta, centerX, centerY, centerZ;
	//Scale
	float scaleX, scaleY, scaleZ;

	ofVec3f cameraPosition;
	ofVec3f cameraTarget;
	float time;
	float dt;
	float lastTime;

	scene * scn;
	ccamera * cam;

	std::list<ofRay> rays;
	std::list<extModel> externalModels;

	ofxCvColorImage filter;
	ofImage sceneImg;
	ofPixels scenePixels;
};

