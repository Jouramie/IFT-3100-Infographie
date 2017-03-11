#include "renderer.h"
#include "ofxCvImage.h"


renderer::renderer()
{
}


void renderer::setup()
{
	ofSetFrameRate(60);
	/*mainCam = ofEasyCam();
	rotate = -1;
	mainCam.begin();*/
	
	filter.allocate(ofGetWindowWidth(), ofGetWindowHeight());

	setupCamera();
	timeCurrent = timeLast = ofGetElapsedTimef();

	isCameraMoveLeft = false;
	isCameraMoveRight = false;
	isCameraMoveUp = false;
	isCameraMoveDown = false;
	isCameraMoveForward = false;
	isCameraMoveBackward = false;
	blur = false;
	invert = false;
	dilate = false;
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

	for (auto& p : *scn)
	{
		p.draw(wireFrame);
	}
// 	std::list<primitive>::iterator iterator;
// 	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
// 	{
// 		iterator->draw(wireFrame);
// 	}

	std::list<ofRay>::iterator iterator2;
	for (iterator2 = rays.begin(); iterator2 != rays.end(); ++iterator2)
	{
		iterator2->draw();
	}
	chekFilters();
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

void renderer::chekFilters(){
	//--------------Filters
	sceneImg.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	scenePixels = sceneImg.getPixels();
	filter.clear();
	filter.allocate(ofGetWindowWidth(), ofGetWindowHeight());
	filter.setFromPixels(scenePixels);
	if (blur) {
		filter.blur();
	}
	if (invert) {
		filter.invert();
	}
	if (dilate) {
		filter.dilate();
	}
	filter.mirror(false, true);
	filter.draw(-(ofGetWindowWidth()*.75), -(ofGetWindowHeight()*.75), ofGetWindowWidth()*1.5, ofGetWindowHeight()*1.5);
}

//-------------------------------- 2D Primitives----------------
/**
* Render a square with given width, height and border width.
*/
void renderer::createSquare(float x, float y, float w, float h) {
	ofColor f = ofColor(255, 255, 255);
	ofColor s = ofColor(255, 255, 255);
	createSquare(x,y,w,h,f,s);
}

/**
* Render a square with given width, height, border width and color.
*/
void renderer::createSquare(float x, float y, float w, float h, ofColor fillColor, ofColor strokeColor) {
	ofPath* rect = new ofPath();
	rect->rectangle(ofRectangle(x, y, w, h));
	rect->setColor(fillColor);
	rect->setStrokeColor(strokeColor);
	scn->addElement(primitive2d{ rect, fillColor, strokeColor });
}

/**
* Render a circle/ellipse with given radius.
*/
void renderer::createCircle(float x, float y, float r1, float r2) {
	ofColor f = ofColor(255, 255, 255);
	ofColor s = ofColor(255, 255, 255);
	createCircle(x, y, r1, r2, f,s);
}

/**
* Render a circle/ellipse with given radius and color.
*/
void renderer::createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor) {
	ofPath* circle = new ofPath();
	circle->ellipse(x, y, r1, r2);
	circle->setColor(fillColor);
	circle->setStrokeColor(strokeColor);
	scn->addElement(primitive2d{ circle, fillColor, strokeColor });
}

/**
* Render a line with given x, y and deltas.
*/
void renderer::createLine(float x, float y, float xDelta, float yDelta) {
	ofColor f = ofColor(255, 255, 255);
	createLine(x, y, xDelta, yDelta, f);
}

/**
* Render a line with given color, x, y and deltas.
*/
void renderer::createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor) {
	ofPath* line = new ofPath();
	line->moveTo(x, y, 0);
	line->lineTo(x +  xDelta, y + yDelta);
	line->setColor(fillColor);	
	scn->addElement(primitive2d{ line, fillColor });
}

/**
* Render a triangle with given points.
*/
void renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	ofColor f = ofColor(255, 255, 255);
	ofColor s = ofColor(255, 255, 255);
	createTriangle(x1, y1, x2, y2, x3, y3, f, s);
}

/**
* Render a triangle with given points and color.
*/
void renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor) {
	ofPath* triangle = new ofPath();
	triangle->triangle(x1, y1, x2, y2, x3, y3);
	triangle->setColor(fillColor);
	triangle->setStrokeColor(strokeColor);
	scn->addElement(primitive2d{ triangle, fillColor, strokeColor });
}

/**
* Render a line with given x,y and deltas.
*/
void renderer::createPoint(float x, float y, float radius) {
	ofColor f = ofColor(255, 255, 255);
	ofColor s = ofColor(255, 255, 255);
	createPoint(x, y, radius, f, s);
}

/**
* Render a line with given x,y and deltas.
*/
void renderer::createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor) {
	ofPath* point = new ofPath();
	point->circle(x, y, radius);
	point->setColor(fillColor);
	point->setStrokeColor(strokeColor);
	scn->addElement(primitive2d{ point, fillColor, strokeColor });
}
//-------------3D primitives-----------------------
void renderer::createCube(int x, int y, int z, int w, int h, int d)
{
	ofColor c = ofColor(255, 255, 255);
	createCube(x, y, z, w, h, d, c);
}

void renderer::createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol)
{
	ofBoxPrimitive* box = new ofBoxPrimitive();

	float smallest = min(w, min(h, d));

	box->setWidth(smallest);
	box->setHeight(smallest);
	box->setDepth(smallest);

	float newX = (float)w / smallest;
	float newY = (float)h / smallest;
	float newZ = (float)d / smallest;

	ofVec3f scaleVec = ofVec3f(newX, newY, newZ);

	box->setPosition(x, y, z);

	for (int i = 0; i < 6; i++)
	{
		box->setSideColor(i, fillCol);
	}
 	scn->addElement(primitive3d{ box, fillCol, scaleVec });
 	cout << *scn;
}

void renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	ofColor c = ofColor(255, 255, 255);
	createSphere(x, y, z, sizeX, sizeY, sizeZ, c);
}

void renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color)
{
	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setPosition(x, y, z);


	float smallest = min(sizeX, min(sizeY, sizeZ));

	ball->setRadius(smallest);

	float newX = (float)sizeX / smallest;
	float newY = (float)sizeY / smallest;
	float newZ = (float)sizeZ / smallest;

	ofVec3f scaleVec = ofVec3f(newX, newY, newZ);

	scn->addElement(primitive3d{ ball, color, scaleVec });
	cout << *scn;
}

void renderer::importModel(string path) {
	ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
	model->loadModel(path, false);
	model->enableTextures();
	ofTexture tex = ofTexture();
	extModel mod = extModel(model);
	externalModels.push_back(mod);
	draw();
}

void renderer::clearPrimitives()
{
	scn->clearElements();
}

void renderer::changeWireFrameMode()
{
	wireFrame = !wireFrame;
}

void renderer::changeCameraMode()
{
	if (camera.getOrtho()) {
		camera.disableOrtho();
	}
	else {
		camera.enableOrtho();
	}
}

void renderer::selectPrimitive(int x, int y, bool shiftHeld)
{
	ofVec3f screenToWorld = camera.screenToWorld(ofVec3f(x, y, 0.0));

	primitive* intersectPrim = nullptr;
	int distanceClosest = std::numeric_limits<int>::max();

	ofVec3f vectNow = (screenToWorld - camera.getPosition());
	vectNow.scale(25);

	ofRay ray(camera.getPosition(), vectNow, true);
	// Pour dessiner le rayon (à des fins de débogage)
	// rays.push_back(ray);

	for (primitive& p : *scn)
	{
		if (!shiftHeld)
		{
			p.setSelected(false);
		}

		float* distance = new float(0);

		bool found = p.checkIntersectionPlaneAndLine(ray, distance);
		if (found)// && *distance >= 0 && *distance < distanceClosest)
		{
			intersectPrim = &p;
			//distanceClosest = *distance;
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
}

void renderer::addBlur() {
	blur = true;
}

void renderer::removeBlur() {
	blur = false;
}

void renderer::addInvert() {
	invert = true;
}

void renderer::removeInvert() {
	invert = false;
}

void renderer::addDilate() {
	dilate = true;
}

void renderer::removeDilate() {
	dilate = false;
}

renderer::~renderer()
{
}
