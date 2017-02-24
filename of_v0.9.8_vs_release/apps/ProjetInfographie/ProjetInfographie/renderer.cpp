#include "renderer.h"



renderer::renderer()
{
}


void renderer::setup()
{
	ofSetFrameRate(60);
	
}

void renderer::update()
{

}

void renderer::draw()
{
	ofClear(0, 0, 200);

	std::list<of3dPrimitive>::const_iterator iterator;
	for (iterator = primitives.begin(); iterator != primitives.end(); ++iterator)
	{
		ofSetLineWidth(1.0);
		if (wireFrame)
			iterator->drawWireframe();
		else
			iterator->draw();
	}
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

renderer::~renderer()
{
}
