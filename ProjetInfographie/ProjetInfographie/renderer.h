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

	~renderer();
};

