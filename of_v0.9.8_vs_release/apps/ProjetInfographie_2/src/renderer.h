#pragma once
#include "ofMain.h"
#include "ofxRay.h"
#include "ofxOpenCv.h"
#include "ofxBezierSurface.h"
#include "ofxCubeMap.h"
#include "primitive3d.h"
#include <limits>
#include "scene.h"
#include "primitive2d.h"
#include "primitiveTopo.h"
#include "ccamera.h"
#include "extModel.h"
#include "light.h"
#include <typeinfo>
#include "ofxShadersFX.h"

// fonction d'évaluation d'une courbe de hermite (4 points de contrôle)
inline void hermite(
	float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float p4x, float p4y, float p4z,
	float&  x, float& y, float&  z)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float tt = t * t;
	float ttt = tt * t;

	x = (2 * ttt - 3 * tt + 1) * p1x + (ttt - 2 * tt + t) * p2x + (ttt - tt) * p3x + (-2 * ttt + 3 * tt) * p4x;
	y = (2 * ttt - 3 * tt + 1) * p1y + (ttt - 2 * tt + t) * p2y + (ttt - tt) * p3y + (-2 * ttt + 3 * tt) * p4y;
	z = (2 * ttt - 3 * tt + 1) * p1z + (ttt - 2 * tt + t) * p2z + (ttt - tt) * p3z + (-2 * ttt + 3 * tt) * p4z;
}

class renderer
{
public:
	enum illuminationModel
	{
		PHONG, BLINN_PHONG
	};

	ofColor background;
	ofColor stroke;
	ofColor fill;
	ofMaterial activeMaterial;
	ofParameter<float> strokeThickness;

	void setup();
	void update();
	void drawGlass(char axis);
	void draw();
	void imageExport(const string name, const string extension);
	void checkFilters();
	void sceneTranslate(float x, float y, float z);
	void sceneRotate(float angle, float centerX, float centerY, float centerZ);
	void sceneScale(float scaleX, float scaleY, float scaleZ);
	void applySelection(ofMatrix4x4 matrix);

	//2D primitives
	ofParameter<bool> createSquare(float x, float y, float width, float height);
	ofParameter<bool> createSquare(float x, float y, float width, float height, ofMaterial mat);
	ofParameter<bool> createCircle(float x, float y, float r1, float r2);
	ofParameter<bool> createCircle(float x, float y, float r1, float r2, ofMaterial mat);
	ofParameter<bool> createLine(float x, float y, float xDelta, float yDelta);
	ofParameter<bool> createLine(float x, float y, float xDelta, float yDelta, ofMaterial mat);
	ofParameter<bool> createTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	ofParameter<bool> createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ofMaterial mat);
	ofParameter<bool> createPoint(float x, float y, float radius);
	ofParameter<bool> createPoint(float x, float y, float radius, ofMaterial mat);

	//Topologie
	ofParameter<bool> createBezier(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf);
	ofParameter<bool> createBezier(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, ofMaterial mat);
	ofParameter<bool> createHermite(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, int lineRes);
	ofParameter<bool> createHermite(float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float xi, float yi, float zi, float xf, float yf, float zf, int lineRes, ofMaterial mat);
	ofParameter<bool> createCatmullRom(const ofPoint cp1, const ofPoint cp2, const ofPoint to, const ofPoint cp4, int lineRes);
	ofParameter<bool> createCatmullRom(const ofPoint cp1, const ofPoint cp2, const ofPoint to, const ofPoint cp4, int lineRes, ofMaterial mat);
	ofParameter<bool> createSurface(int w, int h, int dim, int res, const ofPoint cp1, const ofPoint cp2, const ofPoint cp3, const ofPoint cp4);
	ofParameter<bool> createSurface(int w, int h, int dim, int res, const ofPoint cp1, const ofPoint cp2, const ofPoint cp3, const ofPoint cp4, ofMaterial mat);

	//3D primitives
	ofParameter<bool> createCube(int x, int y, int z, int w, int h, int d);
	ofParameter<bool> createCube(int x, int y, int z, int w, int h, int d, ofMaterial mat);
	ofParameter<bool> createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createSphere(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat);
	ofParameter<bool> createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createCone(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat);
	ofParameter<bool> createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ);
	ofParameter<bool> createIcecream(int x, int y, int z, int sizeX, int sizeY, int sizeZ, ofMaterial mat);

	ofParameter<bool> setAmbiantLight(ofColor col) { tempAmbientLight = col; };
	ofParameter<bool> createDirectionalLight(int ax, int ay, int az, ofColor difCol, ofColor specCol);
	ofParameter<bool> createPonctualLight(int x, int y, int z, ofColor difCol, ofColor specCol);
	ofParameter<bool> createSpotlight(ofVec3f pos, int ax, int ay, int az, ofColor difCol, ofColor specCol);

	ofParameter<bool> importModel(string path);

	void clearPrimitives();
	void changeWireFrameMode();
	void setWireFrameMode(bool wf);
	void selectPrimitive(int x, int y, bool shiftHeld);

	void setScene(scene * scn) { this->scn = scn; }
	void setCamera(ccamera * cam) { this->cam = cam; }

	//Translations
	float getDeltaX() { return deltaX; }
	float getDeltaY() { return deltaY; }
	float getDeltaZ() { return deltaZ; }

	//Rotation;
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	//Scale
	float getScaleX() { return scaleX; }
	float getScaleY() { return scaleY; }
	float getScaleZ() { return scaleZ; }

	void addBlur();
	void removeBlur();
	void addInvert();
	void removeInvert();
	void addDilate();
	void removeDilate();

	void setIlluminationModel(illuminationModel model);
	void setMustPrepares();

	void drawLines();
	void setIsShaderUsed(bool val) { isShaderUsed = val; };
	void addPoint(ofPoint pt) { points.push_back(pt); };
	void setupShader();

private:

	//ofEasyCam mainCam;
	bool wireFrame;

	//Filters
	bool isFiltered;
	bool blur;
	bool invert;
	bool dilate;

	//Transformations
	bool translate;
	bool rotate;
	bool scale;

	//Translations
	float deltaX, deltaY, deltaZ;
	//Rotation;
	float centerX, centerY, centerZ;
	//Scale
	float scaleX, scaleY, scaleZ;

	ofVec3f cameraPosition;
	ofVec3f cameraTarget;
	float time;
	float dt;
	float lastTime;

	scene * scn;
	ccamera * cam;

	std::list<ofRay> rays;

	ofxCvColorImage filter;
	ofImage sceneImg;
	ofPixels scenePixels;

	string hasRef;
	int refPosition;


	ofShader geometryShader;
	bool isShaderUsed;
	vector<ofPoint> points;

	bool isSkyboxUsed;
	ofxCubeMap cubeMap;
	
	ofColor tempAmbientLight;
	ofLight* tempDirectionalLight;

	ofxShadersFX::Lighting::LightingShader lightShader;
};

