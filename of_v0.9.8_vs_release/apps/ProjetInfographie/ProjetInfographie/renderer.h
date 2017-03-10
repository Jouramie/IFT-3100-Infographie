#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "primitive.h"
#include <limits>
#include "scene.h"


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
	void createCube(int x, int y, int z, int w, int h, int d);
	void createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol);
	void createSphere(int x, int y, int z, int size);
	void createSphere(int x, int y, int z, int size, ofColor color);
	void clearPrimitives();
	void changeWireFrameMode();
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }

	~renderer();

private:
	void setupCamera();

	std::list<primitive> primitives;

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

