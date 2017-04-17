#include "ccamera.h"

void ccamera::setupCamera()
{
	speed = 500.0f;

	cam->setPosition( { 0.0f, 0.0f, -2000.0f } );
	cam->lookAt( { 0.0f, 0.0f, 0.0f } );
}

void ccamera::setupParameters() {
	posX.setName("Position en X");
	posX.setMin(MinX);
	posX.setMax(MaxX);
	posX.set(-cam->getX());

	posY.setName("Position en Y");
	posY.setMin(MinY);
	posY.setMax(MaxY);
	posY.set(cam->getY());

	posZ.setName("Position en Z");
	posZ.setMin(MinZ);
	posZ.setMax(MaxZ);
	posZ.set(cam->getZ());

	fov.setName("Champs de vision");
	fov.setMin(0);
	fov.setMax(180);
	fov.set(cam->getFov());

	autoRatio.setName("Ratio d'aspect auto.");
	autoRatio.set(true);

	ratio.setName("Ratio d'aspect");
	ratio.setMin(0.25);
	ratio.setMax(4);
	ratio.set(cam->getAspectRatio());

	nearClip.setName("Clipping avant");
	nearClip.setMin(0);
	nearClip.setMax(2 * MaxZ);
	nearClip.set(cam->getNearClip());

	farClip.setName("Cliping arriere");
	farClip.setMin(0);
	farClip.setMax(2 * MaxZ);
	farClip.set(cam->getFarClip());

	ortho.setName("Projection orthogonal");
	ortho.set(false);

	parameterGroup.setName("Parametre de la camera");
	parameterGroup.add(posX);
	parameterGroup.add(posY);
	parameterGroup.add(posZ);
	parameterGroup.add(fov);
	parameterGroup.add(autoRatio);
	parameterGroup.add(ratio);
	parameterGroup.add(nearClip);
	parameterGroup.add(farClip);
	parameterGroup.add(ortho);
}

bool ccamera::compare(ccamera otherCam) {
	bool params1 = (otherCam.autoRatio.get() == autoRatio.get()) && (otherCam.farClip.get() == farClip.get()) && (otherCam.fov.get() == fov.get());
	bool params2 = (otherCam.isCameraMoveBackward == isCameraMoveBackward) && (otherCam.isCameraMoveDown == isCameraMoveDown) && (otherCam.isCameraMoveForward == isCameraMoveForward);
	bool params3 = (otherCam.isCameraMoveLeft == isCameraMoveLeft) && (otherCam.isCameraMoveRight == isCameraMoveRight) && (otherCam.isCameraMoveUp == isCameraMoveUp);
	bool params4 = (otherCam.MaxX == MaxX) && (otherCam.MaxY == MaxY) && (otherCam.MaxZ == MaxZ);
	bool params5 = (otherCam.MinX == MinX) && (otherCam.MinY == MinY) && (otherCam.MinZ == MinZ);
	bool params6 = (otherCam.nearClip.get() == nearClip.get()) && (otherCam.ortho.get() == ortho.get()) && (otherCam.posX.get() == posX.get());
	bool params7 = (otherCam.posY.get() == posY.get()) && (otherCam.posZ.get() == posZ.get()) && (otherCam.ratio.get() == ratio.get());
	bool params8 = (otherCam.speed == speed);
	bool params9 = (otherCam.cam->getGlobalOrientation() == cam->getGlobalOrientation()) && (otherCam.cam->getGlobalPosition() == cam->getGlobalPosition()) && (otherCam.cam->getGlobalScale() == cam->getGlobalScale()) && (otherCam.cam->getGlobalTransformMatrix()._mat == cam->getGlobalTransformMatrix()._mat);
	return (params1 && params2 && params3 && params4 && params5 && params6 && params7 && params8);
}

void ccamera::update(float dt)
{
	float dist = speed * dt;
	float dx = 0;
	float dy = 0;
	float dz = 0;

	dx = 0;
	if (isCameraMoveLeft)
		dx += dist;
	if (isCameraMoveRight)
		dx -= dist;
	cam->truck(-dx);
	posX.set(round(-cam->getX()));

	dy = 0;
	if (isCameraMoveUp)
		dy -= dist;
	if (isCameraMoveDown)
		dy += dist;
	cam->boom(-dy);
	posY.set(round(cam->getY()));

	dz = 0;
	if (isCameraMoveForward)
		dz -= dist;
	if (isCameraMoveBackward)
		dz += dist;
	cam->dolly(dz);
	posZ.set(round(cam->getZ()));

	cam->setFov(fov.get());
	if (autoRatio.get()) {
		cam->setForceAspectRatio(false);
	}
	else {
		cam->setAspectRatio(ratio.get());
	}
	cam->setNearClip(nearClip.get());
	cam->setFarClip(farClip.get());

	if (ortho.get()) {
		cam->enableOrtho();
	}
	else {
		cam->disableOrtho();
	}
}