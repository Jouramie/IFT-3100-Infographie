#include "ofApp.h"

ofApp::ofApp()
{
	rend = nullptr;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowTitle("Visualiseur interactif de scènes 3D");

	initPosition();
	initDimension();
	initColors();
	initGroups();

	gui.setDefaultWidth(270);
	gui.setup();

	is2dDisplay = false;
	is3dDisplay = false;
	isPropertiesDisplay = false;
	isListenersUnlocked = true;

	btnSelect.addListener(this, &ofApp::btnSelectClicked);
	btn2D.addListener(this, &ofApp::btn2DClicked);
	btn3D.addListener(this, &ofApp::btn3DClicked);
	btnProperty.addListener(this, &ofApp::btnPropertyClicked);
	btnDraw.addListener(this, &ofApp::btnDrawClicked);
	btnExit.addListener(this, &ofApp::btnExitClicked);

	setupGui();


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
}

//--------------------------------------------------------------
void ofApp::draw()
{
	rend->draw();
	gui.draw();
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

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == ' ')
		rend->imageExport("render", "png");
	else if (key == 'r')
	{
		ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		rend->createCube(rand() % 1004 + 20, rand() % 748 + 20, rand() % 100, 100, c);
	}
	else if (key == 's')
	{
		ofColor c = ofColor(rand() % 256, rand() % 256, rand() % 256);
		rend->createSphere(rand() % 1004 + 20, rand() % 748 + 20, rand() % 100, 100, c);
	}
	else if (key == 'c')
		rend->clearPrimitives();
	else if (key == 'w')
		rend->changeWireFrameMode();
	/*else if (key == 'q')
		rend->changeRotate();*/

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

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
	rend->selectPrimitive(x, y);
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

}

void ofApp::setupGui() {
	
	isListenersUnlocked = false;

	gui.clear(); 

	gui.add(ofParameter<string>("Boite a outil"));
	gui.add(btnDraw.setup("Dessiner!"));
	gui.add(btnSelect.setup("Outils de selection"));
	gui.add(btn2D.setup("Outils de dessin 2D"));
	gui.add(btn3D.setup("Outils de dessin 3D"));
	gui.add(btnProperty.setup("Proprietes de l'outil de dessin"));
	
	if (is2dDisplay) {
		display2D();
	}

	if (is3dDisplay) {
		display3D();
	}

	if (isPropertiesDisplay) {
		displayProperties();
	}
	
	gui.add(btnExit.setup("Quitter"));

	isListenersUnlocked = true;
}

void ofApp::displayProperties() 
{
	ofColor defaultColor = gui.getFillColor();

	gui.add(groupThick);
	
	gui.setDefaultFillColor(fill);
	gui.add(groupFill);

	gui.setDefaultFillColor(stroke);
	gui.add(groupStroke);

	gui.setDefaultFillColor(background);
	gui.add(groupBackground);

	gui.setDefaultFillColor(defaultColor);
	
}

void ofApp::display2D()
{
	float centreX = (MaxX + MinX) / 2;
	float centreY = (MaxY + MinY) / 2;

	gui.add(ofParameter<string>("--------------------------------"));
	gui.add(ofParameter<string>("Position"));
	gui.add(posX.set(posX));
	gui.add(posY.set(posY));
	gui.add(ofParameter<string>("--------------------------------"));
	gui.add(ofParameter<string>("Dimension"));
	gui.add(width.set(width));
	gui.add(height.set(height));
}

void ofApp::display3D()
{
	gui.add(groupPosition);

	gui.add(groupDimension);
}

void ofApp::btnSelectClicked()
{
	if(isListenersUnlocked)
	{
		isPropertiesDisplay = false;
		is2dDisplay = false;
		is3dDisplay = false;

		setupGui();
	}
}

void ofApp::btnPropertyClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btnPropertyClicked>";

		isPropertiesDisplay = !isPropertiesDisplay;
		setupGui();
	}
}

void ofApp::btn2DClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btn2DClicked>";

		is2dDisplay = !is2dDisplay;

		is3dDisplay = false;

		setupGui();
	}
}

void ofApp::btn3DClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btn3DClicked>";

		is3dDisplay = !is3dDisplay;

		is2dDisplay = false;

		setupGui();
	}
}

void ofApp::btnDrawClicked()
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btnDrawClicked>";

		//TODO: Faites vous du fun!

		rend->createCube(posX, posY, posZ, height, stroke);
	}
}

void ofApp::btnExitClicked() 
{
	if (isListenersUnlocked)
	{
		ofLog() << "<app::btnExitClicked>";
		exit();		//TODO: Trouver une autre solution
	}
}

void ofApp::initGroups()
{

	groupPosition.add(ofParameter<string>("Position"));
	groupPosition.add(posX.set(posX));
	groupPosition.add(posY.set(posY));
	groupPosition.add(posZ.set(posZ));

	groupDimension.add(ofParameter<string>("Dimension"));
	groupDimension.add(width.set(width));
	groupDimension.add(height.set(height));
	groupDimension.add(depth.set(depth));

	groupThick.add(ofParameter<string>("Epaisseur des traits"));
	groupThick.add(strokeThickness.set(strokeThickness));

	groupFill.add(ofParameter<string>("Couleur de remplissage"));
	groupFill.add(fillHue.set(fillHue));
	groupFill.add(fillSaturation.set(fillSaturation));
	groupFill.add(fillBrightess.set(fillBrightess));
	groupFill.add(fillAlpha.set(fillAlpha));
		
	groupStroke.add(ofParameter<string>("Couleur de bordure"));
	groupStroke.add(strokeHue.set(strokeHue));
	groupStroke.add(strokeSaturation.set(strokeSaturation));
	groupStroke.add(strokeBrightess.set(strokeBrightess));
	groupStroke.add(strokeAlpha.set(strokeAlpha));

	groupBackground.add(ofParameter<string>("Couleur de fond"));
	groupBackground.add(bgHue.set(bgHue));
	groupBackground.add(bgSaturation.set(bgSaturation));
	groupBackground.add(bgBrightess.set(bgBrightess));
 
}

void ofApp::initPosition() {

	posX.setName("X");
	posX.setMin(MinX);
	posX.setMax(MaxX);
	posX.set((MinX + MaxX) / 2);

	posY.setName("Y");
	posY.setMin(MinY);
	posY.setMax(MaxY);
	posY.set((MinY + MaxY) / 2);

	posZ.setName("Z");
	posZ.setMin(MinZ);
	posZ.setMax(MaxZ);
	posZ.set((MinZ + MaxZ) / 2);
}

void ofApp::initDimension() 
{
	height.setName("Hauteur");
	height.setMin(MinY);
	height.setMax(MaxY);
	height.set((MinY + MaxY) / 2);

	width.setName("Largeur");
	width.setMin(MinX);
	width.setMax(MaxX);
	width.set((MinX + MaxX) / 2);

	depth.setName("Profondeur");
	depth.setMin(MinZ);
	depth.setMax(MaxZ);
	depth.set((MinZ + MaxZ) / 2);

	radius.setName("Rayon");
	radius.setMin(MinY);
	radius.setMax(MaxY);
	radius.set((MinY + MaxY) / 2);


	strokeThickness.setName("Epaisseur");
	strokeThickness.setMin(0);
	strokeThickness.setMax(100);
	strokeThickness.set(10);
}

void ofApp::initColors()
{
	
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

}

void ofApp::setColors()
{
	stroke = ofColor::fromHsb(strokeHue, strokeSaturation, strokeBrightess, strokeAlpha);
	fill = ofColor::fromHsb(fillHue, fillSaturation, fillBrightess, fillAlpha);
	background = ofColor::fromHsb(bgHue, bgSaturation, bgBrightess);
}
