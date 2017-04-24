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

void primitive3d::draw(bool wireframe, ofxShadersFX::Lighting::LightingShader& lightShader) {

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

		lightShader.useMaterial(&mat);
		lightShader.begin();
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
	
		lightShader.end();
		lightShader.removeMaterial();
		ofPopMatrix();
	}
}

// Rend une couleur plus pâle, pour éviter les miroirs invisibles
ofColor getSlightlyLighterColor(ofColor original, float ratio = 1.5) {
	int red = original.r;
	int green = original.g;
	int blue = original.b;

	red = red * ratio;
	green = green * ratio;
	blue = blue * ratio;

	int maxColorValue = ofColor::limit();

	// Évidemment, si une des composantes dépasse la limite, on la ramène.
	// On est pas Tide(tm), on ne veut pas que les choses soient plus blanc que blanc!
	if (red > maxColorValue)
		red = maxColorValue;
	if (green > maxColorValue)
		green = maxColorValue;
	if (blue > maxColorValue)
		blue = maxColorValue;

	return ofColor(red, green, blue);
}

// Préparation de la vitre/miroir, seulement si nécéssaire
vector<ofRay> primitive3d::prepareGlass(const ofCamera cam, vector<primitive*> otherPrims, ofColor backgroundCol) {//, const scene * scn) {
	vector<ofRay> returnVec = vector<ofRay>();
	int wid = ofGetWidth();
	int hei = ofGetHeight();

	// Si la primitive n'est ni un miroir ni une vitre, on n'a pas notre place ici!
	if (isMirror || isGlass)
	{	
		// Un miroir n'a pas besoin de couleur!
		fillCol = ofColor(255, 255, 255, 0);
		
		// Si un rafraichissement manuel a été demandé depuis la dernière fois.
		if (mustPrepare) {
			
			// matrice de transformation globale
			ofMatrix4x4 toWorldSpace = prim->getGlobalTransformMatrix();

			// Le mesh de la primitive
			ofMesh * mesh = prim->getMeshPtr();

			// Et ses faces
			vector<ofMeshFace> allFaces = mesh->getUniqueFaces();

			int firstX = wid;
			int lastX = 0;
			int firstY = hei;
			int lastY = 0;

			// Ça ne sert à rien d'envoyer des rayons sur des pixels qui n'ont aucune chance de toucher la primitive.
			// À des fins d'optimisation, on détecte les limites (en x et y) du rectangle qui contient la primitive, pour ne tester que ces rayons.
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

			// On ne lance pas de rayon en dehors de l'écran, voyons. tss tss.
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

			// Dans notre rectangle, feu à volonté!
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

					// On lance un rayon sur chaque pixel en cherchant si il intersecte notre miroir.
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

					// Si on a intersecté notre miroir/vitre, ET qu'on a rien intersecté de plus proche avec le même rayon, on peut travailler.
					if (distance > -0.9 && toSelect->getName() == getName()) {
						// La face touchée
						const ofMeshFace * face = &allFaces[(realHits)[0].faceIndex];

						ofColor col = backgroundCol;

						ofRay mathReflectedRay = ofRay();

						if (isMirror) {
							// Les sommets du triangle touché
							ofVec3f A = face->getVertex(0) * toWorldSpace;
							ofVec3f B = face->getVertex(1) * toWorldSpace;
							ofVec3f C = face->getVertex(2) * toWorldSpace;

							A = A * transfoMatrix;
							B = B * transfoMatrix;
							C = C * transfoMatrix;

							// Des maths. Sont assez self-explanatory pour ceux qui connaissent ça, et prendraient des dizaines de lignes de commentaires à expliquer sinon.
							// Good luck!
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

							// Juste pour que le rayon soit visible (mais pas infini) quand on l'affiche pour déboguer
							w = w.scale(1000);

							// Long story short, on a trouvé l'origine et la direction du rayon refleté, donc c'est cool
							mathReflectedRay = ofRay(OR, w, false);
						}
						else if (isGlass)
						{

							// Si on est une vitre, on fait sensiblement la même chose
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

							// Sauf ici.
							ofVec3f w = ofVec3f((v.x + n.x) / 2, (v.y + n.y) / 2, (v.z + n.z) / 2);

							ofVec3f S = ray.getStart();
							ofVec3f SA = (A - S);

							float t = (n.dot(SA)) / (n.dot(v));

							ofVec3f OR = S + (t * v);

							w = w.scale(1000);

							// Basically, on part vers l'intérieur plutôt que vers l'extérieur, et on génére encore le beau rayon tout neuf.
							mathReflectedRay = ofRay(OR, w, false);
						}
						
						// Si on veut déboguer les rayons, tous les afficher serait un méchant désordre. On en affiche donc seulement 1/800, choisis au hasard, ça nous en laisse assez pour voir les problèmes, mais pas trop pour comprendre leur comportement.
						if (rand() % 800 == 0)
						{
							ray.color = ofColor(255, 0, 0);
							mathReflectedRay.color = ofColor(0, 255, 0);
							returnVec.push_back(ray);
							returnVec.push_back(mathReflectedRay);
						}

						// Avec notre nouveau rayon, on va chercher la première couleur qu'il touche!
						// Si personne le touche, on se retrouve avec la couleur de fond, car on avait initialisé col à ça.
						for (auto& otherPrim3D : otherPrims)
						{
							if (otherPrim3D->getName() != getName())
							{
								otherPrim3D->getColorOfRay(mathReflectedRay, &col);
							}
						}
						
						// Un peu plus pâle, les miroirs invisibles c'est villain. On fonce dedans comme un moineau dans une porte patio,
						ofColor lighter = getSlightlyLighterColor(col);
						
						toPaint.getPixels().setColor(x, y, lighter);
					}
				}
			}
			
			// Pas besoin de rafraichir avant de se le faire redemander!
			mustPrepare = false;
		}
	}
	
	// On dessine par dessus le cube notre belle reflexion pas du tout pixelisée. Toute qu'une oeuvre d'art!
	toPaint.update();
	ofSetColor(255);
	toPaint.draw(0, 0, wid, hei);
	
	// Ah et on retourne un rayon sur 800, si jamais le renderer voulait les avoir pour déboguer.
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

// Permet d'obtenir la couleur de la primitive Si et seulement si elle est touchée par un rayon
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