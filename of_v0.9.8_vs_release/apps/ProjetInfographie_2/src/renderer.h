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
	void sceneTranslate(float x, float y);
	void sceneRotate(float angle, float centerX, float centerY);
	void sceneScale(float scaleX, float scaleY);
	//2D primitives
	void createSquare(float x, float y, float width, float height);
	void createSquare(float x, float y, float width, float height, ofColor fillColor, ofColor strokeColor);
	void createCircle(float x, float y, float r1, float r2);
	void createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor);
	void createLine(float x, float y, float xDelta, float yDelta);
	void createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor);
	void createPoint(float x, float y, float radius);
	void createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor);

	void createCube(int x, int y, int z, int w, int h, int d);
	void createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);

	bool importModel(string path);

	void clearPrimitives();
	void changeWireFrameMode();
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }
	void setCamera(ccamera * cam) { this->cam = cam; }

	void addBlur();
	void removeBlur();
	void addInvert();
	void removeInvert();
	void addDilate();
	void removeDilate();
	void enableTransform();

	~renderer();

private:

	//ofEasyCam mainCam;
	bool wireFrame;
	bool isFiltered;
	bool blur;
	bool invert;
	bool dilate;
	bool transform;
	//int rotate;

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

