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
	
	renderer * rend;
	scene * scn;
	ccamera *cam;
	
	ofxPanel menu2D;
	ofxPanel menu3D;
	ofxPanel cameraMenu;
	ofxPanel transformationMenu;
	ofxPanel filterMenu;
	ofxPanel optionMenu;
	ofxPanel selectionMenu;

	ofxButton btnDrawPrimitive;
	ofxButton btnAddLight;
	ofxButton btnClear;
	ofxButton btnExit;

	ofxButton btnExport;
	ofxButton btnImport;
	
	ofxButton btnApplySelect;
	ofParameter<bool> applyAll;

	ofParameterGroup groupProperty;
	ofParameterGroup groupThick;
	ofParameter<float> strokeThickness;

	ofParameterGroup groupCouleur;
	ofParameterGroup groupAmbient;
	ofColor ambient;
	ofParameter<float> ambientHue;
	ofParameter<float> ambientSaturation;
	ofParameter<float> ambientBrightess;

	ofParameterGroup groupDiff;
	ofColor diff;
	ofParameter<float> diffHue;
	ofParameter<float> diffSaturation;
	ofParameter<float> diffBrightess;

	ofParameterGroup groupEmis;
	ofColor emis;
	ofParameter<float> emisHue;
	ofParameter<float> emisSaturation;
	ofParameter<float> emisBrightess;

	ofParameterGroup groupSpec;
	ofColor spec;
	ofParameter<float> specHue;
	ofParameter<float> specSaturation;
	ofParameter<float> specBrightess;

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

	ofParameterGroup groupLight;
	ofParameter<bool> directionalLight;
	ofParameter<bool> ponctualLight;
	ofParameter<bool> spotLight;

	ofParameterGroup groupTranslateLight;
	ofParameter<float> translateXLight;
	ofParameter<float> translateYLight;
	ofParameter<float> translateZLight;

	ofParameterGroup groupRotateLight;
	ofParameter<float> rotateXLight;
	ofParameter<float> rotateYLight;
	ofParameter<float> rotateZLight;

	ofParameterGroup groupDiffLight;
	ofColor diffLight;
	ofParameter<float> diffLightHue;
	ofParameter<float> diffLightSaturation;
	ofParameter<float> diffLightBrightess;

	ofParameterGroup groupSpecLight;
	ofColor specLight;
	ofParameter<float> specLightHue;
	ofParameter<float> specLightSaturation;
	ofParameter<float> specLightBrightess;


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

	ofParameterGroup groupWireFrame;
	ofParameter<bool> wireFrame;

	void initGroups();
	void initButtonListener();
	void initOfParameters();

	void setColors();

	void setRendererParameter();
		
	void setupMenu2D();
	void setupMenu3D(); 
	void setupCameraMenu();
	void setupTransformationMenu();
	void setupFilterMenu();
	void setupSelectionMenu();
	void setupOptionMenu();

	void updatePositionMenu();

	void btnDrawPrimitiveClicked();
	void btnAddLightClicked();
	void btnClearClicked();
	void btnExitClicked();
	void btnExportClicked();
	void btnApplySelectClicked();
	void applyAllChanged(bool& value);
	void btnImportClicked();
	
	void primDim2DChanged(bool& value);
	void primDim3DChanged(bool& value);
	void primTypeCubeChanged(bool& value);
	void primTypeSphereChanged(bool& value);
	void primTypeTriangleChanged(bool & value);
	void primTypeLineChanged(bool & value);
	void primTypePointChanged(bool & value);

	void directionalLightChanged(bool & value);

	void ponctualLightChanged(bool & value);

	void spotLightChanged(bool & value);

	void wireFrameChanged(bool& value);

	void translateChanged(float & value);
	void rotateChanged(float & value);
	void scaleChanged(float & value);

	void blurChanged(bool & value);
	void invertChanged(bool & value);
	void dilateChanged(bool & value);

	void drawMenus();


	void updateKeys();

	bool isKeyPressUp;
	bool isKeyPressDown;
	bool isKeyPressLeft;
	bool isKeyPressRight;
	bool isKeyPressPageUp;
	bool isKeyPressPageDown;

};
