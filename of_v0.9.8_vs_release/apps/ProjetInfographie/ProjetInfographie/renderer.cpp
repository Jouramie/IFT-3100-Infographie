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

}

void renderer::imageExport(const string name, const string extension) const
{
	ofImage imageTemp;

	// extraire des donn�es temporelles format�es
	string timestamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// g�n�rer un nom de fichier unique et ordonn�
	string fileName = name + timestamp + "." + extension;

	// capturer le contenu du framebuffer actif
	imageTemp.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	// sauvegarder le fichier image
	imageTemp.save(fileName);

	ofLog() << "<export image: " << fileName << ">";
}

renderer::~renderer()
{
}
