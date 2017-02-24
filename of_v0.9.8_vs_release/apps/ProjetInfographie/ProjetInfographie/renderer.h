#pragma once
#include "ofMain.h"


class renderer
{
public:

	renderer();

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

	~renderer();

private:
	std::list<of3dPrimitive> primitives;
	bool wireFrame;
};

