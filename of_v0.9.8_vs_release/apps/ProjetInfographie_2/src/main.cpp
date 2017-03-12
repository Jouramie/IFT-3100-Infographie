#include "ofMain.h"
#include "ofApp.h"
#include "scene.h"

#define test 0

//========================================================================
int main()
{
	if (test == 1) {
		test_scene();
	} else {
		ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context

														// this kicks off the running of my app
														// can be OF_WINDOW or OF_FULLSCREEN
														// pass in width and height too:
		ofRunApp(new ofApp());
	}
}
