#include "renderer.h"
#include "ofxCvImage.h"
#include "forme3d.h"


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

	ofVec3f sum = ofVec3f();
	int count = 0;
	for (auto& p : *scn)
	{
		if (p.getSelected()) {
			sum += p.getGlobalPosition();
			count++;
		}
	}
	sum /= count;
	cam->setTarget(sum);
}


void renderer::draw()
{
	ofClear(background);

	cam->begin();
	ofPushMatrix();

	ofEnableDepthTest();

	ofSetLineWidth(1.0);

	if (translate) {
		ofTranslate(deltaX, deltaY, deltaZ);
	}
	if (rotate) {
		ofRotate(centerX, 1, 0, 0);
		ofRotate(centerY, 0, 1, 0);
		ofRotate(centerZ, 0, 0, 1);
	}
	if (scale) {
		ofScale(scaleX, scaleY, scaleZ);
	}

	for (auto& p : *scn)
	{
		p.draw(wireFrame);
	}

	std::list<ofRay>::iterator iterator2;
	for (iterator2 = rays.begin(); iterator2 != rays.end(); ++iterator2)
	{
		iterator2->draw();
	}

	ofDisableDepthTest();

	ofPopMatrix();
	cam->end();
	if (isFiltered) {
		checkFilters();
	}
}

void renderer::imageExport(const string name, const string extension)
{
	draw();

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


void renderer::applySelection(ofMatrix4x4 matrix)
{
	for (auto& p : *scn)
	{
		if (p.selected.get())
		{
			ofMatrix4x4 oldMat = p.getTransfo();
			p.setTransfo(oldMat * matrix);
		}
	}
}

//-------------------------------- 2D Primitives----------------
/**
* Render a square with given width, height and border width.
*/
ofParameter<bool> renderer::createSquare(float x, float y, float w, float h) {
	return createSquare(x,y,w,h,fill, stroke);
}

/**
* Render a square with given width, height, border width and color.
*/
ofParameter<bool> renderer::createSquare(float x, float y, float w, float h, ofColor fillColor, ofColor strokeColor) {
	ofPath* rect = new ofPath();
	rect->rectangle(ofRectangle(x, y, w, h));
	rect->setColor(fillColor);
	rect->setStrokeColor(strokeColor);
	rect->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ rect, fillColor, strokeColor,strokeThickness };
	prim.setName("Carre " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a circle/ellipse with given radius.
*/
ofParameter<bool> renderer::createCircle(float x, float y, float r1, float r2) {
	return createCircle(x, y, r1, r2, fill, stroke);
}

/**
* Render a circle/ellipse with given radius and color.
*/
ofParameter<bool> renderer::createCircle(float x, float y, float r1, float r2, ofColor fillColor, ofColor strokeColor) {
	ofPath* circle = new ofPath();
	circle->ellipse(x, y, r1, r2);
	circle->setColor(fillColor);
	circle->setStrokeColor(strokeColor);
	circle->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ circle, fillColor, strokeColor, strokeThickness };
	prim.setName("Cercle " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a line with given x, y and deltas.
*/
ofParameter<bool> renderer::createLine(float x, float y, float xDelta, float yDelta) {
	return createLine(x, y, xDelta, yDelta, stroke);
}

/**
* Render a line with given color, x, y and deltas.
*/
ofParameter<bool> renderer::createLine(float x, float y, float xDelta, float yDelta, ofColor fillColor) {
	ofPath* line = new ofPath();
	line->moveTo(x, y, 0);
	line->lineTo(x +  xDelta, y + yDelta);
	line->setColor(fillColor);	
	line->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ line, fillColor, strokeThickness };
	prim.setName("Ligne " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a triangle with given points.
*/
ofParameter<bool> renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	return createTriangle(x1, y1, x2, y2, x3, y3, fill, stroke);
}

/**
* Render a triangle with given points and color.
*/
ofParameter<bool> renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofColor fillColor, ofColor strokeColor) {
	ofPath* triangle = new ofPath();
	triangle->triangle(x1, y1, x2, y2, x3, y3);
	triangle->setColor(fillColor);
	triangle->setStrokeColor(strokeColor);
	triangle->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ triangle, fillColor, strokeColor, strokeThickness };
	prim.setName("Triangle " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a line with given x,y and deltas.
*/
ofParameter<bool> renderer::createPoint(float x, float y, float radius) {
	return createPoint(x, y, radius, fill, stroke);
}

/**
* Render a line with given x,y and deltas.
*/
ofParameter<bool> renderer::createPoint(float x, float y, float radius, ofColor fillColor, ofColor strokeColor) {
	ofPath* point = new ofPath();
	point->circle(x, y, radius);
	point->setColor(fillColor);
	point->setStrokeColor(strokeColor);
	point->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ point, fillColor, strokeColor, strokeThickness };
	prim.setName("Point " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}
//-------------3D primitives-----------------------
ofParameter<bool> renderer::createCube(int x, int y, int z, int w, int h, int d)
{
	return createCube(x, y, z, w, h, d, fill);
}

ofParameter<bool> renderer::createCube(int x, int y, int z, int w, int h, int d, ofColor fillCol)
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
	prim.setName("Cube " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;

 	//cout << *scn;
}

ofParameter<bool> renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createSphere(x, y, z, sizeX, sizeY, sizeZ, fill);
}

ofParameter<bool> renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color)
{
	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setPosition(0, 0, 0);

	float smallest = min(sizeX, min(sizeY, sizeZ));

	ball->setRadius(smallest/2);

	float newX = (float)sizeX / smallest;
	float newY = (float)sizeY / smallest;
	float newZ = (float)sizeZ / smallest;

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	primitive3d prim = primitive3d{ ball, color, matrix };
	prim.setName("Sphere " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

ofParameter<bool> renderer::createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createCone(x, y, z, sizeX, sizeY, sizeZ, fill);
}

ofParameter<bool> renderer::createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color)
{
	ofConePrimitive* cone = new ofConePrimitive();
	cone->setPosition(0, 0, 0);

	float smallest = min(sizeX, sizeZ);
	cone->setRadius(smallest / 2);
	cone->setHeight(sizeY);

	float newX = (float)sizeX / smallest;
	float newY = 1.0f;
	float newZ = (float)sizeZ / smallest;

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	primitive3d prim = primitive3d{ cone, color, matrix };
	prim.setName("Cone " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;

}

ofParameter<bool>  renderer::createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createIcecream(x, y, z, sizeX, sizeY, sizeZ, fill);
}

ofParameter<bool>  renderer::createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofColor color)
{
	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setPosition(x, y + sizeY / 3, z);

	ofConePrimitive* cone = new ofConePrimitive();
	cone->setPosition(x, y - sizeY / 3, z);

	float smallestSphere = min(sizeX, min(sizeY, sizeZ));
	ball->setRadius(smallestSphere / 2);

	float smallestCone = min(sizeX, sizeZ);
	cone->setRadius(smallestCone / 2);
	cone->setHeight(sizeY);

	float newX = (float)sizeX / smallestSphere;
	float newY = (float)sizeY / smallestSphere;
	float newZ = (float)sizeZ / smallestSphere;

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	forme3d forme{ ball, color, matrix };
	forme.addPrimitive(cone);
	forme.setName("IceCream " + to_string(scn->nbElements() + 1));
	scn->addElement(forme);
	return forme.selected;
}

ofParameter<bool> renderer::importModel(string path) {
	ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
	bool ret = model->loadModel(path, false);
	if (ret)
	{
		model->enableTextures();
		ofTexture tex = ofTexture();
		extModel mod = extModel(model);

		string fName(path);
		size_t pos = fName.rfind(".");
		if (pos != string::npos)
		{
			if (pos != 0)
			{
				fName = fName.substr(0, pos);
			}
		}
		pos = fName.rfind("\\");
		if (pos != string::npos)
		{
			if (pos != 0)
			{
				fName = fName.substr(pos + 1);
			}
		}

		mod.setName(fName + " " + to_string(scn->nbElements() + 1));
		scn->addElement(mod);
		return mod.selected;
	}
	return ofParameter<bool>(true);
}

void renderer::clearPrimitives()
{
	scn->clearElements();
}

void renderer::changeWireFrameMode()
{
	wireFrame = !wireFrame;
}

void renderer::setWireFrameMode(bool wf)
{
	wireFrame = wf;
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

	//for (primitive& p : *scn)
	//{
	//	if (!shiftHeld)
	//	{
	//		p.setSelected(false);
	//	}

	//	float* distance = new float(0);

	//	bool found = p.checkIntersectionPlaneAndLine(ray, distance);
	//	if (found)// && *distance >= 0 && *distance < distanceClosest)
	//	{
	//		intersectPrim = &p;
	//		//distanceClosest = *distance;
	//	}
	//}

	//if (distanceClosest < (std::numeric_limits<int>::max() - 1))
	//{
	//	intersectPrim->setSelected(!intersectPrim->getSelected());
	//	std::cout << "Selected Primitive" << std::endl;
	//}
	//else
	//{
	//	std::cout << "Selected Nothing" << std::endl;
	//}
}

void renderer::addBlur() {
	isFiltered = true;
	blur = true;
}

void renderer::removeBlur() {
	blur = false;
	if (!invert && !dilate) 
		isFiltered = false;
}

void renderer::addInvert() {
	isFiltered = true;
	invert = true;
}

void renderer::removeInvert() {
	invert = false;
	if (!blur && !dilate)
		isFiltered = false;
}

void renderer::addDilate() {
	isFiltered = true;
	dilate = true;
}

void renderer::removeDilate() {
	dilate = false;
	if (!blur && !invert)
		isFiltered = false;
}

renderer::~renderer()
{
}
