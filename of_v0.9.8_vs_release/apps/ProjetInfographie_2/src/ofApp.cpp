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
	else if (key == 'l')
	{
		selectionMenu.add(rend->createPonctualLight(100, 100, 100, ofColor(255, 255, 255), ofColor(255, 255, 255)));
		ofLog() << "<app::keyReleasel>";
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
	groupPrimitiveType3D.add(primTypeTriangle);
	groupPrimitiveType3D.add(primTypeLine);

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

	groupAmbient.setName("Couleur ambiente");
	groupAmbient.add(ambientHue.set(ambientHue));
	groupAmbient.add(ambientSaturation.set(ambientSaturation));
	groupAmbient.add(ambientBrightess.set(ambientBrightess));

	groupDiff.setName("Couleur de diffusion");
	groupDiff.add(diffHue.set(diffHue));
	groupDiff.add(diffSaturation.set(diffSaturation));
	groupDiff.add(diffBrightess.set(diffBrightess));

	groupEmis.setName("Couleur d'emission");
	groupEmis.add(emisHue.set(emisHue));
	groupEmis.add(emisSaturation.set(emisSaturation));
	groupEmis.add(emisBrightess.set(emisBrightess));

	groupSpec.setName("Couleur speculaire");
	groupSpec.add(specHue.set(specHue));
	groupSpec.add(specSaturation.set(specSaturation));
	groupSpec.add(specBrightess.set(specBrightess));

	groupBackground.setName("Couleur de fond");
	groupBackground.add(bgHue.set(bgHue));
	groupBackground.add(bgSaturation.set(bgSaturation));
	groupBackground.add(bgBrightess.set(bgBrightess));
	

	groupTranslateLight.setName("Position");
	groupTranslateLight.add(translateXLight);
	groupTranslateLight.add(translateYLight);
	groupTranslateLight.add(translateZLight);

	groupRotateLight.setName("Angle");
	groupRotateLight.add(rotateXLight);
	groupRotateLight.add(rotateYLight);
	groupRotateLight.add(rotateZLight);

	groupDiffLight.setName("Couleur de diffusion");
	groupDiffLight.add(diffLightHue.set(diffLightHue));
	groupDiffLight.add(diffLightSaturation.set(diffLightSaturation));
	groupDiffLight.add(diffLightBrightess.set(diffLightBrightess));

	groupSpecLight.setName("Couleur speculaire");
	groupSpecLight.add(specLightHue.set(specLightHue));
	groupSpecLight.add(specLightSaturation.set(specLightSaturation));
	groupSpecLight.add(specLightBrightess.set(specLightBrightess));


	groupLight.setName("Lumiere");
	groupLight.add(directionalLight);
	groupLight.add(ponctualLight);
	groupLight.add(spotLight);
	groupLight.add(groupTranslateLight);
	groupLight.add(groupRotateLight);
	groupLight.add(groupDiffLight);
	groupLight.add(groupSpecLight);


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

	groupCouleur.setName("Couleurs");
	groupCouleur.add(groupAmbient);
	groupCouleur.add(groupDiff);
	groupCouleur.add(groupEmis);
	groupCouleur.add(groupSpec);

}

void ofApp::initButtonListener() {
	btnDrawPrimitive.addListener(this, &ofApp::btnDrawPrimitiveClicked);
	btnAddLight.addListener(this, &ofApp::btnAddLightClicked);
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

	specHue.setName("Teinte");
	specHue.setMin(0);
	specHue.setMax(255);
	specHue.set(0);

	specSaturation.setName("Saturation");
	specSaturation.setMin(0);
	specSaturation.setMax(255);
	specSaturation.set(100);

	specBrightess.setName("Valeur");
	specBrightess.setMin(0);
	specBrightess.setMax(255);
	specBrightess.set(255);

	emisHue.setName("Teinte");
	emisHue.setMin(0);
	emisHue.setMax(255);
	emisHue.set(0);

	emisSaturation.setName("Saturation");
	emisSaturation.setMin(0);
	emisSaturation.setMax(255);
	emisSaturation.set(100);

	emisBrightess.setName("Valeur");
	emisBrightess.setMin(0);
	emisBrightess.setMax(255);
	emisBrightess.set(255);

	diffHue.setName("Teinte");
	diffHue.setMin(0);
	diffHue.setMax(255);
	diffHue.set(0);

	diffSaturation.setName("Saturation");
	diffSaturation.setMin(0);
	diffSaturation.setMax(255);
	diffSaturation.set(100);

	diffBrightess.setName("Valeur");
	diffBrightess.setMin(0);
	diffBrightess.setMax(255);
	diffBrightess.set(255);


	ambientHue.setName("Teinte");
	ambientHue.setMin(0);
	ambientHue.setMax(255);
	ambientHue.set(0);

	ambientSaturation.setName("Saturation");
	ambientSaturation.setMin(0);
	ambientSaturation.setMax(255);
	ambientSaturation.set(100);

	ambientBrightess.setName("Valeur");
	ambientBrightess.setMin(0);
	ambientBrightess.setMax(255);
	ambientBrightess.set(255);


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



	directionalLight.setName("Directionnelle");
	directionalLight.set(true);
	directionalLight.addListener(this, &ofApp::directionalLightChanged);

	ponctualLight.setName("Ponctuelle");
	ponctualLight.set(false);
	ponctualLight.addListener(this, &ofApp::ponctualLightChanged);

	spotLight.setName("Projecteur");
	spotLight.set(false);
	spotLight.addListener(this, &ofApp::spotLightChanged);

	translateXLight.setName("X");
	translateXLight.setMin(MinX);
	translateXLight.setMax(MaxX);
	translateXLight.set((MinX + MaxX) / 2);
	translateXLight.addListener(this, &ofApp::translateChanged);

	translateYLight.setName("Y");
	translateYLight.setMin(MinY);
	translateYLight.setMax(MaxY);
	translateYLight.set((MinY + MaxY) / 2);
	translateYLight.addListener(this, &ofApp::translateChanged);

	translateZLight.setName("Z");
	translateZLight.setMin(MinZ);
	translateZLight.setMax(MaxZ);
	translateZLight.set((MinZ + MaxZ) / 2);
	translateZLight.addListener(this, &ofApp::translateChanged);

	rotateXLight.setName("X");
	rotateXLight.setMin(0);
	rotateXLight.setMax(360);
	rotateXLight.set(0);
	rotateXLight.addListener(this, &ofApp::rotateChanged);

	rotateYLight.setName("Y");
	rotateYLight.setMin(0);
	rotateYLight.setMax(360);
	rotateYLight.set(0);
	rotateYLight.addListener(this, &ofApp::rotateChanged);

	rotateZLight.setName("Z");
	rotateZLight.setMin(0);
	rotateZLight.setMax(360);
	rotateZLight.set(0);
	rotateZLight.addListener(this, &ofApp::rotateChanged);

	specLightHue.setName("Teinte");
	specLightHue.setMin(0);
	specLightHue.setMax(255);
	specLightHue.set(0);

	specLightSaturation.setName("Saturation");
	specLightSaturation.setMin(0);
	specLightSaturation.setMax(255);
	specLightSaturation.set(100);

	specLightBrightess.setName("Valeur");
	specLightBrightess.setMin(0);
	specLightBrightess.setMax(255);
	specLightBrightess.set(255);

	diffLightHue.setName("Teinte");
	diffLightHue.setMin(0);
	diffLightHue.setMax(255);
	diffLightHue.set(0);

	diffLightSaturation.setName("Saturation");
	diffLightSaturation.setMin(0);
	diffLightSaturation.setMax(255);
	diffLightSaturation.set(100);

	diffLightBrightess.setName("Valeur");
	diffLightBrightess.setMin(0);
	diffLightBrightess.setMax(255);
	diffLightBrightess.set(255);
}

void ofApp::setColors()
{
	ambient = ofColor::fromHsb(ambientHue, ambientSaturation, ambientBrightess);
	diff = ofColor::fromHsb(diffHue, diffSaturation, diffBrightess);
	emis = ofColor::fromHsb(emisHue, emisSaturation, emisBrightess);
	spec = ofColor::fromHsb(specHue, specSaturation, specBrightess);
	background = ofColor::fromHsb(bgHue, bgSaturation, bgBrightess);
}

void ofApp::setRendererParameter() {

	rend->activeMaterial.setAmbientColor(ambient);
	rend->activeMaterial.setDiffuseColor(diff);
	rend->activeMaterial.setEmissiveColor(emis);
	rend->activeMaterial.setSpecularColor(spec);
	
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
	else {
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

}

void ofApp::btnAddLightClicked() {
		diffLight = ofColor::fromHsb(diffLightHue, diffLightSaturation, diffLightBrightess);
		specLight = ofColor::fromHsb(specLightHue, specLightSaturation, specLightBrightess);
	if (directionalLight.get()) {
		rend->createDirectionalLight(rotateXLight, rotateYLight, rotateZLight, diffLight, specLight);
	}
	else if (ponctualLight.get()) {
		rend->createPonctualLight(translateXLight, translateYLight, translateZLight, diffLight, specLight);
	}
	else if (spotLight.get()) {
		rend->createSpotlight(ofVec3f(translateXLight, translateYLight, translateZLight), rotateXLight, rotateYLight, rotateZLight, diffLight, specLight);
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
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
		primTypeTriangle.setName("Triangle");
		primTypeLine.setName("Ligne");
	}
	else {
		primType3D.set(true);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
		primTypeTriangle.setName("Cone");
		primTypeLine.setName("IceCream");
	}

}

void ofApp::primDim3DChanged(bool& value) {
	if (primType3D.get()) {
		primType2D.set(false);
		primTypeCube.setName("Cube");
		primTypeSphere.setName("Sphere");
		primTypeTriangle.setName("Cone");
		primTypeLine.setName("IceCream");
	}
	else {
		primType2D.set(true);
		primTypeCube.setName("Carre");
		primTypeSphere.setName("Cercle");
		primTypeTriangle.setName("Triangle");
		primTypeLine.setName("Ligne");
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

void ofApp::directionalLightChanged(bool& value) {

	directionalLight.disableEvents();
	ponctualLight.disableEvents();
	spotLight.disableEvents();

	if (directionalLight.get()) {
		ponctualLight.set(false);
		spotLight.set(false);
	}
	else {
		directionalLight.set(true);
	}

	directionalLight.enableEvents();
	ponctualLight.enableEvents();
	spotLight.enableEvents();
}

void ofApp::ponctualLightChanged(bool& value) {

	directionalLight.disableEvents();
	ponctualLight.disableEvents();
	spotLight.disableEvents();

	if (ponctualLight.get()) {
		directionalLight.set(false);
		spotLight.set(false);
	}
	else {
		ponctualLight.set(true);
	}

	directionalLight.enableEvents();
	ponctualLight.enableEvents();
	spotLight.enableEvents();
}

void ofApp::spotLightChanged(bool& value) {

	directionalLight.disableEvents();
	ponctualLight.disableEvents();
	spotLight.disableEvents();

	if (spotLight.get()) {
		ponctualLight.set(false);
		directionalLight.set(false);
	}
	else {
		spotLight.set(true);
	}

	directionalLight.enableEvents();
	ponctualLight.enableEvents();
	spotLight.enableEvents();
}

void ofApp::wireFrameChanged(bool& value) {

	ofLog() << "<app::wireFrameModeChanged>";
	rend->setWireFrameMode(value);
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

	menu2D.add(groupCouleur);

	menu2D.add(groupLight);

	menu2D.minimizeAll();

	menu2D.registerMouseEvents();
}

void ofApp::setupMenu3D() {

	menu3D.setDefaultWidth(200);

	menu3D.setup();

	menu3D.add(groupPrimitiveType3D);
	menu3D.add(groupPrimitivePosition3D);
	menu3D.add(groupPrimitiveSize3D);

	menu3D.add(groupCouleur);

	menu3D.add(groupLight);

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
	optionMenu.add(btnAddLight.setup("Ajouter une lumiere"));
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