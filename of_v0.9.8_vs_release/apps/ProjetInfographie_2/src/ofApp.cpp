#include "ofApp.h"
#include "ccamera.h"
#include <windows.h>      // For common windows data types and function headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents

ofApp::ofApp()
{
	rend = nullptr;
	scn = nullptr;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Visualiseur interactif de scenes 3D");

	rend = new renderer();
	scn = new scene();
	cam = new ccamera();
	rend->setScene(scn);
	rend->setCamera(cam);
	rend->setup();

	initOfParameters();
	initGroups();

	setupMenu2D();
	setupMenu3D();
	setupCameraMenu();
	setupTransformationMenu();
	setupOptionMenu();
	setupSelectionMenu();

	initButtonListener();

	isKeyPressDown = false;
	isKeyPressUp = false;
	isKeyPressPageDown = false;
	isKeyPressPageUp = false;
	isKeyPressLeft = false;
	isKeyPressRight = false;

	ofLog() << "<app::setup>";
}

//--------------------------------------------------------------
void ofApp::update()
{
	setColors();
	setRendererParameter();
	updatePositionMenu();

	rend->update();

	updateKeys();
}

void ofApp::updateKeys()
{
	cam->isCameraMoveDown = isKeyPressDown;
	cam->isCameraMoveUp = isKeyPressUp;
	cam->isCameraMoveBackward = isKeyPressPageDown;
	cam->isCameraMoveForward = isKeyPressPageUp;
	cam->isCameraMoveLeft = isKeyPressLeft;
	cam->isCameraMoveRight = isKeyPressRight;
}


//--------------------------------------------------------------
void ofApp::draw()
{
	rend->draw();
	drawMenus();
}

ofApp::~ofApp()
{
	if (nullptr != rend)
		delete rend;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_LEFT)
	{
		isKeyPressLeft = true;
		ofLog() << "<app::keyPressedLeft>";
	}
	else if (key == OF_KEY_RIGHT)
	{
		isKeyPressRight = true;
		ofLog() << "<app::keyPressedRight>";
	}
	else if (key == OF_KEY_UP)
	{
		isKeyPressUp = true;
		ofLog() << "<app::keyPressedUp>";
	}
	else if (key == OF_KEY_DOWN)
	{
		isKeyPressDown = true;
		ofLog() << "<app::keyPressedDown>";
	}
	else if (key == OF_KEY_PAGE_DOWN)
	{
		isKeyPressPageDown = true;
		ofLog() << "<app::keyPressedPageDown>";
	}
	else if (key == OF_KEY_PAGE_UP)
	{
		isKeyPressPageUp = true;
		ofLog() << "<app::keyPressedPageUp>";
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == ' ')
		rend->imageExport("render", "png");
	else if (key == 'r')
	{
		ofLog() << "rotate X:" << rotateX << "Y:" << rotateY << "Z:" << rotateZ;
		rend->sceneRotate(45, rotateX, rotateY, rotateZ);
	}
	else if (key == 't')
	{
		ofLog() << "translate X:" << translateX << "Y:" << translateY << "Z:" << translateZ;
		rend->sceneTranslate(translateX, translateY, translateZ);
	}
	else if (key == 's')
	{
		ofLog() << "scale X:" << translateX << "Y:" << translateY;
		rend->sceneScale(proportionX, proportionY, proportionZ);
	}
	else if (key == 'm')
	{
		//ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		//rend->createCube(rand() % 1004 + 20, rand() % 748 + 20, rand() % 100, 100, c);
		//rend->importModel("..\\..\\Models\\IronMan\\Iron_Man.obj");
	}
	else if (key == 'n')
	{
		//rend->importModel("..\\..\\Models\\IronMan\\Iron_Man.obj");
	}
	else if (key == 'c') {
		ofLog() << "<app::primitiveCleared>";
		rend->clearPrimitives();
	}
	else if (key == 'w') {
		ofLog() << "<app::wireFrameModeChanged>";
		rend->changeWireFrameMode();
	}
	else if (key == 'z') {
		ofLog() << "<app::resetCamera>";
		cam->resetPos();
	}
	else if (key == 'x') {
		ofLog() << "<app::setTarget>";
		cam->setTarget({ 200.0f, 0.0f, 0.0f });
	}
	else if (key == OF_KEY_LEFT)
	{
		isKeyPressLeft = false;
		ofLog() << "<app::keyReleaseLeft>";
	}
	else if (key == OF_KEY_RIGHT)
	{
		isKeyPressRight = false;
		ofLog() << "<app::keyReleaseRight>";
	}
	else if (key == OF_KEY_UP)
	{
		isKeyPressUp = false;
		ofLog() << "<app::keyReleaseUp>";
	}
	else if (key == OF_KEY_DOWN)
	{
		isKeyPressDown = false;
		ofLog() << "<app::keyReleaseDown>";
	}
	else if (key == OF_KEY_PAGE_DOWN)
	{
		isKeyPressPageDown = false;
		ofLog() << "<app::keyReleasePageDown>";
	}
	else if (key == OF_KEY_PAGE_UP)
	{
		isKeyPressPageUp = false;
		ofLog() << "<app::keyReleasePageUp>";
	}
	/*else if (key == 'q')
		rend->changeRotate();*/

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	HCURSOR curs;
	if (!cursorIsInControl(x, y))
	{
		curs = LoadCursor(NULL, IDC_ARROW);
		SetCursor(curs);
	}
}

bool ofApp::cursorIsInControl(int x, int y) {
	vector<string> names = menu2D.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = menu2D.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}

	names = menu3D.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = menu3D.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}

	names = cameraMenu.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = cameraMenu.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}

	names = transformationMenu.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = transformationMenu.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}

	names = filterMenu.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = filterMenu.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}

	names = optionMenu.getControlNames();

	for each (string name in names)
	{
		ofxBaseGui* control = optionMenu.getControl(name);
		ofPoint pos = control->getPosition();
		float h = control->getHeight();
		float w = control->getWidth();
		if (x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h)
			return true;
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	//if (button == )
	rend->selectPrimitive(x, y, GetKeyState(VK_SHIFT));
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	//TODO importer le model3d?
}

void ofApp::initGroups()
{

	groupPrimitiveType2D.setName("Type");
	groupPrimitiveType2D.add(primType2D);
	groupPrimitiveType2D.add(primType3D);
	groupPrimitiveType2D.add(primTypeCube);
	groupPrimitiveType2D.add(primTypeSphere);
	groupPrimitiveType2D.add(primTypeTriangle);
	groupPrimitiveType2D.add(primTypeLine);
	groupPrimitiveType2D.add(primTypePoint);

	groupPrimitiveType3D.setName("Type");
	groupPrimitiveType3D.add(primType2D);
	groupPrimitiveType3D.add(primType3D);
	groupPrimitiveType3D.add(primTypeCube);
	groupPrimitiveType3D.add(primTypeSphere);

	groupPrimitivePosition2D.setName("Position");
	groupPrimitivePosition2D.add(primPosX.set(primPosX));
	groupPrimitivePosition2D.add(primPosY.set(primPosY));

	groupPrimitivePosition3D.setName("Position");
	groupPrimitivePosition3D.add(primPosX.set(primPosX));
	groupPrimitivePosition3D.add(primPosY.set(primPosY));
	groupPrimitivePosition3D.add(primPosZ.set(primPosZ));

	groupPrimitiveSize2D.setName("Taille");
	groupPrimitiveSize2D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSize2D.add(primSizeHeight.set(primSizeHeight));

	groupPrimitiveSize3D.setName("Taille");
	groupPrimitiveSize3D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSize3D.add(primSizeHeight.set(primSizeHeight));
	groupPrimitiveSize3D.add(primSizeDepth.set(primSizeDepth));

	groupThick.setName("Epaisseur des traits");
	groupThick.add(strokeThickness.set(strokeThickness));

	groupFill.setName("Couleur de remplissage");
	groupFill.add(fillHue.set(fillHue));
	groupFill.add(fillSaturation.set(fillSaturation));
	groupFill.add(fillBrightess.set(fillBrightess));
	groupFill.add(fillAlpha.set(fillAlpha));

	groupStroke.setName("Couleur de bordure");
	groupStroke.add(strokeHue.set(strokeHue));
	groupStroke.add(strokeSaturation.set(strokeSaturation));
	groupStroke.add(strokeBrightess.set(strokeBrightess));
	groupStroke.add(strokeAlpha.set(strokeAlpha));

	groupBackground.setName("Couleur de fond");
	groupBackground.add(bgHue.set(bgHue));
	groupBackground.add(bgSaturation.set(bgSaturation));
	groupBackground.add(bgBrightess.set(bgBrightess));

	groupTexture.setName("Texture");
	groupTexture.add(noTexture);
	groupTexture.add(metalTexture);
	groupTexture.add(waterTexture);

	groupWireFrame.setName("Représentation 3D");
	groupWireFrame.add(wireFrame);

	groupTranslate2D.setName("Translation");
	groupTranslate2D.add(translateX);
	groupTranslate2D.add(translateY);

	groupTranslate3D.setName("Translation");
	groupTranslate3D.add(translateX);
	groupTranslate3D.add(translateY);
	groupTranslate3D.add(translateZ);

	groupRotate2D.setName("Rotation");
	groupRotate2D.add(rotateX);
	groupRotate2D.add(rotateY);

	groupRotate3D.setName("Rotation");
	groupRotate3D.add(rotateX);
	groupRotate3D.add(rotateY);
	groupRotate3D.add(rotateZ);

	groupProportion2D.setName("Proportion");
	groupProportion2D.add(proportionX);
	groupProportion2D.add(proportionY);

	groupProportion3D.setName("Proportion");
	groupProportion3D.add(proportionX);
	groupProportion3D.add(proportionY);
	groupProportion3D.add(proportionZ);

	groupFilter.setName("Traitement d'image");
	groupFilter.add(blur);
	groupFilter.add(invert);
	groupFilter.add(dilate);
	/*
	groupSelection.setName("Selection");
	groupSelection.add(selTransfoMatrix);*/
}

/*
void ofApp::selectionChanged()
{

}

void ofApp::transfoChanged(ofMatrix4x4& value)
{

}*/

void ofApp::initButtonListener() {
		btnSelect.addListener(this, &ofApp::btnSelectClicked);
	btnDrawPrimitive.addListener(this, &ofApp::btnDrawPrimitiveClicked);
	btnExit.addListener(this, &ofApp::btnExitClicked);

	btnExport.addListener(this, &ofApp::btnExportClicked);
	btnImport.addListener(this, &ofApp::btnImportClicked);
}

void ofApp::initOfParameters() {

	primType2D.setName("2D");
	primType2D.set(true);
	primType2D.addListener(this, &ofApp::primDim2DChanged);

	primType3D.setName("3D");
	primType3D.set(false);
	primType3D.addListener(this, &ofApp::primDim3DChanged);

	primTypeCube.setName("Carre");
	primTypeCube.set(true);
	primTypeCube.addListener(this, &ofApp::primTypeCubeChanged);

	primTypeSphere.setName("Cercle");
	primTypeSphere.set(false);
	primTypeSphere.addListener(this, &ofApp::primTypeSphereChanged);

	primTypeTriangle.setName("Triangle");
	primTypeTriangle.set(false);
	primTypeTriangle.addListener(this, &ofApp::primTypeTriangleChanged);

	primTypeLine.setName("Ligne");
	primTypeLine.set(false);
	primTypeLine.addListener(this, &ofApp::primTypeLineChanged);

	primTypePoint.setName("Point");
	primTypePoint.set(false);
	primTypePoint.addListener(this, &ofApp::primTypePointChanged);


	primPosX.setName("X");
	primPosX.setMin(MinX);
	primPosX.setMax(MaxX);
	primPosX.set((MinX + MaxX) / 2);

	primPosY.setName("Y");
	primPosY.setMin(MinY);
	primPosY.setMax(MaxY);
	primPosY.set((MinY + MaxY) / 2);

	primPosZ.setName("Z");
	primPosZ.setMin(MinZ);
	primPosZ.setMax(MaxZ);
	primPosZ.set((MinZ + MaxZ) / 2);


	primSizeHeight.setName("Hauteur");
	primSizeHeight.setMin(0);
	primSizeHeight.setMax(MaxY);
	primSizeHeight.set((0 + MaxY) / 2);

	primSizeWidth.setName("Largeur");
	primSizeWidth.setMin(0);
	primSizeWidth.setMax(MaxX);
	primSizeWidth.set((0 + MaxX) / 2);

	primSizeDepth.setName("Profondeur");
	primSizeDepth.setMin(0);
	primSizeDepth.setMax(MaxZ);
	primSizeDepth.set((0 + MaxZ) / 2);

	strokeThickness.setName("Epaisseur");
	strokeThickness.setMin(0);
	strokeThickness.setMax(20);
	strokeThickness.set(3);

	fillHue.setName("Teinte");
	fillHue.setMin(0);
	fillHue.setMax(255);
	fillHue.set(0);

	fillSaturation.setName("Saturation");
	fillSaturation.setMin(0);
	fillSaturation.setMax(255);
	fillSaturation.set(100);

	fillBrightess.setName("Valeur");
	fillBrightess.setMin(0);
	fillBrightess.setMax(255);
	fillBrightess.set(255);

	fillAlpha.setName("Transparence");
	fillAlpha.setMin(0);
	fillAlpha.setMax(255);
	fillAlpha.set(255);


	strokeHue.setName("Teinte");
	strokeHue.setMin(0);
	strokeHue.setMax(255);
	strokeHue.set(0);

	strokeSaturation.setName("Saturation");
	strokeSaturation.setMin(0);
	strokeSaturation.setMax(255);
	strokeSaturation.set(100);

	strokeBrightess.setName("Valeur");
	strokeBrightess.setMin(0);
	strokeBrightess.setMax(255);
	strokeBrightess.set(255);

	strokeAlpha.setName("Transparence");
	strokeAlpha.setMin(0);
	strokeAlpha.setMax(255);
	strokeAlpha.set(255);


	bgHue.setName("Teinte");
	bgHue.setMin(0);
	bgHue.setMax(255);
	bgHue.set(170);

	bgSaturation.setName("Saturation");
	bgSaturation.setMin(0);
	bgSaturation.setMax(255);
	bgSaturation.set(170);

	bgBrightess.setName("Valeur");
	bgBrightess.setMin(0);
	bgBrightess.setMax(255);
	bgBrightess.set(170);

	setColors();

	wireFrame.setName("Forme 3D en file de fer");
	wireFrame.set(true);
	wireFrame.addListener(this, &ofApp::wireFrameChanged);

	noTexture.setName("Aucune");
	noTexture.set(true);
	noTexture.addListener(this, &ofApp::noTextureChanged);

	metalTexture.setName("Metalique");
	metalTexture.set(false);
	metalTexture.addListener(this, &ofApp::metalTextureChanged);

	waterTexture.setName("Aquatique");
	waterTexture.set(false);
	waterTexture.addListener(this, &ofApp::waterTextureChanged);


	translateX.setName("X");
	translateX.setMin(MinX);
	translateX.setMax(MaxX);
	translateX.set((MinX + MaxX) / 2);
	translateX.addListener(this, &ofApp::translateChanged);

	translateY.setName("Y");
	translateY.setMin(MinY);
	translateY.setMax(MaxY);
	translateY.set((MinY + MaxY) / 2);
	translateY.addListener(this, &ofApp::translateChanged);

	translateZ.setName("Z");
	translateZ.setMin(MinZ);
	translateZ.setMax(MaxZ);
	translateZ.set((MinZ + MaxZ) / 2);
	translateZ.addListener(this, &ofApp::translateChanged);

	rotateX.setName("X");
	rotateX.setMin(0);
	rotateX.setMax(360);
	rotateX.set(0);
	rotateX.addListener(this, &ofApp::rotateChanged);

	rotateY.setName("Y");
	rotateY.setMin(0);
	rotateY.setMax(360);
	rotateY.set(0);
	rotateY.addListener(this, &ofApp::rotateChanged);

	rotateZ.setName("Z");
	rotateZ.setMin(0);
	rotateZ.setMax(360);
	rotateZ.set(0);
	rotateZ.addListener(this, &ofApp::rotateChanged);

	proportionX.setName("X");
	proportionX.setMin(0);
	proportionX.setMax(10);
	proportionX.set(1);
	proportionX.addListener(this, &ofApp::scaleChanged);

	proportionY.setName("Y");
	proportionY.setMin(0);
	proportionY.setMax(10);
	proportionY.set(1);
	proportionY.addListener(this, &ofApp::scaleChanged);

	proportionZ.setName("Z");
	proportionZ.setMin(0);
	proportionZ.setMax(10);
	proportionZ.set(1);
	proportionZ.addListener(this, &ofApp::scaleChanged);


	blur.setName("Brouiller");
	blur.set(false);
	blur.addListener(this, &ofApp::blurChanged);

	invert.setName("Inverser");
	invert.set(false);
	invert.addListener(this, &ofApp::invertChanged);

	dilate.setName("Dilater");
	dilate.set(false);
	dilate.addListener(this, &ofApp::dilateChanged);
/*
	selTransfoMatrix.setName("Transformation");
	selTransfoMatrix.set(ofMatrix4x4());
	selTransfoMatrix.addListener(this, &ofApp::transfoChanged);*/
}

void ofApp::setColors()
{
	stroke = ofColor::fromHsb(strokeHue, strokeSaturation, strokeBrightess, strokeAlpha);
	fill = ofColor::fromHsb(fillHue, fillSaturation, fillBrightess, fillAlpha);
	background = ofColor::fromHsb(bgHue, bgSaturation, bgBrightess);
}

void ofApp::setRendererParameter() {

	rend->stroke = stroke;
	rend->fill = fill;
	rend->background = background;

	rend->strokeThickness = strokeThickness;
}

void ofApp::btnSelectClicked()
{

}

void ofApp::btnDrawPrimitiveClicked()
{
	ofLog() << "<app::btnDrawPrimitiveClicked>";

	if (primType2D.get()) {
		if (primTypeCube.get()) {
			selectionMenu.add(rend->createSquare(primPosX, primPosY, primSizeWidth, primSizeHeight));
		}
		else if (primTypeSphere.get()) {
			selectionMenu.add(rend->createCircle(primPosX, primPosY, primSizeWidth, primSizeHeight));
		}
		else if (primTypeTriangle.get()) {
			selectionMenu.add(rend->createTriangle(primPosX, primPosY, primPosX + primSizeWidth, primPosY, (primPosX + primSizeWidth) / 2, primPosY + primSizeHeight));
		}
		else if (primTypeLine.get()) {
			selectionMenu.add(rend->createLine(primPosX, primPosY, primSizeWidth, primSizeHeight));
		}
		else if (primTypePoint.get()) {
			selectionMenu.add(rend->createPoint(primPosX, primPosY, strokeThickness));
		}
	}
	else {
		if (primTypeCube.get()) {
			selectionMenu.add(rend->createCube(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
		else {
			selectionMenu.add(rend->createSphere(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
	}

}

void ofApp::btnExitClicked()
{
	ofLog() << "<app::btnExitClicked>";
	std::exit(0);
}

void ofApp::btnExportClicked()
{
	rend->draw();
	rend->imageExport("render", "png");
}

void ofApp::primDim2DChanged(bool& value) {
	if (primType2D.get()) {
		primType3D.set(false);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
	}
	else {
		primType3D.set(true);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
	}

}

void ofApp::primDim3DChanged(bool& value) {
	if (primType3D.get()) {
		primType2D.set(false);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
	}
	else {
		primType2D.set(true);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
	}

}

void ofApp::primTypeCubeChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();

	if (primTypeCube.get()) {
		primTypeSphere.set(false);
		primTypeTriangle.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
	}
	else
		primTypeCube.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
}

void ofApp::primTypeSphereChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();

	if (primTypeSphere.get()) {
		primTypeCube.set(false);
		primTypeTriangle.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
	}
	else
		primTypeSphere.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
}

void ofApp::primTypeTriangleChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();

	if (primTypeTriangle.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
	}
	else
		primTypeTriangle.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
}

void ofApp::primTypeLineChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();

	if (primTypeLine.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeTriangle.set(false);
		primTypePoint.set(false);
	}
	else
		primTypeLine.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
}

void ofApp::primTypePointChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();

	if (primTypePoint.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeLine.set(false);
		primTypeTriangle.set(false);
	}
	else
		primTypePoint.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
}

void ofApp::wireFrameChanged(bool& value) {

	ofLog() << "<app::wireFrameModeChanged>";
	rend->changeWireFrameMode();
}

void ofApp::noTextureChanged(bool& value) {

	ofLog() << "<app::noTextureChanged>";

	noTexture.disableEvents();
	metalTexture.disableEvents();
	waterTexture.disableEvents();

	noTexture.set(true);
	metalTexture.set(false);
	waterTexture.set(false);



	noTexture.enableEvents();
	metalTexture.enableEvents();
	waterTexture.enableEvents();
}

void ofApp::metalTextureChanged(bool& value) {

	ofLog() << "<app::metalTextureChanged>";

	noTexture.disableEvents();
	metalTexture.disableEvents();
	waterTexture.disableEvents();

	waterTexture.set(false);

	if (metalTexture) {
		noTexture.set(false);
	}
	else {
		noTexture.set(true);
	}



	noTexture.enableEvents();
	metalTexture.enableEvents();
	waterTexture.enableEvents();
}

void ofApp::waterTextureChanged(bool& value) {

	ofLog() << "<app::waterTextureChanged>";

	noTexture.disableEvents();
	metalTexture.disableEvents();
	waterTexture.disableEvents();

	metalTexture.set(false);

	if (waterTexture) {
		noTexture.set(false);
	}
	else {
		noTexture.set(true);
	}



	noTexture.enableEvents();
	metalTexture.enableEvents();
	waterTexture.enableEvents();
}

void ofApp::translateChanged(float& value) {
	rend->sceneTranslate(translateX, translateY, translateZ);
}

void ofApp::rotateChanged(float& value) {

	rend->sceneRotate(45, rotateX, rotateY, rotateZ);
}

void ofApp::scaleChanged(float& value) {
	rend->sceneScale(proportionX, proportionY, proportionZ);
}

void ofApp::blurChanged(bool& value) {
	if (blur)
		rend->addBlur();
	else
		rend->removeBlur();
}

void ofApp::invertChanged(bool& value) {
	if (invert)
		rend->addInvert();
	else
		rend->removeInvert();
}

void ofApp::dilateChanged(bool& value) {
	if (dilate)
		rend->addDilate();
	else
		rend->removeDilate();
}

void ofApp::drawMenus() {

	if (primType2D)
		menu2D.draw();
	else
		menu3D.draw();

	cameraMenu.draw();
	transformationMenu.draw();
	optionMenu.draw();
	selectionMenu.draw();
}

void ofApp::setupMenu2D() {

	menu2D.setDefaultWidth(200);

	menu2D.setup();
	
	menu2D.add(groupPrimitiveType2D);
	menu2D.add(groupPrimitivePosition2D);
	menu2D.add(groupPrimitiveSize2D);

	menu2D.add(groupThick);

	menu2D.add(groupFill);

	menu2D.add(groupStroke);

	menu2D.add(groupTexture);

	menu2D.minimizeAll();

	menu2D.registerMouseEvents();
}

void ofApp::setupMenu3D() {

	menu3D.setDefaultWidth(200);

	menu3D.setup();
	
	menu3D.add(groupPrimitiveType3D);
	menu3D.add(groupPrimitivePosition3D);
	menu3D.add(groupPrimitiveSize3D);

	menu3D.add(groupFill);

	menu3D.add(groupTexture);

	menu3D.minimizeAll();

	menu3D.registerMouseEvents();
}

void ofApp::setupCameraMenu() 
{
	cameraMenu.setDefaultWidth(200);

	cameraMenu.setup();
	cameraMenu.add(cam->getParameterGroup());

	cameraMenu.setPosition(ofGetWindowWidth() - 210, 10);

	cameraMenu.registerMouseEvents();
}

void ofApp::setupTransformationMenu() {

	transformationMenu.setDefaultWidth(200);

	transformationMenu.setup("Menu de Transformation");
	transformationMenu.add(groupTranslate3D);
	transformationMenu.add(groupRotate3D);
	transformationMenu.add(groupProportion3D);
	transformationMenu.add(groupFilter);

	transformationMenu.setPosition(ofGetWindowWidth() - 210, 260);

	transformationMenu.registerMouseEvents();

}

void ofApp::setupFilterMenu() {
	//Jamais utilisé
	filterMenu.setDefaultWidth(200);

	filterMenu.setup();
	filterMenu.add(groupFilter);

	filterMenu.setPosition(ofGetWindowWidth() - 210, 540);

	filterMenu.registerMouseEvents();
}

void ofApp::setupSelectionMenu() {

	selectionMenu.setDefaultWidth(100);

	selectionMenu.setup();

	selectionMenu.setPosition(ofGetWindowWidth() - 320, 10);

	selectionMenu.registerMouseEvents();
}

void ofApp::setupOptionMenu() {

	optionMenu.setDefaultWidth(200);

	optionMenu.setup();

	optionMenu.add(btnDrawPrimitive.setup("Ajouter une primitive"));

	optionMenu.add(groupBackground);

	optionMenu.add(groupWireFrame);

	optionMenu.add(btnSelect.setup("Outils de selection"));

	optionMenu.add(btnImport.setup("Importation de modele"));
	optionMenu.add(btnExport.setup("Exportation en image"));

	optionMenu.add(btnExit.setup("Quitter"));

	optionMenu.registerMouseEvents();
}

void ofApp::updatePositionMenu() {
	menu2D.setPosition(10, 260);
	menu3D.setPosition(10, 260);

	//cameraMenu.setPosition(ofGetWindowWidth() - 280, 10);
	//transformationMenu.setPosition(ofGetWindowWidth() - 280, 260);
	//filterMenu.setPosition(ofGetWindowWidth() - 280, 540);
	optionMenu.setPosition(10, 10);
}

void ofApp::btnImportClicked()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					LPWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{

						std::wstring path = wstring(pszFilePath);
						std::string strPath(path.begin(), path.end());

						ofParameter<bool>* param;

						if (rend->importModel(strPath, param))
						{
							strPath = "Le modele " + strPath + " a ete importe avec succes!";
							path = std::wstring(strPath.begin(), strPath.end());
							LPCWSTR title = (LPCWSTR)path.c_str();
							MessageBox(NULL, title, L"Succes", MB_OK);
						}
						else
						{
							strPath = "Le modele n'a pas pu etre importe.";
							path = std::wstring(strPath.begin(), strPath.end());
							LPCWSTR title = (LPCWSTR)path.c_str();
							MessageBox(NULL, title, L"Echec", MB_OK);
						}

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}