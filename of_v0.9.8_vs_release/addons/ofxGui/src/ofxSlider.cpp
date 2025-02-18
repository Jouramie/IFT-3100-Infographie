#include "ofxSlider.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxSlider<Type>::ofxSlider(){
	bUpdateOnReleaseOnly = false;
	bGuiActive = false;
	mouseInside = false;
}

template<typename Type>
ofxSlider<Type>::~ofxSlider(){
	value.removeListener(this,&ofxSlider::valueChanged);
}

template<typename Type>
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(ofParameter<Type> _val, float width, float height){
	bUpdateOnReleaseOnly = false;
	value.makeReferenceTo(_val);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	value.addListener(this,&ofxSlider::valueChanged);
	registerMouseEvents();
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width, float height){
	value.set(sliderName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
void ofxSlider<Type>::setMin(Type min){
    value.setMin(min);
}

template<typename Type>
Type ofxSlider<Type>::getMin(){
    return value.getMin();
}

template<typename Type>
void ofxSlider<Type>::setMax(Type max){
    value.setMax(max);
}

template<typename Type>
Type ofxSlider<Type>::getMax(){
    return value.getMax();
}

template<typename Type>
bool ofxSlider<Type>::mouseMoved(ofMouseEventArgs & args){
	mouseInside = isGuiDrawing() && b.inside(ofPoint(args.x,args.y));

	HCURSOR curs;
	if (mouseInside)
	{
		curs = LoadCursor(NULL, IDC_SIZEWE);
		SetCursor(curs);
	}
	/*else
	{
		curs = LoadCursor(NULL, IDC_ARROW);
	}*/

	return mouseInside;
}

template<typename Type>
bool ofxSlider<Type>::mousePressed(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.disableEvents();
	}
	if(setValue(args.x, args.y, true)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseDragged(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, false)){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseReleased(ofMouseEventArgs & args){
	if(bUpdateOnReleaseOnly){
		value.enableEvents();
	}
	bool attended = setValue(args.x, args.y, false);

	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

template<typename Type>
typename std::enable_if<std::is_integral<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename Type>
bool ofxSlider<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside){
		if(args.scrollY>0 || args.scrollY<0){
			double range = getRange(value.getMin(),value.getMax(),b.width);
			Type newValue = value + ofMap(args.scrollY,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<typename Type>
double ofxSlider<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxSlider<Type>::operator const Type & (){
	return value;
}

template<typename Type>
void ofxSlider<Type>::generateDraw(){
	bg.clear();
	bar.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	float valAsPct = ofMap( value, value.getMin(), value.getMax(), 0, b.width-2, true );
	bar.setFillColor(thisFillColor);
	bar.setFilled(true);
	bar.rectangle(b.x+1, b.y+1, valAsPct, b.height-2);

	generateText();
}


template<typename Type>
void ofxSlider<Type>::generateText(){
	string valStr = ofToString(value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<>
void ofxSlider<unsigned char>::generateText(){
	string valStr = ofToString((int)value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<typename Type>
void ofxSlider<Type>::render(){
	ofColor c = ofGetStyle().color;

	bg.draw();
	bar.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}


template<typename Type>
bool ofxSlider<Type>::setValue(float mx, float my, bool bCheck){
	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;
		}
	}
	if( bGuiActive ){
		value = ofMap(mx, b.x, b.x + b.width, value.getMin(), value.getMax(), true);
		return true;
	}
	return false;
}

template<typename Type>
ofAbstractParameter & ofxSlider<Type>::getParameter(){
	return value;
}


template<typename Type>
void ofxSlider<Type>::setUpdateOnReleaseOnly(bool _bUpdateOnReleaseOnly){
	bUpdateOnReleaseOnly = _bUpdateOnReleaseOnly;
}

template<typename Type>
void ofxSlider<Type>::valueChanged(Type & value){
    setNeedsRedraw();
}

template class ofxSlider<int8_t>;
template class ofxSlider<uint8_t>;
template class ofxSlider<int16_t>;
template class ofxSlider<uint16_t>;
template class ofxSlider<int32_t>;
template class ofxSlider<uint32_t>;
template class ofxSlider<int64_t>;
template class ofxSlider<uint64_t>;
template class ofxSlider<float>;
template class ofxSlider<double>;
