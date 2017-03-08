#include "renderer.h"



renderer::renderer()
{
}


void renderer::setup()
{
	ofSetFrameRate(60);
	/*mainCam = ofEasyCam();
	rotate = -1;
	mainCam.begin();*/
}

void renderer::update()
{

}

void renderer::draw()
{
	ofClear(background);

	std::list<of3dPrimitive>::const_iterator iterator;
	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
	{
		ofSetLineWidth(1.0);
		if (wireFrame)
			iterator->drawWireframe();
		else
			iterator->draw();
	}
	/*if (rotate > -1)
	{
		rotate++;
		if (rotate > 359)
			rotate = 0;
		mainCam.rotate(rotate, 1, 0, 0);
	}
	else
		mainCam.reset();
	mainCam.draw();*/
}

void renderer::imageExport(const string name, const string extension) const
{
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

void renderer::createCube(int x, int y, int z, int size)
{
	ofColor c = ofColor(255, 255, 255);
	createCube(x, y, z, size, c);
}

void renderer::createCube(int x, int y, int z, int size, ofColor color)
{
	ofBoxPrimitive box = ofBoxPrimitive();
	box.set(size);
	box.setPosition(x, y, z);
	box.rotate(rand() % 360, 1.0, 0.0, 0.0);
	box.rotate(rand() % 360, 0, 1.0, 0.0);
	box.rotate(rand() % 360, 0, 0.0, 1.0);
	for (int i = 0; i < 6; i++)
	{
		box.setSideColor(i, color);
	}
	primitives.push_back(box);
	draw();
}

void renderer::createSphere(int x, int y, int z, int size)
{
	ofColor c = ofColor(255, 255, 255);
	createSphere(x, y, z, size, c);
}

void renderer::createSphere(int x, int y, int z, int size, ofColor color)
{
	ofSpherePrimitive ball = ofSpherePrimitive();
	ball.setRadius(size);
	ball.setPosition(x, y, z);
	ball.rotate(rand() % 360, 1.0, 0.0, 0.0);
	ball.rotate(rand() % 360, 0, 1.0, 0.0);
	ball.rotate(rand() % 360, 0, 0.0, 1.0);
	//ball.set
	primitives.push_back(ball);
	draw();
}

void renderer::clearPrimitives()
{
	primitives.clear();
}

void renderer::changeWireFrameMode()
{
	wireFrame = !wireFrame;
}

void renderer::selectPrimitive(int x, int y)
{
	/*ofxRayTriangleIntersection  rtIntersect;
	vector<FaceTri>             tris;
	vector<Ray>                 rays;

	FaceTri tri;
	tri.v0 = ofPoint(ofGetWidth() / 2, ofGetHeight() / 2 / 2, -1000);
	tri.v1 = ofPoint(ofGetWidth() / 2 - 200, ofGetHeight() / 2 + (ofGetHeight() / 2 / 2), -1000);
	tri.v2 = ofPoint(ofGetWidth() / 2 + 200, ofGetHeight() / 2 + (ofGetHeight() / 2 / 2), -1000);
	tris.push_back(tri);

	Ray ray;
	ray.rayOrig.set(0, 0, 0);
	ray.rayEnd.set(0, 0, -2000);
	rays.push_back(ray);*/

	std::cout << "Selected Primitive" << std::endl;
}

renderer::~renderer()
{
}
