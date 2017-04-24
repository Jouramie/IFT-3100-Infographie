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
	ofxPanel menuTopo;
	ofxPanel menuLight;
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
	ofParameterGroup groupCouleurLight;
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
	ofParameterGroup groupPrimitiveTypeTopo;
	ofParameterGroup groupPrimitiveTypeLight;
	ofParameter<bool> primType2D;
	ofParameter<bool> primType3D;
	ofParameter<bool> primTypeTopo;
	ofParameter<bool> primTypeLight;
	ofParameter<bool> primTypeSphere;
	ofParameter<bool> primTypeCube;
	ofParameter<bool> primTypeCubeReflect;
	ofParameter<bool> primTypeCubeRefract;
	ofParameter<bool> primTypeTriangle;
	ofParameter<bool> primTypeLine;
	ofParameter<bool> primTypePoint;
	ofParameter<bool> primTypeBezier;
	ofParameter<bool> primTypeHermite;
	ofParameter<bool> primTypeCatmullRom;
	ofParameter<bool> primTypeSurface;

	ofParameterGroup groupPrimitivePosition3D;
	ofParameterGroup groupPrimitivePosition2D;
	ofParameterGroup groupPrimitivePositionTopo;
	ofParameter<float> primPosX;
	ofParameter<float> primPosY;
	ofParameter<float> primPosZ;
	ofParameter<float> primPosX2;
	ofParameter<float> primPosY2;
	ofParameter<float> primPosZ2;
	ofParameter<float> primPosX3;
	ofParameter<float> primPosY3;
	ofParameter<float> primPosZ3;
	ofParameter<float> primPosX4;
	ofParameter<float> primPosY4;
	ofParameter<float> primPosZ4;
	ofParameter<float> primPosX5;
	ofParameter<float> primPosY5;
	ofParameter<float> primPosZ5;

	ofParameterGroup groupPrimitiveSize2D;
	ofParameterGroup groupPrimitiveSize3D;
	ofParameterGroup groupPrimitiveSizeTopo;
	ofParameter<float> primSizeHeight;
	ofParameter<float> primSizeWidth;
	ofParameter<float> primSizeDepth;

#pragma endregion

	ofParameter<bool> ambientLight;
	ofParameter<bool> directionalLight;
	ofParameter<bool> ponctualLight;
	ofParameter<bool> spotLight;

	ofParameter<float> translateXLight;
	ofParameter<float> translateYLight;
	ofParameter<float> translateZLight;

	ofParameterGroup groupRotateLight;
	ofParameter<float> rotateXLight;
	ofParameter<float> rotateYLight;
	ofParameter<float> rotateZLight;
	

	ofParameterGroup groupTranslate2D;
	ofParameterGroup groupTranslate3D;
	ofParameterGroup groupTranslateLight;
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

	ofParameter<bool> vertex;
	ofParameter<bool> fragment;
	ofParameter<bool> phong;
	ofParameter<bool> blinnphong;


	void initGroups();
	void initButtonListener();
	void initOfParameters();

	void setColors();

	void setRendererParameter();
		
	void setupMenu2D();
	void setupMenu3D(); 
	void setupMenuTopo();
	void setupMenuLight();
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
	void primTopoChanged(bool& value);
	void lightChanged(bool & value);
	void primTypeCubeChanged(bool& value);
	void primTypeCubeReflectChanged(bool & value);
	void primTypeCubeRefractChanged(bool & value);
	void primTypeSphereChanged(bool& value);
	void primTypeTriangleChanged(bool & value);
	void primTypeLineChanged(bool & value);
	void primTypePointChanged(bool & value);
	void primTypeBezierChanged(bool & value);
	void primTypeHermiteChanged(bool& value);
	void primTypeCatmullRomChanged(bool& value); 
	void primTypeSurfaceChanged(bool& value);

	void ambientLightChanged(bool & value);
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

	void vertexChanged(bool & value);
	void fragmentChanged(bool & value);
	void phongChanged(bool & value);
	void blinnphongChanged(bool & value);

	void drawMenus();


	void updateKeys();

	bool isKeyPressUp;
	bool isKeyPressDown;
	bool isKeyPressLeft;
	bool isKeyPressRight;
	bool isKeyPressPageUp;
	bool isKeyPressPageDown;

};
