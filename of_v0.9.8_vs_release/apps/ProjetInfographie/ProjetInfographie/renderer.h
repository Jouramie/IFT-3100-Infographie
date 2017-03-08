#pragma once
#include "ofMain.h"


class renderer
{
public:

	ofColor background;

	bool isCameraMoveLeft;
	bool isCameraMoveRight;
	bool isCameraMoveUp;
	bool isCameraMoveDown;
	bool isCameraMoveForward;
	bool isCameraMoveBackward;

	renderer();

	ofColor background;

	void setup();
	void update();
	void draw();
	void imageExport(const string name, const string extension) const;
	void createCube(int x, int y, int z, int size);
	void createCube(int x, int y, int z, int size, ofColor color);
	void createSphere(int x, int y, int z, int size);
	void createSphere(int x, int y, int z, int size, ofColor color);
	void clearPrimitives();
	void changeWireFrameMode();
	void changeRotate();
	void selectPrimitive(int x, int y);

	~renderer();

private:
	void setupCamera();

	std::list<of3dPrimitive> primitives;
	of3dPrimitive* selectedPrim;
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

	ofCamera camera;
};

