#include "primitive3d.h"
#include <algorithm>

primitive3d::primitive3d() : primitive3d{ nullptr, ofColor(255, 255, 255), ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive) : primitive3d{ primitive, ofColor(255, 255, 255), ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive, ofColor fill) : primitive3d{ primitive, fill, ofMatrix4x4() }
{

}

primitive3d::primitive3d(of3dPrimitive* primitive, ofColor fill, ofMatrix4x4 matrix) : primitive{}, prim { primitive }, fillCol{ fill }
{
	transfoMatrix = matrix;
	mustPrepare = true;
}

of3dPrimitive* primitive3d::getPrimitive() {
	return prim;
}

ofColor primitive3d::getFillColor() {
	return fillCol;
}

void primitive3d::setMirror(bool mirror) {
	if (mirror)
	{
		isGlass = false;
	}
	isMirror = mirror;
}

void primitive3d::setGlass(bool glass) {
	if (glass)
	{
		isMirror = false;
	}
	isGlass = glass;
}

void primitive3d::draw(bool wireframe) {

	if ((!isGlass && !isMirror) || true)
	{
		/*ofPoint pointa = ofPoint(0, -200, 0);
		ofPoint pointb = ofPoint(100, 100, 0);
		ofPoint pointc = ofPoint(-100, 100, 0);

		ofTriangle(pointa, pointb, pointc);
		return;*/

		ofPushMatrix();
		ofTranslate(transfoMatrix.getTranslation());

		ofSetColor(fillCol);

		ofQuaternion rotation = transfoMatrix.getRotate();
		float rotationAmount;
		ofVec3f rotationAngle;
		rotation.getRotate(rotationAmount, rotationAngle);

		ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

		ofScale(transfoMatrix.getScale());


		if (wireframe || selected.get())
			prim->drawWireframe();
		else
		{
			/*for (auto face : prim->getMesh().getUniqueFaces())
			{
				ofSetColor(face.getColor(0));

				prim->getMesh().dr;
			}*/
			prim->drawFaces();
		}

		ofPopMatrix();
	}
}

ofColor getSlightlyLighterColor(ofColor original, float ratio = 1.1) {
	int red = original.r;
	int green = original.g;
	int blue = original.b;

	red = red * ratio;
	green = green * ratio;
	blue = blue * ratio;

	int maxColorValue = ofColor::limit();

	if (red > maxColorValue)
		red = maxColorValue;
	if (green > maxColorValue)
		green = maxColorValue;
	if (blue > maxColorValue)
		blue = maxColorValue;

	return ofColor(red, green, blue);
}

vector<ofRay> primitive3d::prepareGlass(const ofCamera cam, vector<primitive*> otherPrims, ofColor backgroundCol) {//, const scene * scn) {
	vector<ofRay> returnVec = vector<ofRay>();
	int wid = ofGetWidth();
	int hei = ofGetHeight();

	if (isMirror || isGlass)
	{	
		fillCol = ofColor(255, 255, 255, 0);
		if (mustPrepare) {
			ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();

			ofMesh * mesh = prim->getMeshPtr();

			vector<ofMeshFace> allFaces = mesh->getUniqueFaces();

			int firstX = wid;
			int lastX = 0;
			int firstY = hei;
			int lastY = 0;

			for (int i = 0; i < allFaces.size(); i++) {
				for (int j = 0; j < 3; j++)
				{
					ofVec3f screen = cam.worldToScreen(allFaces[i].getVertex(j) * toWorldSpace * transfoMatrix);
					if (firstX > screen.x)
						firstX = screen.x;
					if (firstY > screen.y)
						firstY = screen.y;
					if (lastX < screen.x)
						lastX = screen.x;
					if (lastY < screen.y)
						lastY = screen.y;
				}
			}

			if (firstX < 0)
				firstX = 0;
			if (firstY < 0)
				firstY = 0;
			if (lastX >= wid)
				lastX = wid - 1;
			if (lastY >= hei - 1)
				lastY = hei;

			toPaint = ofImage();
			toPaint.allocate(wid, hei, OF_IMAGE_COLOR_ALPHA);

			for (int i = 0; i < wid; i++)
			{
				for (int j = 0; j < hei; j++)
				{
					toPaint.getPixels().setColor(i, j, ofColor(255, 255, 255, 0));
				}
			}

			allFaces = mesh->getUniqueFaces();

			for (int x = firstX; x <= lastX; x++)
			{
				for (int y = firstY; y <= lastY; y++)
				{

					ofVec3f screenToWorld = cam.screenToWorld(ofVec3f(x, y, 0.0));
					ofRay ray(cam.getPosition(), screenToWorld - cam.getPosition());

					ofVec2f coords = ofVec2f(x, y);

					primitive* toSelect;
					float distance = -1;
					vector<hit> hits;
					vector<hit> realHits;

					for (auto& otherPrim3D : otherPrims)
					{
						hits = (otherPrim3D->intersectsMeshInstance(coords, cam));;
						if (hits.size() > 0 && (distance == -1 || (hits)[0].distance < distance))
						{
							distance = (hits)[0].distance;
							toSelect = otherPrim3D;
							realHits = hits;
						}
					}

					if (distance > -0.9 && toSelect->getName() == getName()) {
						const ofMeshFace * face = &allFaces[(realHits)[0].faceIndex];

						ofColor col = backgroundCol;

						ofRay mathReflectedRay = ofRay();

						if (isMirror) {
							ofVec3f A = face->getVertex(0) * toWorldSpace;
							ofVec3f B = face->getVertex(1) * toWorldSpace;
							ofVec3f C = face->getVertex(2) * toWorldSpace;

							A = A * transfoMatrix;
							B = B * transfoMatrix;
							C = C * transfoMatrix;

							ofVec3f AB = B - A;
							ofVec3f AC = C - A;

							ofVec3f ABxAC = AB.getCrossed(AC);

							ofVec3f n = (ABxAC) / (ABxAC.length());
							ofVec3f v = ray.getTransmissionVector();

							ofVec3f OK = (v.dot(n)) * n;
							ofVec3f OL = 2 * OK;

							ofVec3f w = v - OL;

							ofVec3f S = ray.getStart();
							ofVec3f SA = (A - S);

							float t = (n.dot(SA)) / (n.dot(v));

							ofVec3f OR = S + (t * v);

							w = w.scale(1000);

							mathReflectedRay = ofRay(OR, w, false);
						}
						else if (isGlass)
						{

							ofVec3f A = face->getVertex(0) * toWorldSpace;
							ofVec3f B = face->getVertex(1) * toWorldSpace;
							ofVec3f C = face->getVertex(2) * toWorldSpace;

							A = A * transfoMatrix;
							B = B * transfoMatrix;
							C = C * transfoMatrix;

							ofVec3f AB = B - A;
							ofVec3f AC = C - A;

							ofVec3f ABxAC = AB.getCrossed(AC);

							ofVec3f n = (ABxAC) / (ABxAC.length());
							ofVec3f invN = -n;
							ofVec3f v = ray.getTransmissionVector();

							ofVec3f w = ofVec3f((v.x + n.x) / 2, (v.y + n.y) / 2, (v.z + n.z) / 2);

							ofVec3f S = ray.getStart();
							ofVec3f SA = (A - S);

							float t = (n.dot(SA)) / (n.dot(v));

							ofVec3f OR = S + (t * v);

							w = w.scale(1000);

							mathReflectedRay = ofRay(OR, w, false);
						}

						if (rand() % 800 == 0)
						{
							ray.color = ofColor(255, 0, 0);
							mathReflectedRay.color = ofColor(0, 255, 0);
							returnVec.push_back(ray);
							returnVec.push_back(mathReflectedRay);
						}

						for (auto& otherPrim3D : otherPrims)
						{
							if (otherPrim3D->getName() != getName())
							{
								otherPrim3D->getColorOfRay(mathReflectedRay, &col);
							}
						}
						ofColor lighter = getSlightlyLighterColor(col);
						//face->setColor(0, lighter);
						//face->setColor(1, lighter);
						//face->setColor(2, lighter);
						ofColor otherCol = ofColor(0, 0, 255);
						//ofSetColor(lighter);
						toPaint.getPixels().setColor(x, y, lighter);
						//ofDrawRectangle(x, y, 1, 1);
					}
				}
			}
			mustPrepare = false;
		}
	}
	toPaint.update();
	ofSetColor(255);
	toPaint.draw(0, 0, wid, hei);
	return returnVec;
}

vector<hit> primitive3d::intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) {

	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<hit>* hits = new vector<hit>();

	ofVec3f screenToWorld = cam.screenToWorld(ofVec3f(screenCoordinates.x, screenCoordinates.y, 0.0));
	ofRay ray(cam.getPosition(), screenToWorld - cam.getPosition());
	
	intersectsMesh(ray, mesh, toWorldSpace, hits);

	return *hits;
}

bool primitive3d::getColorOfRay(ofRay ray, ofColor * colHit) {
	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<hit>* hits = new vector<hit>();

	vector<ofMeshFace> allFaces = mesh.getUniqueFaces();

	bool hasHit = intersectsMesh(ray, mesh, toWorldSpace, hits);

	if (hasHit)
	{
		//*hit = allFaces[(*hits)[0]].getColor(0);
		*colHit = fillCol;
	}
	return hasHit;
}