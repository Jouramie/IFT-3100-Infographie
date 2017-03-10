#include "ofApp.h"
#include "camera.h"

ofApp::ofApp()
{
	rend = nullptr;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Visualiseur interactif de sc�nes 3D");

	initColors();
	initPrimitives();
	initGroups();

	setupCameraMenu();

	gui.setDefaultWidth(270);
	gui.setup();

	isListenersUnlocked = true;

	gui.registerMouseEvents();

	btnSelect.addListener(this, &ofApp::btnSelectClicked);
	btnDrawPrimitive.addListener(this, &ofApp::btnDrawPrimitiveClicked);
	btnExit.addListener(this, &ofApp::btnExitClicked);

	setupGui();

	isKeyPressDown = false;
	isKeyPressUp = false;
	isKeyPressPageDown = false;
	isKeyPressPageUp = false;
	isKeyPressLeft = false;
	isKeyPressRight = false;

	rend = new renderer();
	rend->setup();

	ofLog() << "<app::setup>";
}

//--------------------------------------------------------------
void ofApp::update()
{
	setColors();
	rend->background = background;
	rend->update();

	updateKeys();
}

void ofApp::updateKeys()
{
	rend->isCameraMoveBackward = isKeyPressDown;
	rend->isCameraMoveForward = isKeyPressUp;
	rend->isCameraMoveDown = isKeyPressPageDown;
	rend->isCameraMoveUp = isKeyPressPageUp;
	rend->isCameraMoveLeft = isKeyPressLeft;
	rend->isCameraMoveRight = isKeyPressRight;
}


//--------------------------------------------------------------
void ofApp::draw()
{
	rend->draw();
	gui.draw();
	cameraMenu.draw();

}

void ofApp::exit()
{
	ofLog() << "<app::exit>";
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
		//ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		//rend->createCube(rand() % 1004 + 20, rand() % 748 + 20, rand() % 100, 100, c);
	}
	else if (key == 's')
	{
		/*ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		rend->createSphere(rand() % 1004 + 20, rand() % 748 + 20, rand() % 100, 100, c);*/
	}
	else if (key == 'c') {
		rend->clearPrimitives();
	}
	else if (key == 'w') {
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
	if (cursorIsInControl(x, y))
	{
		curs = LoadCursor(NULL, IDC_HAND);
	}
	else
	{
		curs = LoadCursor(NULL, IDC_ARROW);
	}
	SetCursor(curs);
}

bool ofApp::cursorIsInControl(int x, int y) {
	vector<string> names = gui.getControlNames();
	for each (string name in names)
	{
		ofxBaseGui* control = gui.getControl(name);
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

void ofApp::setupGui() {

	isListenersUnlocked = false;

	gui.clear();
	
	gui.add(groupPrimitiveType);

	if (primType2D)
	{
		gui.add(groupPrimitivePosition2D);

		if (primTypeCube) {
			gui.add(groupPrimitiveSizeCube2D);
		}
		else {
			gui.add(groupPrimitiveSizeSphere);
		}
	}
	else
	{
		gui.add(groupPrimitivePosition3D);

		if (primTypeCube) {
			gui.add(groupPrimitiveSizeCube3D);
		}
		else {
			gui.add(groupPrimitiveSizeSphere);
		}
	}


	ofColor defaultColor = gui.getFillColor();

	if (primType2D)
	{
		gui.add(groupThick);
	}

	gui.setDefaultFillColor(fill);
	gui.add(groupFill);

	if (primType2D)
	{
		gui.setDefaultFillColor(stroke);
		gui.add(groupStroke);
	}

	gui.add(btnDrawPrimitive.setup("Dessiner!"));

	gui.setDefaultFillColor(background);
	gui.add(groupBackground);

	gui.setDefaultFillColor(defaultColor);

	gui.add(btnSelect.setup("Outils de selection"));

	gui.add(btnExit.setup("Quitter"));

	isListenersUnlocked = true;
}

void ofApp::btnSelectClicked()
{
	if (isListenersUnlocked)
	{

		setupGui();
	}
}

void ofApp::btnDrawPrimitiveClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btnDrawPrimitiveClicked>";

		if (primType2D.get()) {
			if (primTypeCube.get()){
				rend->createSquare(primPosX, primPosY, primSizeWidth, primSizeHeight, fillCol);
			}
			else {
				if (primTypeSphere.get()) {
					rend->createCircle(primPosX, primPosY, primSizeWidth, primSizeHeight, fillCol);
				}
				else {
					rend->createLine(primPosX, primPosY, primSizeWidth, primSizeHeight, fillCol);
					rend->createTriangle(primPosX, primPosY, primPosX + primSizeWidth, primPosY, (primPosX + primSizeWidth)/2, primPosY + primSizeHeight, fillCol);
				}
				
			}
		}
		else {
			if (primTypeCube.get()) {
				rend->createCube(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth, fill);
			}
			else {
				rend->createSphere(primPosX, primPosY, primPosZ, primSizeWidth, primSizeHeight, primSizeDepth, fillCol);
			}
		}

	}
}

void ofApp::btnExitClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btnExitClicked>";
		ofExit();		//TODO: Trouver une autre solution
	}
}

void ofApp::initGroups()
{

	groupPrimitiveType.setName("Type");
	groupPrimitiveType.add(primType2D);
	groupPrimitiveType.add(primType3D);
	groupPrimitiveType.add(primTypeSphere);
	groupPrimitiveType.add(primTypeCube);

	groupPrimitivePosition2D.setName("Position");
	groupPrimitivePosition2D.add(primPosX.set(primPosX));
	groupPrimitivePosition2D.add(primPosY.set(primPosY));

	groupPrimitivePosition3D.setName("Position");
	groupPrimitivePosition3D.add(primPosX.set(primPosX));
	groupPrimitivePosition3D.add(primPosY.set(primPosY));
	groupPrimitivePosition3D.add(primPosZ.set(primPosZ));

	groupPrimitiveSizeCube2D.setName("Taille");
	groupPrimitiveSizeCube2D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSizeCube2D.add(primSizeHeight.set(primSizeHeight));

	groupPrimitiveSizeCube3D.setName("Taille");
	groupPrimitiveSizeCube3D.add(primSizeWidth.set(primSizeWidth));
	groupPrimitiveSizeCube3D.add(primSizeHeight.set(primSizeHeight));
	groupPrimitiveSizeCube3D.add(primSizeDepth.set(primSizeDepth));

	groupPrimitiveSizeSphere.setName("Taille");
	groupPrimitiveSizeSphere.add(primSizeRadius);
	
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



}

void ofApp::initColors()
{
	strokeThickness.setName("Epaisseur");
	strokeThickness.setMin(0);
	strokeThickness.setMax(100);
	strokeThickness.set(10);

	fillHue.setName("Teinte");
	fillHue.setMin(0);
	fillHue.setMax(255);
	fillHue.set(0);
	//fillHue.addListener(this, &ofApp::colorParameterChanged);

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
}

void ofApp::setColors()
{
	stroke = ofColor::fromHsb(strokeHue, strokeSaturation, strokeBrightess, strokeAlpha);
	fill = ofColor::fromHsb(fillHue, fillSaturation, fillBrightess, fillAlpha);
	background = ofColor::fromHsb(bgHue, bgSaturation, bgBrightess);
}

float ofApp::colorParameterChanged(float& value) {
	setupGui();
	return 1;
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
	setupGui();
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
	setupGui();
}

void ofApp::primTypeCubeChanged(bool& value) {
	if (primTypeCube.get())
		primTypeSphere.set(false);
	else
		primTypeSphere.set(true);
	setupGui();
}

void ofApp::primTypeSphereChanged(bool& value) {
	if (primTypeSphere.get())
		primTypeCube.set(false);
	else
		primTypeCube.set(true);
	setupGui();
}

void ofApp::initPrimitives() {

	primType2D.setName("2D");
	primType2D.set(true);
	primType2D.addListener(this, &ofApp::primDim2DChanged);

	primType3D.setName("3D");
	primType3D.set(false);
	primType3D.addListener(this, &ofApp::primDim3DChanged);

	primTypeCube.setName("Carr�");
	primTypeCube.set(true);
	primTypeCube.addListener(this, &ofApp::primTypeCubeChanged);

	primTypeSphere.setName("Cercle");
	primTypeSphere.set(false);
	primTypeSphere.addListener(this, &ofApp::primTypeSphereChanged);


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

	primSizeRadius.setName("Rayon");
	primSizeRadius.setMin(0);
	primSizeRadius.setMax((MaxX + MaxY + MaxZ) / 3);
	primSizeRadius.set((0 + (MaxX + MaxY + MaxZ) / 3) / 2);


}

void ofApp::setupCameraMenu() {

	cam = new camera();

	cameraMenu.setDefaultWidth(270);

	cameraMenu.setup();
	cameraMenu.add(cam->getParameterGroup());

	cameraMenu.setPosition(ofGetWindowWidth() - 280, 10);

}
