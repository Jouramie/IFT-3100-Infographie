#include "primitive.h"

primitive::primitive()
{
	prim = 0;
	fillCol = ofColor(255, 255, 255);
}

primitive::primitive(of3dPrimitive* primitive)
{
	prim = primitive;
	fillCol = ofColor(255, 255, 255);
}

primitive::primitive(of3dPrimitive* primitive, ofColor fill)
{
	prim = primitive;
	fillCol = fill;
}

of3dPrimitive* primitive::getPrimitive() {
	return prim;
}

bool primitive::getSelected() {
	return selected;
}

void primitive::changeSelected() {
	selected = !selected;
}

void primitive::setSelected(bool val) {
	selected = val;
}

ofColor primitive::getFillColor() {
	return fillCol;
}

void primitive::draw(bool wireframe) {
	//ofDisableAlphaBlending();
	//ofTexture texture = ofTexture();
	//texture.allocate(5, 5, GL_TEXTURE_INTERNAL_FORMAT);
	//texture.bind();
	if (wireframe || selected)
		prim->drawWireframe();
	else
		prim->draw();
}

bool primitive::calcTriangleIntersection(ofRay ray, float *result) const {

	ofMesh mesh = prim->getMesh();
	std::vector<ofMeshFace> indices = mesh.getUniqueFaces();

	for (std::vector<ofMeshFace>::iterator i = indices.begin(); i != indices.end(); ++i) {

		ofMeshFace face = *i;

		ofVec3f edge1, edge2, tvec, pvec, qvec;
		float det;
		float u, v;
		const float EPSILON = 0.000001f;

		edge1 = face.getVertex(1) - face.getVertex(0);
		edge2 = face.getVertex(2) - face.getVertex(0);

		pvec = ray.t.getCrossed(edge2);
		det = edge1.dot(pvec);

#if 0 // we don't want to backface cull
		if (det >= EPSILON)
		{
			tvec = getOrigin() - vert0;

			u = tvec.dot(pvec);
			if (!((u < 0.0f) || (u > det)))
			{

				qvec = tvec.getCrossed(edge1);
				v = getDirection().dot(qvec);
				if (!(v < 0.0f || u + v > det))
				{

					*result = edge2.dot(qvec) / det;
					return true;
				}
			}
		}
#else
		if (!(det > -EPSILON && det < EPSILON))
		{
			float inv_det = 1.0f / det;
			tvec = ray.s - face.getVertex(0);
			u = tvec.dot(pvec) * inv_det;
			if (!(u < 0.0f || u > 1.0f))
			{

				qvec = tvec.getCrossed(edge1);

				v = ray.t.dot(qvec) * inv_det;
				if (!(v < 0.0f || u + v > 1.0f))
				{

					*result = edge2.dot(qvec) * inv_det;
					return true;
				}
			}
		}
#endif
	}
	return false;
}

bool primitive::checkIntersectionPlaneAndLine(ofRay ray, float *result) const {

	ofMesh mesh = prim->getMesh();
	std::vector<ofMeshFace> indices = mesh.getUniqueFaces();

	for (std::vector<ofMeshFace>::iterator i = indices.begin(); i != indices.end(); ++i)
	{

		ofMeshFace face = *i;

		ofVec3f P1, P2;
		P1 = ray.s;
		P2 = ray.s + ray.t;

		ofVec3f p1, p2, p3;
		p1 = face.getVertex(0);
		p2 = face.getVertex(1);
		p3 = face.getVertex(2);

		ofVec3f v1 = p1 - p2;
		ofVec3f v2 = p3 - p2;

		float a, b, c, d;

		a = v1.y * v2.z - v1.z * v2.y;
		b = -(v1.x * v2.z - v1.z * v2.x);
		c = v1.x * v2.y - v1.y * v2.x;
		d = -(a * p1.x + b * p1.y + c * p1.z);

		ofVec3f O = P1;
		ofVec3f V = P2 - P1;

		float t;

		t = -(a * O.x + b * O.y + c * O.z + d) / (a * V.x + b * V.y + c * V.z);

		ofVec3f p = O + V * t;

		float xmin = std::min(P1.x, P2.x);
		float ymin = std::min(P1.y, P2.y);
		float zmin = std::min(P1.z, P2.z);

		float xmax = std::max(P1.x, P2.x);
		float ymax = std::max(P1.y, P2.y);
		float zmax = std::max(P1.z, P2.z);


		if (inside(p, xmin, xmax, ymin, ymax, zmin, zmax)) {
			*result = p.length();
			return true;
		}
	}
	return false;
}

bool primitive::inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const {

	if (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax && p.z >= zmin && p.z <= zmax)
		return true;

	return false;

}
