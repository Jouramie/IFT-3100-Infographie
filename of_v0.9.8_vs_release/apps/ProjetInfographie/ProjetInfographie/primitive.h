#pragma once

#include "ofMain.h"

class primitive
{
public:
	
	primitive();
	primitive(of3dPrimitive* primitive);
	primitive(of3dPrimitive* primitive, ofColor fill);
	of3dPrimitive* getPrimitive();
	ofColor getFillColor();

private:

	of3dPrimitive* prim;
	ofColor fillCol;

};