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
	setupMenuTopo();
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
	else if (key == 'w') {
		ofLog() << "<app::wireFrameModeChanged>";
		rend->changeWireFrameMode();
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
	groupPrimitiveType2D.add(primTypeTopo);
	groupPrimitiveType2D.add(primTypeCube);
	groupPrimitiveType2D.add(primTypeSphere);
	groupPrimitiveType2D.add(primTypeTriangle);
	groupPrimitiveType2D.add(primTypeLine);
	groupPrimitiveType2D.add(primTypePoint);

	groupPrimitiveType3D.setName("Type");
	groupPrimitiveType3D.add(primType2D);
	groupPrimitiveType3D.add(primType3D);
	groupPrimitiveType3D.add(primTypeTopo);
	groupPrimitiveType3D.add(primTypeCube);
	groupPrimitiveType3D.add(primTypeSphere);
	groupPrimitiveType3D.add(primTypeTriangle);
	groupPrimitiveType3D.add(primTypeLine);

	groupPrimitiveTypeTopo.setName("Type");
	groupPrimitiveTypeTopo.add(primType2D);
	groupPrimitiveTypeTopo.add(primType3D);
	groupPrimitiveTypeTopo.add(primTypeTopo);
	groupPrimitiveTypeTopo.add(primTypeBezier);
	groupPrimitiveTypeTopo.add(primTypeHermite);
	groupPrimitiveTypeTopo.add(primTypeCatmullRom);
	groupPrimitiveTypeTopo.add(primTypeSurface);

	groupPrimitivePosition2D.setName("Position");
	groupPrimitivePosition2D.add(primPosX.set(primPosX));
	groupPrimitivePosition2D.add(primPosY.set(primPosY));

	groupPrimitivePosition3D.setName("Position");
	groupPrimitivePosition3D.add(primPosX.set(primPosX));
	groupPrimitivePosition3D.add(primPosY.set(primPosY));
	groupPrimitivePosition3D.add(primPosZ.set(primPosZ));

	groupPrimitivePositionTopo.setName("Position");
	groupPrimitivePositionTopo.add(primPosX.set(primPosX));
	groupPrimitivePositionTopo.add(primPosY.set(primPosY));
	groupPrimitivePositionTopo.add(primPosZ.set(primPosZ));
	groupPrimitivePositionTopo.add(primSizeWidth.set(primSizeWidth));
	groupPrimitivePositionTopo.add(primSizeHeight.set(primSizeHeight));
	groupPrimitivePositionTopo.add(primSizeDepth.set(primSizeDepth));

	groupPrimitiveSize2D.setName("Taille");
	groupPrimitiveSize2D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSize2D.add(primSizeHeight.set(primSizeHeight));

	groupPrimitiveSize3D.setName("Taille");
	groupPrimitiveSize3D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSize3D.add(primSizeHeight.set(primSizeHeight));
	groupPrimitiveSize3D.add(primSizeDepth.set(primSizeDepth));

	groupPrimitiveSizeTopo.setName("Controle");
	groupPrimitiveSizeTopo.add(primPosX2.set(primPosX2));
	groupPrimitiveSizeTopo.add(primPosY2.set(primPosY2));
	groupPrimitiveSizeTopo.add(primPosZ2.set(primPosZ2));
	groupPrimitiveSizeTopo.add(primPosX3.set(primPosX3));
	groupPrimitiveSizeTopo.add(primPosY3.set(primPosY3));
	groupPrimitiveSizeTopo.add(primPosZ3.set(primPosZ3));
	groupPrimitiveSizeTopo.add(primPosX4.set(primPosX4));
	groupPrimitiveSizeTopo.add(primPosY4.set(primPosY4));
	groupPrimitiveSizeTopo.add(primPosZ4.set(primPosZ4));
	groupPrimitiveSizeTopo.add(primPosX5.set(primPosX5));
	groupPrimitiveSizeTopo.add(primPosY5.set(primPosY5));
	groupPrimitiveSizeTopo.add(primPosZ5.set(primPosZ5));

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
}

void ofApp::initButtonListener() {
	btnDrawPrimitive.addListener(this, &ofApp::btnDrawPrimitiveClicked);
	btnClear.addListener(this, &ofApp::btnClearClicked);
	btnExit.addListener(this, &ofApp::btnExitClicked);

	btnExport.addListener(this, &ofApp::btnExportClicked);
	btnImport.addListener(this, &ofApp::btnImportClicked);

	btnApplySelect.addListener(this, &ofApp::btnApplySelectClicked);
}

void ofApp::initOfParameters() {

	primType2D.setName("2D");
	primType2D.set(true);
	primType2D.addListener(this, &ofApp::primDim2DChanged);

	primType3D.setName("3D");
	primType3D.set(false);
	primType3D.addListener(this, &ofApp::primDim3DChanged);

	primTypeTopo.setName("Topologie");
	primTypeTopo.set(false);
	primTypeTopo.addListener(this, &ofApp::primTopoChanged);

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

	primTypeBezier.setName("Bezier");
	primTypeBezier.set(false);
	primTypeBezier.addListener(this, &ofApp::primTypeBezierChanged);

	primTypeHermite.setName("Hermite");
	primTypeHermite.set(false);
	primTypeHermite.addListener(this, &ofApp::primTypeHermiteChanged);

	primTypeCatmullRom.setName("CatmullRom");
	primTypeCatmullRom.set(false);
	primTypeCatmullRom.addListener(this, &ofApp::primTypeCatmullRomChanged);

	primTypeSurface.setName("Surface de Bezier");
	primTypeSurface.set(false);
	primTypeSurface.addListener(this, &ofApp::primTypeSurfaceChanged);

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

	primPosX2.setName("X1");
	primPosX2.setMin(MinX);
	primPosX2.setMax(MaxX);
	primPosX2.set((MinX + MaxX) / 2);

	primPosY2.setName("Y1");
	primPosY2.setMin(MinY);
	primPosY2.setMax(MaxY);
	primPosY2.set((MinY + MaxY) / 2);

	primPosZ2.setName("Z1");
	primPosZ2.setMin(MinZ);
	primPosZ2.setMax(MaxZ);
	primPosZ2.set((MinZ + MaxZ) / 2);

	primPosX3.setName("X2");
	primPosX3.setMin(MinX);
	primPosX3.setMax(MaxX);
	primPosX3.set((MinX + MaxX) / 2);

	primPosY3.setName("Y2");
	primPosY3.setMin(MinY);
	primPosY3.setMax(MaxY);
	primPosY3.set((MinY + MaxY) / 2);

	primPosZ3.setName("Z2");
	primPosZ3.setMin(MinZ);
	primPosZ3.setMax(MaxZ);
	primPosZ3.set((MinZ + MaxZ) / 2);

	primPosX4.setName("X3");
	primPosX4.setMin(MinX);
	primPosX4.setMax(MaxX);
	primPosX4.set((MinX + MaxX) / 2);

	primPosY4.setName("Y3");
	primPosY4.setMin(MinY);
	primPosY4.setMax(MaxY);
	primPosY4.set((MinY + MaxY) / 2);

	primPosZ4.setName("Z3");
	primPosZ4.setMin(MinZ);
	primPosZ4.setMax(MaxZ);
	primPosZ4.set((MinZ + MaxZ) / 2);

	primPosX5.setName("X2");
	primPosX5.setMin(MinX);
	primPosX5.setMax(MaxX);
	primPosX5.set((MinX + MaxX) / 2);

	primPosY5.setName("Y2");
	primPosY5.setMin(MinY);
	primPosY5.setMax(MaxY);
	primPosY5.set((MinY + MaxY) / 2);

	primPosZ5.setName("Z2");
	primPosZ5.setMin(MinZ);
	primPosZ5.setMax(MaxZ);
	primPosZ5.set((MinZ + MaxZ) / 2);

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

	wireFrame.setName("Mode file de fer");
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

	applyAll.setName("Appliquer a la scene");
	applyAll.set(true);
	applyAll.addListener(this, &ofApp::applyAllChanged);

	blur.setName("Brouiller");
	blur.set(false);
	blur.addListener(this, &ofApp::blurChanged);

	invert.setName("Inverser");
	invert.set(false);
	invert.addListener(this, &ofApp::invertChanged);

	dilate.setName("Dilater");
	dilate.set(false);
	dilate.addListener(this, &ofApp::dilateChanged);
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
	else if (primType3D.get()) {
		if (primTypeCube.get()) {
			selectionMenu.add(rend->createCube(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
		else if (primTypeSphere.get()) {
			selectionMenu.add(rend->createSphere(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
		else if (primTypeTriangle.get()) {
			selectionMenu.add(rend->createCone(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
		else {
			selectionMenu.add(rend->createIcecream(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
	}
	else {
		if (primTypeBezier.get()) {
			selectionMenu.add(rend->createBezier(primPosX2, primPosY2, primPosZ2, primPosX3, primPosY3, primPosZ3, primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth));
		}
		else if (primTypeHermite.get()) {
			selectionMenu.add(rend->createHermite(primPosX2, primPosY2, primPosZ2, primPosX3, primPosY3, primPosZ3, primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth, 100));
		}
		else if (primTypeCatmullRom.get()) {
			selectionMenu.add(rend->createCatmullRom(ofPoint(primPosX2, primPosY2, primPosZ2), ofPoint(primPosX, primPosY, primPosZ), ofPoint(primSizeWidth, primSizeHeight, primSizeDepth), ofPoint(primPosX3, primPosY3, primPosZ3), 100));
		}
		else {
			selectionMenu.add(rend->createSurface(500, 500, 2, 20, ofPoint(primPosX2, primPosY2, primPosZ2), ofPoint(primPosX3, primPosY3, primPosZ3), ofPoint(primPosX4, primPosY4, primPosZ4), ofPoint(primPosX5, primPosY5, primPosZ5)));
		}
	}

}

void ofApp::btnClearClicked() {
	rend->clearPrimitives();
	selectionMenu.clear();
}

void ofApp::btnExitClicked()
{
	ofLog() << "<app::btnExitClicked>";
	std::exit(0);
}

void ofApp::btnExportClicked()
{
	rend->imageExport("render", "png");
}

void ofApp::btnApplySelectClicked() {

	ofMatrix4x4 matrix = ofMatrix4x4();
	matrix.scale(proportionX, proportionY, proportionZ);

	float rotX = (rotateX * PI) / 180;
	float rotY = (rotateY * PI) / 180;
	float rotZ = (rotateZ * PI) / 180;

	ofMatrix4x4 matrixX = ofMatrix4x4(1, 0, 0, 0, 0, cos(rotX), -sin(rotX), 0, 0, sin(rotX), cos(rotX), 0, 0, 0, 0, 1);
	ofMatrix4x4 matrixY = ofMatrix4x4(cos(rotY), 0, sin(rotY), 0, 0, 1, 0, 0, -sin(rotY), 0, cos(rotY), 0, 0, 0, 0, 1);
	ofMatrix4x4 matrixZ = ofMatrix4x4(cos(rotZ), -sin(rotZ), 0, 0, sin(rotZ), cos(rotZ), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	matrix = matrix * matrixX;
	matrix = matrix * matrixY;
	matrix = matrix * matrixZ;

	matrix.setTranslation(translateX, translateY, translateZ);

	rend->applySelection(matrix);
}

void ofApp::applyAllChanged(bool& value) {
	if (applyAll) {
		btnApplySelect.removeListener(this, &ofApp::btnApplySelectClicked);
		
		float deltaX = rend->getDeltaX();
		float deltaY = rend->getDeltaY();
		float deltaZ = rend->getDeltaZ();
		float centerX = rend->getCenterX();
		float centerY = rend->getCenterY();
		float centerZ = rend->getCenterZ();
		float scaleX = rend->getScaleX();
		float scaleY = rend->getScaleY();
		float scaleZ = rend->getScaleZ();

		translateX.set(deltaX);
		translateY.set(deltaY);
		translateZ.set(deltaZ);
		rotateX.set(centerX);
		rotateY.set(centerY);
		rotateZ.set(centerZ);
		proportionX.set(scaleX);
		proportionY.set(scaleY);
		proportionZ.set(scaleZ);
		
	}
	else {
		btnApplySelect.addListener(this, &ofApp::btnApplySelectClicked);

		translateX.set((MinX + MaxX) / 2);
		translateY.set((MinY + MaxY) / 2);
		translateZ.set((MinZ + MaxZ) / 2);
		rotateX.set(0);
		rotateY.set(0);
		rotateZ.set(0);
		proportionX.set(1);
		proportionY.set(1);
		proportionZ.set(1);

	}
}

void ofApp::primDim2DChanged(bool& value) {
	if (primType2D.get()) {
		primType3D.set(false);
		primTypeTopo.set(false);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
		primTypeTriangle.setName("Triangle");
		primTypeLine.setName("Ligne");
	}
	else if (primType3D.get()) {
		primType3D.set(true);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
		primTypeTriangle.setName("Cone");
		primTypeLine.setName("IceCream");
	}
	else {
		primTypeTopo.set(true);
		primSizeWidth.setName("Xf");
		primSizeHeight.setName("Yf");
		primSizeDepth.setName("Zf");
	}

}

void ofApp::primDim3DChanged(bool& value) {
	if (primType3D.get()) {
		primType2D.set(false);
		primTypeTopo.set(false);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
		primTypeTriangle.setName("Cone");
		primTypeLine.setName("IceCream");
	}
	else if (primType2D.get()) {
		primType2D.set(true);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
		primTypeTriangle.setName("Triangle");
		primTypeLine.setName("Ligne");
	}
	else {
		primTypeTopo.set(true);
		primSizeWidth.setName("Xf");
		primSizeHeight.setName("Yf");
		primSizeDepth.setName("Zf");
	}

}

void ofApp::primTopoChanged(bool& value) {
	if (primTypeTopo.get()) {
		primType2D.set(false);
		primType3D.set(false);
	}
	else if (primType2D.get()) {
		primType2D.set(true);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
		primTypeTriangle.setName("Triangle");
		primTypeLine.setName("Ligne");
		primSizeWidth.setName("Hauteur");
		primSizeHeight.setName("Largeur");
		primSizeDepth.setName("Profondeur");
	}
	else {
		primType3D.set(true);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
		primTypeTriangle.setName("Cone");
		primTypeLine.setName("IceCream");
		primSizeWidth.setName("Hauteur");
		primSizeHeight.setName("Largeur");
		primSizeDepth.setName("Profondeur");
	}

}

void ofApp::primTypeCubeChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();
	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();

	if (primTypeCube.get()) {
		primTypeSphere.set(false);
		primTypeTriangle.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
		primTypeBezier.set(false);
		primTypeHermite.set(false);
	}
	else
		primTypeCube.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
	primTypeBezier.enableEvents();
	primTypeHermite.enableEvents();
}

void ofApp::primTypeSphereChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();
	primTypeBezier.disableEvents();
	primTypeBezier.disableEvents();

	if (primTypeSphere.get()) {
		primTypeCube.set(false);
		primTypeTriangle.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
		primTypeBezier.set(false);
		primTypeHermite.set(false);
	}
	else
		primTypeSphere.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
	primTypeBezier.enableEvents();
	primTypeHermite.enableEvents();
}

void ofApp::primTypeTriangleChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();
	primTypeBezier.disableEvents();
	primTypeBezier.disableEvents();

	if (primTypeTriangle.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeLine.set(false);
		primTypePoint.set(false);
		primTypeBezier.set(false);
		primTypeHermite.set(false);
	}
	else
		primTypeTriangle.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
	primTypeBezier.enableEvents();
	primTypeHermite.enableEvents();
}

void ofApp::primTypeLineChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();
	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();

	if (primTypeLine.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeTriangle.set(false);
		primTypePoint.set(false);
		primTypeBezier.set(false);
		primTypeHermite.set(false);
	}
	else
		primTypeLine.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
	primTypeBezier.enableEvents();
	primTypeHermite.enableEvents();
}

void ofApp::primTypePointChanged(bool& value) {

	primTypeSphere.disableEvents();
	primTypeCube.disableEvents();
	primTypeTriangle.disableEvents();
	primTypeLine.disableEvents();
	primTypePoint.disableEvents();
	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();

	if (primTypePoint.get())
	{
		primTypeCube.set(false);
		primTypeSphere.set(false);
		primTypeLine.set(false);
		primTypeTriangle.set(false);
		primTypeBezier.set(false);
		primTypeHermite.set(false);
	}
	else
		primTypePoint.set(true);


	primTypeSphere.enableEvents();
	primTypeCube.enableEvents();
	primTypeTriangle.enableEvents();
	primTypeLine.enableEvents();
	primTypePoint.enableEvents();
	primTypeBezier.enableEvents();
	primTypeHermite.enableEvents();
}

void ofApp::primTypeBezierChanged(bool& value) {

	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();
	primTypeCatmullRom.disableEvents();
	primTypeSurface.disableEvents();

	if (primTypeBezier.get())
	{
		primTypeHermite.set(false);
		primTypeCatmullRom.set(false);
		primTypeSurface.set(false);
		primSizeWidth.setName("Xf");
		primSizeHeight.setName("Yf");
		primSizeDepth.setName("Zf");
		primPosX4.setName("inutile ici");
		primPosY4.setName("inutile ici");
		primPosZ4.setName("inutile ici");
		primPosX5.setName("inutile ici");
		primPosY5.setName("inutile ici");
		primPosZ5.setName("inutile ici");
	}
	else
		primTypeBezier.set(true);

	primTypeHermite.enableEvents();
	primTypeBezier.enableEvents();
	primTypeCatmullRom.enableEvents();
	primTypeSurface.enableEvents();
}

void ofApp::primTypeHermiteChanged(bool& value) {

	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();
	primTypeCatmullRom.disableEvents();
	primTypeSurface.disableEvents();

	if (primTypeHermite.get())
	{
		primTypeBezier.set(false);
		primTypeCatmullRom.set(false);
		primTypeSurface.set(false);
		primSizeWidth.setName("Xf");
		primSizeHeight.setName("Yf");
		primSizeDepth.setName("Zf");
		primPosX4.setName("inutile ici");
		primPosY4.setName("inutile ici");
		primPosZ4.setName("inutile ici");
		primPosX5.setName("inutile ici");
		primPosY5.setName("inutile ici");
		primPosZ5.setName("inutile ici");
	}
	else
		primTypeHermite.set(true);

	primTypeHermite.enableEvents();
	primTypeBezier.enableEvents();
	primTypeCatmullRom.enableEvents();
	primTypeSurface.enableEvents();
}

void ofApp::primTypeCatmullRomChanged(bool& value) {

	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();
	primTypeCatmullRom.disableEvents();
	primTypeSurface.disableEvents();

	if (primTypeCatmullRom.get())
	{
		primTypeBezier.set(false);
		primTypeHermite.set(false);
		primTypeSurface.set(false);
		primSizeWidth.setName("Xf");
		primSizeHeight.setName("Yf");
		primSizeDepth.setName("Zf");
		primPosX4.setName("inutile ici");
		primPosY4.setName("inutile ici");
		primPosZ4.setName("inutile ici");
		primPosX5.setName("inutile ici");
		primPosY5.setName("inutile ici");
		primPosZ5.setName("inutile ici");
	}
	else
		primTypeCatmullRom.set(true);

	primTypeHermite.enableEvents();
	primTypeBezier.enableEvents();
	primTypeCatmullRom.enableEvents();
	primTypeSurface.enableEvents();
}

void ofApp::primTypeSurfaceChanged(bool& value) {

	primTypeBezier.disableEvents();
	primTypeHermite.disableEvents();
	primTypeCatmullRom.disableEvents();
	primTypeSurface.disableEvents();

	if (primTypeSurface.get())
	{
		primTypeBezier.set(false);
		primTypeHermite.set(false);
		primTypeCatmullRom.set(false);
		primSizeWidth.setName("Largeur");
		primSizeHeight.setName("Hauteur");
		primSizeDepth.setName("");
		primPosX4.setName("X3");
		primPosY4.setName("Y3");
		primPosZ4.setName("Z3");
		primPosX5.setName("X4");
		primPosY5.setName("Y4");
		primPosZ5.setName("Z4");
	}
	else 
		primTypeSurface.set(true);

	primTypeHermite.enableEvents();
	primTypeBezier.enableEvents();
	primTypeCatmullRom.enableEvents();
	primTypeSurface.enableEvents();
}

void ofApp::wireFrameChanged(bool& value) {

	ofLog() << "<app::wireFrameModeChanged>";
	rend->setWireFrameMode(value);
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
	if (applyAll) {
		rend->sceneTranslate(translateX, translateY, translateZ);
	}
}

void ofApp::rotateChanged(float& value) {
	if (applyAll) {
		rend->sceneRotate(45, rotateX, rotateY, rotateZ);
	}
}

void ofApp::scaleChanged(float& value) {
	if (applyAll) {
		rend->sceneScale(proportionX, proportionY, proportionZ);
	}
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
	else if (primType3D)
		menu3D.draw();
	else
		menuTopo.draw();

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

void ofApp::setupMenuTopo() {

	menuTopo.setDefaultWidth(200);

	menuTopo.setup();

	menuTopo.add(groupPrimitiveTypeTopo);
	menuTopo.add(groupPrimitivePositionTopo);
	menuTopo.add(groupPrimitiveSizeTopo);

	menuTopo.add(groupStroke);

	menuTopo.add(groupTexture);

	menuTopo.minimizeAll();

	menuTopo.registerMouseEvents();
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

	transformationMenu.add(applyAll);
	transformationMenu.add(btnApplySelect.setup("Modifier la selection"));

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
	optionMenu.add(btnClear.setup("Supprimer la scene"));

	optionMenu.add(groupBackground);

	optionMenu.add(groupWireFrame);

	optionMenu.add(btnImport.setup("Importation de modele"));
	optionMenu.add(btnExport.setup("Exportation en image"));

	optionMenu.add(btnExit.setup("Quitter"));

	optionMenu.registerMouseEvents();
}

void ofApp::updatePositionMenu() {
	menu2D.setPosition(10, 260);
	menu3D.setPosition(10, 260);
	menuTopo.setPosition(10, 260);

	//cameraMenu.setPosition(ofGetWindowWidth() - 280, 10);
	//transformationMenu.setPosition(ofGetWindowWidth() - 280, 260);
	//filterMenu.setPosition(ofGetWindowWidth() - 280, 540);
	optionMenu.setPosition(10, 10);
}

//Source : MsDN (la place où que tu vas quand tu veux savoir comment ça marche microsoft)
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
						ofParameter<bool> param = rend->importModel(strPath);

						if (param.get() == false)
						{
							selectionMenu.add(param);
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