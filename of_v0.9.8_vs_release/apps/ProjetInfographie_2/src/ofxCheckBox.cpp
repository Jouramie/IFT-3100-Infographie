#include "ofxCheckBox.h"
using namespace std;

ofxCheckBox::ofxCheckBox() {
	value.setSerializable(false);
}

ofxCheckBox::~ofxCheckBox() {
	//
}

ofxCheckBox* ofxCheckBox::setup(const std::string& toggleName, float width, float height) {
	setName(toggleName);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this, &ofxCheckBox::valueChanged);

	return this;
}

bool ofxCheckBox::mouseReleased(ofMouseEventArgs & args) {
	bool attended = setValue(args.x, args.y, false);
	if (attended) {
		return true;
	}
	else {
		return false;
	}
}

bool ofxCheckBox::mouseMoved(ofMouseEventArgs & args) {
	return ofxToggle::mouseMoved(args);
}

bool ofxCheckBox::mousePressed(ofMouseEventArgs & args) {
	return ofxToggle::mousePressed(args);
}

bool ofxCheckBox::mouseDragged(ofMouseEventArgs & args) {
	return ofxToggle::mouseDragged(args);
}

void ofxCheckBox::valueChanged(bool & v) {
	if (!v) {
		ofNotifyEvent(triggerEvent, this);
	}
}
