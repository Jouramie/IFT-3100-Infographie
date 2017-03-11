#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "primitive.h"
#include <limits>
#include "scene.h"
#include "primitive2d.h"
#include "extModel.h"


class renderer
{
public:

	renderer();

	ofColor background;

	bool isCameraMoveLeft;
	bool isCameraMoveRight;
	bool isCameraMoveUp;
	bool isCameraMoveDown;
	bool isCameraMoveForward;
	bool isCameraMoveBackward;

	void setup();
	void update();
	void draw();
	void imageExport(const string name, const string extension) const;
	//2D primitives
	void createSquare(float x, float y, float width, float height);
	void createSquare(float x, float y, float width, float height, ofColor fillColor);
	void createCircle(float x, float y, float r1, float r2);
	void createCircle(float x, float y, float r1, float r2, ofColor fillColor);
	void createLine(float x, float y, float xDelta, float yDelta);
	void createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor);
	void createPoint(float x, float y, float radius);
	void createPoint(float x, float y, float radius, ofColor fillColor);

	void createCube(int x, int y, int z, int w, int h, int d);
	void createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	void createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color);
	void importModel(string path);
	void clearPrimitives();
	void changeWireFrameMode();
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }

	~renderer();

private:
	void setupCamera();

    std::list<primitive2d> primitives2d;

	//ofEasyCam mainCam;
	bool wireFrame;
	//int rotate;
	
	float cameraSpeed;
	float cameraDeplacement;

	float timeCurrent;
	float timeElapsed;
	float timeLast;

	ofVec3f cameraPosition;
	ofVec3f cameraTarget;

	scene * scn;
	ofCamera camera;

	std::list<ofRay> rays;
};

