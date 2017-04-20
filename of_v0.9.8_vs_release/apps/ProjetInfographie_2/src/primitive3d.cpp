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

	/*ofPoint pointa = ofPoint(0, -200, 0);
	ofPoint pointb = ofPoint(100, 100, 0);
	ofPoint pointc = ofPoint(-100, 100, 0);

	ofTriangle(pointa, pointb, pointc);
	return;*/

	ofPushMatrix();
	ofTranslate(transfoMatrix.getTranslation());

	//ofSetColor(fillCol);

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
		for (auto face : prim->getMesh().getUniqueFaces())
		{
			ofSetColor(face.getColor(0));
			
			//prim->getMesh().dr;
		}
		prim->drawFaces();
	}

	ofPopMatrix();
}

ofColor getSlightlyLighterColor(ofColor original) {
	int red = original.r;
	int green = original.g;
	int blue = original.b;

	red = red * 1.1;
	green = green * 1.1;
	blue = blue * 1.1;

	int maxColorValue = ofColor::limit();

	if (red > maxColorValue)
		red = maxColorValue;
	if (green > maxColorValue)
		green = maxColorValue;
	if (blue > maxColorValue)
		blue = maxColorValue;

	return ofColor(red, green, blue);
}

bool primitive3d::prepareGlass(const ccamera cam, vector<primitive*> otherPrims, ofColor backgroundCol) {//, const scene * scn) {
	if (isMirror || isGlass)
	{
		if (mustPrepare) {
			ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();

			ofMesh * mesh = prim->getMeshPtr();

			vector<ofMeshFace> allFaces = mesh->getUniqueFaces();

			int wid = ofGetWidth();
			int hei = ofGetHeight();

			int firstX = wid;
			int lastX = 0;
			int firstY = hei;
			int lastY = 0;

			for (int i = 0; i < allFaces.size(); i++) {
				for (int j = 0; j < 3; j++)
				{
					ofVec3f screen = (*cam).worldToScreen(allFaces[i].getVertex(0) * toWorldSpace);
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

			for (int x = firstX; x <= lastX; x++)
			{
				for (int y = firstY; y <= lastY; y++)
				{
					ofVec2f coords = ofVec2f(x, y);
					vector<int>* hits = new vector<int>();

					ofVec3f screenToWorld = (*cam).screenToWorld(ofVec3f(coords.x, coords.y, 0.0));
					ofRay ray((*cam).getPosition(), screenToWorld - (*cam).getPosition());

					if (intersectsMesh(ray, *mesh, toWorldSpace, hits)) {

						for (int iter = 0; iter < hits->size(); iter++) {
							ofMeshFace * face = &allFaces[(*hits)[iter]];

							ofColor col = backgroundCol;

							if (isMirror) {
								ofVec3f normal = face->getFaceNormal();
								ofVec3f transmi = ray.getTransmissionVector().getNormalized();
								ofVec3f start = ray.getStart();
								ofVec3f k = face->getVertex(1);
								ofVec3f TVPlusP = k / normal;
								ofVec3f TV = TVPlusP - start;
								ofVec3f t = TV / transmi;
								ofRay reflectedRay = ofRay(t, transmi + (2 * (transmi * normal) * transmi));

								for (auto& otherPrim3D : otherPrims)
								{
									if (otherPrim3D->getName() != getName())
									{
										if (otherPrim3D->getColorOfRay(reflectedRay, &col))
											iter = hits->size() + 1;
									}
								}
							}
							ofColor lighter = getSlightlyLighterColor(col);
							//face->setColor(0, lighter);
							//face->setColor(1, lighter);
							//face->setColor(2, lighter);

							ofSetColor(lighter);
							ofDrawRectangle(x, y, 1, 1);
						}
					}
				}
			}

			/*for (int i = 0; i < allFaces.size(); i++) {
				ofVec3f screen1 = (*cam).worldToScreen(allFaces[i].getVertex(0) * toWorldSpace);
				ofVec2f coords = ofVec2f(screen1.x, screen1.y);
				vector<int>* hits = new vector<int>();

				ofVec3f screenToWorld = (*cam).screenToWorld(ofVec3f(coords.x, coords.y, 0.0));
				ofRay ray((*cam).getPosition(), screenToWorld - (*cam).getPosition());

				if (intersectsMesh(ray, *mesh, toWorldSpace, hits)) {
					for (int iter = 0; iter < hits->size(); iter++) {
						ofMeshFace * face = &allFaces[(*hits)[iter]];

						ofColor col = backgroundCol;

						if (isMirror) {
							ofVec3f normal = face->getFaceNormal();
							ofVec3f transmi = ray.getTransmissionVector().getNormalized();
							ofVec3f start = ray.getStart();
							ofVec3f k = face->getVertex(1);
							ofVec3f TVPlusP = k / normal;
							ofVec3f TV = TVPlusP - start;
							ofVec3f t = TV / transmi;
							ofRay reflectedRay = ofRay(t, transmi + (2 * (transmi * normal) * transmi));

							for (auto& otherPrim3D : otherPrims)
							{
								if (otherPrim3D->getName() != getName())
								{
									if (otherPrim3D->getColorOfRay(reflectedRay, &col))
										iter = hits->size() + 1;
								}
							}
						}
						ofColor lighter = getSlightlyLighterColor(col);
						face->setColor(0, lighter);
						face->setColor(1, lighter);
						face->setColor(2, lighter);
						mesh->setColor(i, lighter);
					}
				}
			}*/
			mustPrepare = false;
		}
	}
	return true;
}

bool primitive3d::intersectsMeshInstance(const ofVec2f &screenCoordinates, const ofCamera &cam) {

	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<int>* hits = new vector<int>();

	ofVec3f screenToWorld = cam.screenToWorld(ofVec3f(screenCoordinates.x, screenCoordinates.y, 0.0));
	ofRay ray(cam.getPosition(), screenToWorld - cam.getPosition());
	
	return intersectsMesh(ray, mesh, toWorldSpace, hits);
}

bool primitive3d::getColorOfRay(ofRay ray, ofColor * hit) {
	ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();
	ofMesh mesh = prim->getMesh();

	vector<int>* hits = new vector<int>();

	vector<ofMeshFace> allFaces = mesh.getUniqueFaces();

	bool hasHit = intersectsMesh(ray, mesh, toWorldSpace, hits);

	if (hasHit)
	{
		//*hit = allFaces[(*hits)[0]].getColor(0);
		*hit = fillCol;
	}
	return hasHit;
}