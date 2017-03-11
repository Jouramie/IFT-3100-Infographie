#include "ccamera.h"

void ccamera::setupCamera()
{
	speed = 250.0f;

	cam->setPosition( { 0.0f, 0.0f, -1000.0f } );
	cam->lookAt( { 0.0f, 0.0f, 0.0f } );
}

void ccamera::setupParameters() {
	posX.setName("Position en X");
	posX.setMin(MinX);
	posX.setMax(MaxX);
	posX.set((MinX + MaxX) / 2);

	posY.setName("Position en Y");
	posY.setMin(MinY);
	posY.setMax(MaxY);
	posY.set((MinY + MaxY) / 2);

	posZ.setName("Position en Z");
	posZ.setMin(MinZ);
	posZ.setMax(MaxZ);
	posZ.set((MinZ + MaxZ) / 2);

	fovH.setName("Champs de vision horizontal");
	fovH.setMin(0);
	fovH.setMax(180);
	fovH.set(90);

	fovV.setName("Champs de vision vertical");
	fovV.setMin(0);
	fovV.setMax(180);
	fovV.set(90);

	aspectRatio.setName("Ratio d'aspect");
	aspectRatio.setMin(0);
	aspectRatio.setMax(180);
	aspectRatio.set(90);

	frontClippingPlan.setName("Plan de clipping avant");
	frontClippingPlan.setMin(0);
	frontClippingPlan.setMax(MaxZ);
	frontClippingPlan.set(MaxZ / 2);

	backClippingPlan.setName("Plan de cliping arriere");
	backClippingPlan.setMin(0);
	backClippingPlan.setMax(MaxZ);
	backClippingPlan.set(MaxZ / 2);

	projectionOrthogonal.setName("Projection orthogonal");
	projectionOrthogonal.set(false);

	cameraInteractive.setName("Camera interactive");
	cameraInteractive.set(false);

	parameterGroup.setName("Parametre de la camera");
	parameterGroup.add(posX);
	parameterGroup.add(posY);
	parameterGroup.add(posZ);
	parameterGroup.add(fovH);
	parameterGroup.add(fovV);
	parameterGroup.add(aspectRatio);
	parameterGroup.add(frontClippingPlan);
	parameterGroup.add(backClippingPlan);
	parameterGroup.add(projectionOrthogonal);
	parameterGroup.add(cameraInteractive);
}

void ccamera::update(float dt)
{
	float dist = speed * dt;

	if (isCameraMoveLeft)
		cam->truck(-dist);
	if (isCameraMoveRight)
		cam->truck(dist);

	if (isCameraMoveUp)
		cam->boom(dist);
	if (isCameraMoveDown)
		cam->boom(-dist);

	if (isCameraMoveForward)
		cam->dolly(-dist);
	if (isCameraMoveBackward)
		cam->dolly(dist);
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
