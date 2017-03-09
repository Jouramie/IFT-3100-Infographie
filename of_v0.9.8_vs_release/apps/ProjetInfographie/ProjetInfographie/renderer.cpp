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

	std::list<primitive>::iterator iterator;
	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
	{
		ofSetLineWidth(1.0);
		if (wireFrame) {
			iterator->getPrimitive()->drawWireframe();
		}
		else {
			iterator->getPrimitive()->draw();
		}
	}
	std::list<primitive2d>::iterator iterator2;
	for (iterator2 = primitives2d.begin(); iterator2 != primitives2d.end(); ++iterator2)
	{
		ofSetLineWidth(1.0);
		iterator2->getPrimitive2d()->draw();

	}

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

/**
* Render a square with given width, height and border width.
*/
void renderer::createSquare(float x, float y, float w, float h) {
	ofColor c = ofColor(255, 255, 255);
	createSquare(x,y,w,h,c);
}

/**
* Render a square with given width, height, border width and color.
*/
void renderer::createSquare(float x, float y, float w, float h, ofColor fillColor) {
	ofPath* rect = new ofPath();
	rect->rectangle(ofRectangle(x, y, w, h));
	rect->setColor(fillColor);
	primitive2d prim = primitive2d(rect, fillColor);
	primitives2d.push_back(prim);
	draw();
	
}

/**
* Render a circle/ellipse with given radius.
*/
void renderer::createCircle(float x, float y, float r1, float r2) {
	ofColor c = ofColor(255, 255, 255);
	createCircle(x, y, r1, r2, c);
}

/**
* Render a circle/ellipse with given radius and color.
*/
void renderer::createCircle(float x, float y, float r1, float r2, ofColor fillColor) {
	ofPath* circle = new ofPath();
	circle->ellipse(x, y, r1, r2);
	circle->setColor(fillColor);
	primitive2d prim = primitive2d(circle, fillColor);
	primitives2d.push_back(prim);
	draw();

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
	box->rotate(rand() % 360, 1.0, 0.0, 0.0);
	box->rotate(rand() % 360, 0, 1.0, 0.0);
	box->rotate(rand() % 360, 0, 0.0, 1.0);
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

void renderer::selectPrimitive(int x, int y)
{
	/*ofxRayTriangleIntersection  rtIntersect;
	vector<FaceTri>             tris;
	vector<Ray>                 rays;

	FaceTri tri;
	tri.v0 = ofPoint(ofGetWidth() / 2, ofGetHeight() / 2 / 2, -1000);
	tri.v1 = ofPoint(ofGetWidth() / 2 - 200, ofGetHeight() / 2 + (ofGetHeight() / 2 / 2), -1000);
	tri.v2 = ofPoint(ofGetWidth() / 2 + 200, ofGetHeight() / 2 + (ofGetHeight() / 2 / 2), -1000);
	tris.push_back(tri);

	Ray ray;
	ray.rayOrig.set(0, 0, 0);
	ray.rayEnd.set(0, 0, -2000);
	rays.push_back(ray);*/

	std::cout << "Selected Primitive" << std::endl;
}

renderer::~renderer()
{
}
