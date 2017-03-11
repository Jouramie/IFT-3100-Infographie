#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "renderer.h"
#include "ccamera.h"
#include "ofxRay.h"
#include "scene.h"

class ofApp : public ofBaseApp {

public:

	const float MaxX = 1000, MaxY = 1000, MaxZ = 1000;
	const float MinX = -1000, MinY = -1000, MinZ = -1000;

	renderer * rend;
	scene * scn;

	ofApp();

	void setup();
	void update();
	void draw();

	~ofApp();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseMovedGui(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool cursorIsInControl(int x, int y);


private:
	
	ccamera *cam;

	bool isListenersUnlocked;

	ofxPanel gui;
	ofxPanel cameraMenu;
	ofxPanel transformationMenu;
	ofxPanel filterMenu;
	ofxButton btnSelect;
	ofxButton btnDrawPrimitive;
	ofxButton btnExit;

	ofxButton btnExport;
	ofxButton btnImport;

	ofParameterGroup groupProperty;
	ofParameterGroup groupThick;
	ofParameter<float> strokeThickness;

	ofParameterGroup groupStroke;
	ofColor stroke;
	ofParameter<float> strokeHue;
	ofParameter<float> strokeSaturation;
	ofParameter<float> strokeBrightess;
	ofParameter<float> strokeAlpha;

	ofParameterGroup groupFill;
	ofColor fill;
	ofParameter<float> fillHue;
	ofParameter<float> fillSaturation;
	ofParameter<float> fillBrightess;
	ofParameter<float> fillAlpha;

	ofParameterGroup groupBackground;
	ofColor background;
	ofParameter<float> bgHue;
	ofParameter<float> bgSaturation;
	ofParameter<float> bgBrightess;

#pragma region "Primitives"

	ofParameterGroup groupPrimitiveType2D;
	ofParameterGroup groupPrimitiveType3D;
	ofParameter<bool> primType2D;
	ofParameter<bool> primType3D;
	ofParameter<bool> primTypeSphere;
	ofParameter<bool> primTypeCube;
	ofParameter<bool> primTypeTriangle;
	ofParameter<bool> primTypeLine;
	ofParameter<bool> primTypePoint;

	ofParameterGroup groupPrimitivePosition3D;
	ofParameterGroup groupPrimitivePosition2D;
	ofParameter<float> primPosX;
	ofParameter<float> primPosY;
	ofParameter<float> primPosZ;

	ofParameterGroup groupPrimitiveSize2D;
	ofParameterGroup groupPrimitiveSize3D;
	ofParameter<float> primSizeHeight;
	ofParameter<float> primSizeWidth;
	ofParameter<float> primSizeDepth;

#pragma endregion
	ofParameterGroup groupTexture;
	ofParameter<bool> noTexture;
	ofParameter<bool> metalTexture;
	ofParameter<bool> waterTexture;

	ofParameterGroup groupTranslate2D;
	ofParameterGroup groupTranslate3D;
	ofParameter<float> translateX;
	ofParameter<float> translateY;
	ofParameter<float> translateZ;

	ofParameterGroup groupRotate2D;
	ofParameterGroup groupRotate3D;
	ofParameter<float> rotateX;
	ofParameter<float> rotateY;
	ofParameter<float> rotateZ;

	ofParameterGroup groupProportion2D;
	ofParameterGroup groupProportion3D;
	ofParameter<float> proportionX;
	ofParameter<float> proportionY;
	ofParameter<float> proportionZ;

	ofParameterGroup groupFilter;
	ofParameter<bool> blur;
	ofParameter<bool> invert;
	ofParameter<bool> dilate;


	void initGroups();
	void initButtonListener();
	void initOfParameters();

	void setColors();

	void setRendererParameter();
		
	void setupGui(); 
	void setupCameraMenu();
	void setupTransformationMenu();
	void setupFilterMenu();

	void btnSelectClicked();
	void btnDrawPrimitiveClicked();
	void btnExitClicked();
	void btnExportClicked();
	void btnImportClicked();
	
	void primDim2DChanged(bool& value);
	void primDim3DChanged(bool& value);
	void primTypeCubeChanged(bool& value);
	void primTypeSphereChanged(bool& value);
	void primTypeTriangleChanged(bool & value);
	void primTypeLineChanged(bool & value);
	void primTypePointChanged(bool & value);

	void noTextureChanged(bool & value);
	void metalTextureChanged(bool & value);
	void waterTextureChanged(bool & value);

	void blurChanged(bool & value);
	void invertChanged(bool & value);
	void dilateChanged(bool & value);

	void updateKeys();

	bool isKeyPressUp;
	bool isKeyPressDown;
	bool isKeyPressLeft;
	bool isKeyPressRight;
	bool isKeyPressPageUp;
	bool isKeyPressPageDown;

};
