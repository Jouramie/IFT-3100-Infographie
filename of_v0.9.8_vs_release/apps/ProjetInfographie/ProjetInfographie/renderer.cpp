#include "renderer.h"


renderer::renderer()
{
}


void renderer::setup()
{
	ofSetFrameRate(60);
	/*mainCam = ofEasyCam();
	rotate = -1;
	mainCam.begin();*/
	
	setupCamera();
	timeCurrent = timeLast = ofGetElapsedTimef();

	isCameraMoveLeft = false;
	isCameraMoveRight = false;
	isCameraMoveUp = false;
	isCameraMoveDown = false;
	isCameraMoveForward = false;
	isCameraMoveBackward = false;
}

void renderer::setupCamera()
{
	cameraSpeed = 250.0f;
	cameraDeplacement = 0.0f;
	cameraPosition = { 0.0f, 0.0f, -1000.0f };
	cameraTarget = { 0.0f, 0.0f, 0.0f };

	camera.setPosition(cameraPosition);
	camera.lookAt(cameraTarget);
}

void renderer::update()
{
	timeCurrent = ofGetElapsedTimef();
	timeElapsed = timeCurrent - timeLast;
	timeLast = timeCurrent;

	cameraDeplacement = cameraSpeed * timeElapsed;

	if (isCameraMoveLeft)
		camera.truck(-cameraDeplacement);
	if (isCameraMoveRight)
		camera.truck(cameraDeplacement);

	if (isCameraMoveUp)
		camera.boom(cameraDeplacement);
	if (isCameraMoveDown)
		camera.boom(-cameraDeplacement);

	if (isCameraMoveForward)
		camera.dolly(-cameraDeplacement);
	if (isCameraMoveBackward)
		camera.dolly(cameraDeplacement);

	if (camera.getPosition() != cameraPosition) {
		cameraPosition = camera.getPosition();
		ofLog() << "<Camera X: " << camera.getX() << " Y:" << camera.getY() << " Z:" << camera.getZ() << ">";
	}

}


void renderer::draw()
{
	ofClear(background);

	camera.begin();

	ofEnableDepthTest();

	ofSetLineWidth(1.0);

	std::list<primitive>::iterator iterator;
	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
	{
		iterator->draw(wireFrame);
	}

	std::list<ofRay>::iterator iterator2;
	for (iterator2 = rays.begin(); iterator2 != rays.end(); ++iterator2)
	{
		iterator2->draw();
	}

	ofDisableDepthTest();

	camera.end();
}

void renderer::imageExport(const string name, const string extension) const
{
	ofImage imageTemp;

	// extraire des données temporelles formatées
	string timestamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// générer un nom de fichier unique et ordonné
	string fileName = name + timestamp + "." + extension;

	// capturer le contenu du framebuffer actif
	imageTemp.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	// sauvegarder le fichier image
	imageTemp.save(fileName);

	ofLog() << "<export image: " << fileName << ">";
}

void renderer::createCube(int x, int y, int z, int w, int h, int d)
{
	ofColor c = ofColor(255, 255, 255);
	createCube(x, y, z, w, h, d, c);
}

void renderer::createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol)
{
	ofBoxPrimitive* box = new ofBoxPrimitive();
	box->set(w, h, d);

	box->setPosition(x, y, z);
	//box->rotate(rand() % 360, 1.0, 0.0, 0.0);
	//box->rotate(rand() % 360, 0, 1.0, 0.0);
	//box->rotate(rand() % 360, 0, 0.0, 1.0);
	for (int i = 0; i < 6; i++)
	{
		box->setSideColor(i, fillCol);
	}
	primitive prim = primitive(box, fillCol);
	primitives.push_back(prim);
	draw();
}

void renderer::createSphere(int x, int y, int z, int size)
{
	ofColor c = ofColor(255, 255, 255);
	createSphere(x, y, z, size, c);
}

void renderer::createSphere(int x, int y, int z, int size, ofColor color)
{
	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setRadius(size);
	ball->setPosition(x, y, z);
	ball->rotate(rand() % 360, 1.0, 0.0, 0.0);
	ball->rotate(rand() % 360, 0, 1.0, 0.0);
	ball->rotate(rand() % 360, 0, 0.0, 1.0);
	//ball.set
	primitive prim = primitive(ball, color);

	primitives.push_back(prim);
	draw();
}

void renderer::clearPrimitives()
{
	primitives.clear();
}

void renderer::changeWireFrameMode()
{
	wireFrame = !wireFrame;
}

void renderer::selectPrimitive(int x, int y, bool shiftHeld)
{
	ofVec3f screenToWorld = camera.screenToWorld(ofVec3f(x, y, 0.0));

	std::list<primitive>::iterator iterator;

	primitive* intersectPrim = 0;
	int distanceClosest = std::numeric_limits<int>::max();

	ofVec3f vectNow = (screenToWorld - camera.getPosition());
	ofVec3f vecLong = vectNow.scale(25);

	ofRay ray(camera.getPosition(), vectNow, true);
	rays.push_back(ray);

	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
	{
		if (!shiftHeld)
		{
			iterator->setSelected(false);
		}

		float* distance = new float(0);

		bool found = iterator->checkIntersectionPlaneAndLine(ray, distance);
		if (found && *distance >= 0 && *distance < distanceClosest)
		{
			intersectPrim = &(*iterator);
			distanceClosest = *distance;
		}
	}

	if (distanceClosest < (std::numeric_limits<int>::max() - 1))
	{
		intersectPrim->setSelected(!intersectPrim->getSelected());
		std::cout << "Selected Primitive" << std::endl;
	}
	else
	{
		std::cout << "Selected Nothing" << std::endl;
	}
	draw();
}

renderer::~renderer()
{
}
