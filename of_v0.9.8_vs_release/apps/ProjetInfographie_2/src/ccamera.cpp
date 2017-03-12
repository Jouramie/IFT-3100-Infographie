#include "ccamera.h"

void ccamera::setupCamera()
{
	speed = 250.0f;

	cam->setPosition( { (MinX + MaxX) / 2, (MinY + MaxY) / 2, MinZ } );
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

	autoRatio.setName("Ratio d'aspect automatique");
	autoRatio.set(true);

	ratio.setName("Ratio d'aspect");
	ratio.setMin(0.25);
	ratio.setMax(4);
	ratio.set(cam->getAspectRatio());

	nearClip.setName("Plan de clipping avant");
	nearClip.setMin(0);
	nearClip.setMax(2 * MaxZ);
	nearClip.set(cam->getNearClip());

	farClip.setName("Plan de cliping arriere");
	farClip.setMin(0);
	farClip.setMax(2 * MaxZ);
	farClip.set(cam->getFarClip());

	ortho.setName("Projection orthogonal");
	ortho.set(false);

	camInteractive.setName("Camera interactive");
	camInteractive.set(false);

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
	parameterGroup.add(camInteractive);
}

void ccamera::update(float dt)
{
	float dist = speed * dt;
	float dx = 0;
	float dy = 0;
	float dz = 0;

	dx = -cam->getX() - posX.get();
	if (isCameraMoveLeft)
		dx += dist;
	if (isCameraMoveRight)
		dx -= dist;
	cam->truck(-dx);
	posX.set(-cam->getX());

	dy = cam->getY() - posY.get();
	if (isCameraMoveUp)
		dy -= dist;
	if (isCameraMoveDown)
		dy += dist;
	cam->boom(-dy);
	posY.set(cam->getY());

	dz = cam->getZ() - posZ.get();
	if (isCameraMoveForward)
		dz -= dist;
	if (isCameraMoveBackward)
		dz += dist;
	cam->dolly(dz);
	posZ.set(cam->getZ());

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

void ccamera::changeMode()
{
	if (cam->getOrtho()) {
		cam->disableOrtho();
	}
	else {
		cam->enableOrtho();
	}
}

void ccamera::resetPos()
{
	ofLog() << "asD";
	cam = new ofCamera();
	cam->setPosition({ (MinX + MaxX) / 2, (MinY + MaxY) / 2, MinZ });
	cam->lookAt({ 0.0f, 0.0f, 0.0f });
	posX.set(0);
	posY.set(0);
	posZ.set(-2000);
	camInteractive.set(false);
}