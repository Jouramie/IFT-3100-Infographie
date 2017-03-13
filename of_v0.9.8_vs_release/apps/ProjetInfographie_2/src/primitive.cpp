#include "primitive.h"

const ofVec3f primitive::getGlobalPosition() const
{
	ofNode dummy = ofNode();
	dummy.setPosition(getLocalPosition());
	dummy.setTransformMatrix(transfoMatrix);
	return dummy.getGlobalPosition();
}
