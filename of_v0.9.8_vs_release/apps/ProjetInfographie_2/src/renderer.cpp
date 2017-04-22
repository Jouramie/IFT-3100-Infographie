#include "renderer.h"
#include "ofxCvImage.h"
#include "forme3d.h"

void renderer::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	/*mainCam = ofEasyCam();
	rotate = -1;
	mainCam.begin();*/
	
	lightShader.setLightingMethod(ofxShadersFX::Lighting::BLINN_PHONG_LIGHTING);
	lightShader.setShadingMethod(ofxShadersFX::Lighting::PIXEL_SHADING);

	cam->setCamera(new ofEasyCam{});
	cam->setup();
	lightShader.useCamera(cam->getOfCamera());
	//filter.allocate(ofGetWindowWidth(), ofGetWindowHeight());

	isFiltered = false;
	blur = false;
	invert = false;
	dilate = false;
	translate = false;
	rotate = false;
	scale = false;

	tempAmbientLight = ofColor::black;
	ofSetGlobalAmbientColor(tempAmbientLight);

	//TODO supprimer lorsque l'interface permettra la création de lumière
	tempDirectionalLight = new ofLight();
	tempDirectionalLight->setDiffuseColor(ofColor(31, 31, 255));
	tempDirectionalLight->setSpecularColor(ofColor(191, 191, 191));
	tempDirectionalLight->setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	tempDirectionalLight->setDirectional();
	//tempDirectionalLight->enable();
	lightShader.useLight(tempDirectionalLight);

	//default activeMaterial
	activeMaterial.setAmbientColor(ofColor::gray);
	activeMaterial.setDiffuseColor(ofColor::darkSeaGreen);
	activeMaterial.setEmissiveColor(ofColor::darkSlateBlue);
	activeMaterial.setSpecularColor(ofColor::gray);
	activeMaterial.setShininess(120.0f);

	time = lastTime = ofGetElapsedTimef();
	setMustPrepares();
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
	vector<primitive*> GlassyPrims;
	vector<primitive*> other3D;

	for (auto& p : *scn)
	{
		if (p.isCubeOrSphere())
		{
			other3D.push_back(&p);
			if (p.isGlassy())
			{
				GlassyPrims.push_back(&p);
			}
		}
	}

	ofClear(background);

	cam->begin();
	ofPushMatrix();

	ofEnableDepthTest();
	//ofEnableLighting();

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
		p.draw(wireFrame, lightShader);
	}

	std::list<ofRay>::iterator iterator2;
	bool origRay = true;
	for (iterator2 = rays.begin(); iterator2 != rays.end(); ++iterator2)
	{
		if (origRay)
			ofSetColor(255, 0, 0);
		else
			ofSetColor(0, 255, 0);
		iterator2->draw();
		origRay = !origRay;
	}

	//ofDisableLighting();
	ofDisableDepthTest();

	ofPopMatrix();

	cam->end();

	for (auto& glassy : GlassyPrims)
	{
		vector<ofRay> newRays = glassy->prepareGlass((**cam), other3D, background);
		for (auto r : newRays)
		{
			//rays.push_back(r);
		}
	}

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

void renderer::checkFilters() {
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
	return createSquare(x, y, w, h, activeMaterial);
}

/**
* Render a square with given width, height, border width and color.
*/
ofParameter<bool> renderer::createSquare(float x, float y, float w, float h, ofMaterial mat) {
	ofPath* rect = new ofPath();
	rect->rectangle(ofRectangle(x, y, w, h));
	//rect->setColor(fillColor);
	//rect->setStrokeColor(strokeColor);
	rect->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ rect, ofColor(), ofColor(), strokeThickness };
	prim.setName("Carre " + to_string(scn->nbElements() + 1));
	prim.setMaterial(mat);
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a circle/ellipse with given radius.
*/
ofParameter<bool> renderer::createCircle(float x, float y, float r1, float r2) {
	return createCircle(x, y, r1, r2, activeMaterial);
}

/**
* Render a circle/ellipse with given radius and color.
*/
ofParameter<bool> renderer::createCircle(float x, float y, float r1, float r2, ofMaterial mat) {
	ofPath* circle = new ofPath();
	circle->ellipse(x, y, r1, r2);
	//circle->setColor(fillColor);
	//circle->setStrokeColor(strokeColor);
	circle->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ circle, ofColor(), ofColor(), strokeThickness };
	prim.setName("Cercle " + to_string(scn->nbElements() + 1));
	prim.setMaterial(mat);
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a line with given x, y and deltas.
*/
ofParameter<bool> renderer::createLine(float x, float y, float xDelta, float yDelta) {
	return createLine(x, y, xDelta, yDelta, activeMaterial);
}

/**
* Render a line with given color, x, y and deltas.
*/
ofParameter<bool> renderer::createLine(float x, float y, float xDelta, float yDelta, ofMaterial mat) {
	ofPath* line = new ofPath();
	line->moveTo(x, y, 0);
	line->lineTo(x + xDelta, y + yDelta);
	//line->setColor(fillColor);
	line->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ line, ofColor(), strokeThickness };
	prim.setName("Ligne " + to_string(scn->nbElements() + 1));
	prim.setMaterial(mat);
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a triangle with given points.
*/
ofParameter<bool> renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	return createTriangle(x1, y1, x2, y2, x3, y3, activeMaterial);
}

/**
* Render a triangle with given points and color.
*/
ofParameter<bool> renderer::createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofMaterial mat) {
	ofPath* triangle = new ofPath();
	triangle->triangle(x1, y1, x2, y2, x3, y3);
	//triangle->setColor(fillColor);
	//triangle->setStrokeColor(strokeColor);
	triangle->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ triangle, ofColor(), ofColor(), strokeThickness };
	prim.setName("Triangle " + to_string(scn->nbElements() + 1));
	prim.setMaterial(mat);
	scn->addElement(prim);
	return prim.selected;
}

/**
* Render a line with given x,y and deltas.
*/
ofParameter<bool> renderer::createPoint(float x, float y, float radius) {
	return createPoint(x, y, radius, activeMaterial);
}

/**
* Render a line with given x,y and deltas.
*/
ofParameter<bool> renderer::createPoint(float x, float y, float radius, ofMaterial mat) {
	ofPath* point = new ofPath();
	point->circle(x, y, radius);
	//point->setColor(fillColor);
	//point->setStrokeColor(strokeColor);
	point->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ point, ofColor(), ofColor(), strokeThickness };
	prim.setName("Point " + to_string(scn->nbElements() + 1));
	prim.setMaterial(mat);
	scn->addElement(prim);
	return prim.selected;
}

void renderer::setMustPrepares() {

	for (auto& p : *scn)
	{
		if (p.isCubeOrSphere())
		{
			if (p.isGlassy())
			{
				p.shouldPrepare();
			}
		}
	}
}

/**
* Render a Bezier curve with given x,y,z and deltas.
*/
ofParameter<bool> renderer::createBezier(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf) {
	return createBezier(cx1, cy1, cz1, cx2, cy2, cz2, xi, yi, zi, xf, yf, zf, fill, stroke);
}

/**
* Render a Bezier curve with given x,y,z and deltas.
*/
ofParameter<bool> renderer::createBezier(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, ofColor fillColor, ofColor strokeColor) {
	ofPath* bezier = new ofPath();
	bezier->moveTo(xi, yi, zi);
	bezier->bezierTo(cx1, cy1, cz1, cx2, cy2, cz2, xf, yf, zf);
	bezier->setColor(fillColor);
	bezier->setStrokeColor(strokeColor);
	bezier->setStrokeWidth(strokeThickness);

	primitive2d prim = primitive2d{ bezier, fillColor, strokeColor, strokeThickness };
	prim.setName("Bezier " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}
/**
* Creates a Hermite spline with given control points ans line Resolution.
*/
ofParameter<bool> renderer::createHermite(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, int lineRes) { 

	return createHermite(cx1, cy1, cz1, cx2, cy2, cz2, xi, yi, zi, xf, yf, zf, lineRes, fill, stroke); 
}
ofParameter<bool> renderer::createHermite(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, int lineRes, ofColor fillColor, ofColor strokeColor) { 
	ofPath* herm = new ofPath();
	ofVec3f position;
	ofVec3f cp1;
	cp1.x = xi;
	cp1.y = yi;
	cp1.z = zi;
	ofVec3f cp2;
	cp2.x = cx1;
	cp2.y = cy1;
	cp2.z = cz1;
	ofVec3f cp3;
	cp3.x = cx2;
	cp3.y = cy2;
	cp3.z = cz2;
	ofVec3f cp4;
	cp4.x = xf;
	cp4.y = yf;
	cp4.z = zf;
	herm->setMode(ofPath::POLYLINES);
	herm->moveTo(xi, yi, zi);
	herm->setColor(fillColor);
	herm->setStrokeColor(strokeColor);
	herm->setStrokeWidth(strokeThickness);
	ofVec3f tangent1 = cp2 - position;
	ofVec3f tangent2 = cp3 - cp4;
	for (int i = 0; i <= lineRes; ++i) {
		hermite(
			i / (float)lineRes,
			cp1.x, cp1.y, cp1.z,
			tangent1.x, tangent1.y, tangent1.z,
			tangent2.x, tangent2.y, tangent2.z,
			cp4.x, cp4.y, cp4.z,
			position.x, position.y, position.z);
		herm->curveTo(position);
	}
	
	primitive2d prim = primitive2d{ herm, fillColor, strokeColor, strokeThickness };
	prim.setName("Hermite " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}
/**
* Creates CatmullRom spline with given control points and line resolution.
*/
ofParameter<bool> renderer::createCatmullRom(const ofPoint cp1, const ofPoint cp2, const ofPoint to, const ofPoint cp4, int lineRes) {
	return createCatmullRom(cp1, cp2, to, cp4, lineRes, fill, stroke);
}

ofParameter<bool> renderer::createCatmullRom(const ofPoint cp1, const ofPoint cp2, const ofPoint to, const ofPoint cp4, int lineRes, ofColor fillColor, ofColor strokeColor) {
	ofPath* catmullRom = new ofPath();
	catmullRom->moveTo(cp2);
	catmullRom->curveTo(cp1);
	catmullRom->curveTo(cp2);
	catmullRom->curveTo(to);
	catmullRom->curveTo(cp4);
	catmullRom->setColor(fillColor);
	catmullRom->setStrokeColor(strokeColor);
	catmullRom->setStrokeWidth(strokeThickness);
	primitive2d prim = primitive2d{ catmullRom, fillColor, strokeColor, strokeThickness };
	prim.setName("Catmull Rom " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

ofParameter<bool> renderer::createSurface(int w, int h, int dim, int res, const ofPoint cp1, const ofPoint cp2, const ofPoint cp3, const ofPoint cp4) {
	return createSurface(w, h, dim, res, cp1, cp2, cp3, cp4, fill, stroke);
}

ofParameter<bool> renderer::createSurface(int w, int h, int dim, int res, const ofPoint cp1, const ofPoint cp2, const ofPoint cp3, const ofPoint cp4, ofColor fillColor, ofColor strokeColor) {
	ofxBezierSurface* surface = new ofxBezierSurface();
	surface->setup(w, h, dim, res);
	std::vector<ofPoint> pts;
	pts.push_back(cp1);
	pts.push_back(cp2);
	pts.push_back(cp3);
	pts.push_back(cp4);
	surface->setControlPnts(pts);
	primitiveTopo prim = primitiveTopo{ surface, fillColor, strokeColor, strokeThickness };
	prim.setName("Bezier Surface " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;
}

//-------------3D primitives-----------------------
ofParameter<bool> renderer::createCube(int x, int y, int z, int w, int h, int d)
{
	return createCube(x, y, z, w, h, d, activeMaterial);
}

ofParameter<bool> renderer::createCube(int x, int y, int z, int w, int h, int d, ofMaterial mat)
{
	setMustPrepares();

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
		//box->setSideColor(i, fillCol);
	}

	//ofMesh boxMesh = box->getMesh();

	primitive3d prim = primitive3d{ box, ofColor(), matrix };
	prim.setName("Cube " + to_string(scn->nbElements() + 1));
	prim.setName("Cube " + to_string(scn->nbElements() + 1));
	prim.setMirror(scn->nbElements() == 6);
	prim.setGlass(false);

	scn->addElement(prim);
	return prim.selected;

	//cout << *scn;
}

ofParameter<bool> renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createSphere(x, y, z, sizeX, sizeY, sizeZ, activeMaterial);
}

ofParameter<bool> renderer::createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat)
{
	setMustPrepares();

	ofSpherePrimitive* ball = new ofSpherePrimitive();
	ball->setPosition(0, 0, 0);
	ball->setResolution(64);// (sizeX + sizeY + sizeZ) / 120);

	float smallest = min(sizeX, min(sizeY, sizeZ));

	ball->setRadius(smallest / 2);

	float newX = (float)sizeX / smallest;
	float newY = (float)sizeY / smallest;
	float newZ = (float)sizeZ / smallest;

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(newX, newY, newZ);
	matrix.setTranslation(x, y, z);

	/*ofMesh * ballMesh = ball->getMeshPtr();
	vector<ofMeshFace> allFaces = ballMesh->getUniqueFaces();

	for (auto& f : allFaces)
	{
		ballMesh->addColor(color);
		f.setHasColors(true);
		f.setColor(0, color);
	}

	ballMesh->enableColors();*/

	primitive3d prim = primitive3d{ ball, ofColor(), matrix };
	prim.setName("Sphere " + to_string(scn->nbElements() + 1));
	prim.setMirror(false);
	prim.setGlass(false);
	scn->addElement(prim);
	return prim.selected;
}

ofParameter<bool> renderer::createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createCone(x, y, z, sizeX, sizeY, sizeZ, activeMaterial);
}

ofParameter<bool> renderer::createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat)
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

	primitive3d prim = primitive3d{ cone, ofColor(), matrix };
	prim.setName("Cone " + to_string(scn->nbElements() + 1));
	scn->addElement(prim);
	return prim.selected;

}

ofParameter<bool>  renderer::createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ)
{
	return createIcecream(x, y, z, sizeX, sizeY, sizeZ, activeMaterial);
}

ofParameter<bool>  renderer::createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat)
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

	forme3d forme{ ball, ofColor(), matrix };
	forme.addPrimitive(cone);
	forme.setName("IceCream " + to_string(scn->nbElements() + 1));
	scn->addElement(forme);
	return forme.selected;
}

//------- Light --------
ofParameter<bool> renderer::createDirectionalLight(int ax, int ay, int az, ofColor difCol, ofColor specCol)
{
	ofLight* ofl = new ofLight();
	ofl->setDirectional();
	//ofl->enable();
	lightShader.useLight(ofl);
	ofMatrix4x4 matrix = ofMatrix4x4();
	ofQuaternion rotate{};
	rotate.makeRotate(ax, ofVec3f(1, 0, 0), ay, ofVec3f(0, 1, 0), az, ofVec3f(0, 0, 1));
	matrix.setRotate(rotate);

	light* l = new light{ ofl, matrix };
	l->setName("Directional " + to_string(scn->nbElements() + 1));
	l->setDiffuseColor(difCol);
	l->setSpecularColor(specCol);
	l->setLightShader(&lightShader);
	scn->addElement(l);
	return l->selected;
}

ofParameter<bool> renderer::createPonctualLight(int x, int y, int z, ofColor difCol, ofColor specCol)
{
	ofLight* ofl = new ofLight();
	ofl->setPointLight();
	//ofl->enable();
	lightShader.useLight(ofl);
	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.setTranslation(x, y, z);

	light* l = new light{ ofl, matrix };
	l->setName("Pontual " + to_string(scn->nbElements() + 1));
	l->setDiffuseColor(difCol);
	l->setSpecularColor(specCol);
	l->setLightShader(&lightShader);
	scn->addElement(l);
	return l->selected;
}

ofParameter<bool> renderer::createSpotlight(ofVec3f pos, int ax, int ay, int az, ofColor difCol, ofColor specCol)
{
	ofLight* ofl = new ofLight();
	ofl->setSpotlight();
	//ofl->enable();
	lightShader.useLight(ofl);
	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.setTranslation(pos);
	ofQuaternion rotate{};
	rotate.makeRotate(ax, ofVec3f(1, 0, 0), ay, ofVec3f(0, 1, 0), az, ofVec3f(0, 0, 1));
	matrix.setRotate(rotate);

	light* l = new light{ ofl, matrix };
	l->setName("Spotlight " + to_string(scn->nbElements() + 1));
	l->setDiffuseColor(difCol);
	l->setSpecularColor(specCol);
	l->setLightShader(&lightShader);
	scn->addElement(l);
	return l->selected;
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
	primitive* toSelect;
	float distance = -1;
	for (primitive& p : *scn)
	{
		vector<hit> hits = p.intersectsMeshInstance(ofVec2f(x, y), (**cam));
		if (hits.size() > 0 && (distance == -1 || hits[0].distance < distance))
		{
			distance = hits[0].distance;
			toSelect = &p;
		}
	}

	if (distance > -0.9)
	{
		toSelect->changeSelected();
	}
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

void renderer::setIlluminationModel(illuminationModel model) {
	if (model == PHONG) {
		lightShader.setLightingMethod(ofxShadersFX::Lighting::PHONG_LIGHTING);
	} else {
		lightShader.setLightingMethod(ofxShadersFX::Lighting::BLINN_PHONG_LIGHTING);
	}
}
