#include "light.h"

void light::setTransfo(const ofMatrix4x4 & matrix)
{
	primitive::setTransfo(matrix);
	li->setTransformMatrix(matrix);
}
