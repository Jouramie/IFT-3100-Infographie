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

	cam->setCamera(new ofEasyCam{ });
	cam->setup();
	//filter.allocate(ofGetWindowWidth(), ofGetWindowHeight());

	isFiltered = false;
	blur = false;
	invert = false;
	dilate = false;
	translate = false;
	rotate = false;
	scale = false;

	time = lastTime = ofGetElapsedTimef();
}

void renderer::update()
{
	int w = ofGetWidth();
	int h = ofGetHeight();

	if (w != 1024 || h != 768) ofSetWindowShape(1024, 768);

	time = ofGetElapsedTimef();
	dt = time - lastTime;
	lastTime = time;

	cam->update(dt);
}


void renderer::draw()
{
	ofClear(background);

	cam->begin();
	ofPushMatrix();

	ofEnableDepthTest();

	ofSetLineWidth(1.0);

	if (translate) {
		ofTranslate(deltaX, deltaY);
	}
	if (rotate) {
		ofRotate(theta, centerX, centerY, centerZ);
	}
	if (scale) {
		ofScale(scaleX, scaleY);
	}

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

	std::list<extModel>::iterator iterator4;
	for (iterator4 = externalModels.begin(); iterator4 != externalModels.end(); ++iterator4)
	{
		iterator4->draw();
	}

	ofDisableDepthTest();

	ofPopMatrix();
	cam->end();
	if (isFiltered) {
		checkFilters();
	}
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

void renderer::checkFilters(){
	//--------------Filters
	sceneImg.clear();
	sceneImg.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	scenePixels.clear();
	scenePixels = sceneImg.getPixels();
	filter.clear();
	filter.allocate(scenePixels.getWidth(), scenePixels.getHeight());
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
	filter.resize(scenePixels.getWidth(), scenePixels.getHeight());
	filter.draw(0, 0);
}

void renderer::sceneTranslate(float x, float y, float z) {	
	translate = true;
	deltaX = x; 
	deltaY = y;
	deltaZ = z;
}

void renderer::sceneRotate(float angle, float cX, float cY, float cZ) {
	rotate = true;
	theta = angle;
	centerX = cX;
	centerY = cY;
	centerZ = cZ;

}

void renderer::sceneScale(float sX, float sY, float sZ) {
	scale = true;
	scaleX = sX;
	scaleY = sY;
	scaleZ = sZ;
}

//-------------------------------- 2D Primitives----------------
/**
* Render a square with given width, height and border width.
*/
void renderer::createSquare(float x, float y, float w, float h) {
	
	createSquare(x,y,w,h,fill, stroke);
}

/**
* Render a square with given width, height, border width and color.
*/
void renderer::createSquare(float x, float y, float w, float h, ofColor fillColor, ofColor strokeColor) {
	ofPath* rect = new ofPath();
	rect->rectangle(ofRectangle(x, y, w, h));
	rect->setColor(fillColor);
	rect->setStrokeColor(strokeColor);
	rect->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ rect, fillColor, strokeColor,strokeThickness };
	prim.setName("Carre " + (scn->nbElements() + 1));
	scn->addElement(prim);
}

/**
* Render a circle/ellipse with given radius.
*/
void renderer::createCircle(float x, float y, float r1, float r2) {
	createCircle(x, y, r1, r2, fill, stroke);
}

/**
* Render a circle/ellipse with given radius and color.
*/
void renderer::createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor) {
	ofPath* circle = new ofPath();
	circle->ellipse(x, y, r1, r2);
	circle->setColor(fillColor);
	circle->setStrokeColor(strokeColor);
	circle->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ circle, fillColor, strokeColor, strokeThickness };
	prim.setName("Cercle " + (scn->nbElements() + 1));
	scn->addElement(prim);
}

/**
* Render a line with given x, y and deltas.
*/
void renderer::createLine(float x, float y, float xDelta, float yDelta) {
	createLine(x, y, xDelta, yDelta, stroke);
}

/**
* Render a line with given color, x, y and deltas.
*/
void renderer::createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor) {
	ofPath* line = new ofPath();
	line->moveTo(x, y, 0);
	line->lineTo(x +  xDelta, y + yDelta);
	line->setColor(fillColor);	
	line->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ line, fillColor, strokeThickness };
	prim.setName("Ligne " + (scn->nbElements() + 1));
	scn->addElement(prim);
}

/**
* Render a triangle with given points.
*/
void renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	createTriangle(x1, y1, x2, y2, x3, y3, fill, stroke);
}

/**
* Render a triangle with given points and color.
*/
void renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor) {
	ofPath* triangle = new ofPath();
	triangle->triangle(x1, y1, x2, y2, x3, y3);
	triangle->setColor(fillColor);
	triangle->setStrokeColor(strokeColor);
	triangle->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ triangle, fillColor, strokeColor, strokeThickness };
	prim.setName("Triangle " + (scn->nbElements() + 1));
	scn->addElement(prim);
}

/**
* Render a line with given x,y and deltas.
*/
void renderer::createPoint(float x, float y, float radius) {
	createPoint(x, y, radius, fill, stroke);
}

/**
* Render a line with given x,y and deltas.
*/
void renderer::createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor) {
	ofPath* point = new ofPath();
	point->circle(x, y, radius);
	point->setColor(fillColor);
	point->setStrokeColor(strokeColor);
	point->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ point, fillColor, strokeColor, strokeThickness };
	prim.setName("Point " + (scn->nbElements() + 1));
	scn->addElement(prim);
}
//-------------3D primitives-----------------------
void renderer::createCube(int x, int y, int z, int w, int h, int d)
{
	createCube(x, y, z, w, h, d, fill);
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

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	for (int i = 0; i < 6; i++)
	{
		box->setSideColor(i, fillCol);
	}

	primitive3d prim = primitive3d{ box, fillCol, matrix };
	prim.setName("Cube " + (scn->nbElements() + 1));
	scn->addElement(prim);

 	//cout << *scn;
}

void renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	createSphere(x, y, z, sizeX, sizeY, sizeZ, fill);
}

void renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color)
{
	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setPosition(0, 0, 0);


	float smallest = min(sizeX, min(sizeY, sizeZ));

	ball->setRadius(smallest);

	float newX = (float)sizeX / smallest;
	float newY = (float)sizeY / smallest;
	float newZ = (float)sizeZ / smallest;

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	primitive3d prim = primitive3d{ ball, color, matrix };
	prim.setName("Sphere " + (scn->nbElements() + 1));
	scn->addElement(prim);
}

bool renderer::importModel(string path) {
	ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
	bool ret = model->loadModel(path, false);
	if (ret)
	{
		model->enableTextures();
		ofTexture tex = ofTexture();
		extModel mod = extModel(model);

		mod.setName(path.substr(path.find_last_of("\\"), path.find_last_of(".")));

		externalModels.push_back(mod);
	}
	draw();
	return ret;
}

void renderer::clearPrimitives()
{
	scn->clearElements();
}

void renderer::changeWireFrameMode()
{
	wireFrame = !wireFrame;
}

void renderer::selectPrimitive(int x, int y, bool shiftHeld)
{
	ofVec3f screenToWorld = (**cam).screenToWorld(ofVec3f(x, y, 0.0));

	primitive* intersectPrim = nullptr;
	int distanceClosest = std::numeric_limits<int>::max();

	ofVec3f vectNow = (screenToWorld - (**cam).getPosition());
	vectNow.scale(25);

	ofRay ray((**cam).getPosition(), vectNow, true);
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
	isFiltered = true;
	blur = true;
}

void renderer::removeBlur() {
	blur = false;
	isFiltered = false;
}

void renderer::addInvert() {
	isFiltered = true;
	invert = true;
}

void renderer::removeInvert() {
	invert = false;
	isFiltered = false;
}

void renderer::addDilate() {
	isFiltered = true;
	dilate = true;
}

void renderer::removeDilate() {
	dilate = false;
	isFiltered = false;
}

renderer::~renderer()
{
}
