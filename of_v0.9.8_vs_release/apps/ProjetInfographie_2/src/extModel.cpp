#include "extModel.h"

extModel::extModel(ofxAssimpModelLoader* mod) : extModel(mod, ofMatrix4x4())
{

}

extModel::extModel(ofxAssimpModelLoader* mod, ofMatrix4x4 transfo)
{
	model = mod;
	transfo = transfo;
	selected = ofParameter<bool>(false);
}

ofxAssimpModelLoader* extModel::getModel() {
	return model;
}

bool extModel::getSelected() {
	return selected;
}

void extModel::changeSelected() {
	selected = !selected;
}

void extModel::setSelected(bool val) {
	selected = val;
}

void extModel::draw(bool wireframe) {

	ofPushMatrix();

	ofScale(transfo.getScale());

	ofQuaternion rotation = transfo.getRotate();
	float rotationAmount;
	ofVec3f rotationAngle;
	rotation.getRotate(rotationAmount, rotationAngle);

	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);

	ofTranslate(transfo.getTranslation());

	if (wireframe || selected)
		model->drawWireframe();
	else
		model->drawFaces();

	ofPopMatrix();
}

bool extModel::calcTriangleIntersection(ofRay ray, float *result) const {

	vector<string> meshNames = model->getMeshNames();

	for (std::vector<string>::iterator name = meshNames.begin(); name != meshNames.end(); ++name) {

		ofMesh mesh = model->getMesh(*name);
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
	}
	return false;
}

bool extModel::checkIntersectionPlaneAndLine(ofRay ray, float *result) const {

	vector<string> meshNames = model->getMeshNames();

	for (std::vector<string>::iterator name = meshNames.begin(); name != meshNames.end(); ++name) {

		ofMesh mesh = model->getMesh(*name);
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
	}
	return false;
}

bool extModel::inside(ofVec3f p, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) const {

	if (p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax && p.z >= zmin && p.z <= zmax)
		return true;

	return false;

}

#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

bool extModel::checkIntersectionTriangleRay(ofRay ray, ofPoint* inter)
{

	vector<string> meshNames = model->getMeshNames();

	for (std::vector<string>::iterator name = meshNames.begin(); name != meshNames.end(); ++name) {

		ofMesh mesh = model->getMesh(*name);
		std::vector<ofMeshFace> indices = mesh.getUniqueFaces();

		for (std::vector<ofMeshFace>::iterator i = indices.begin(); i != indices.end(); ++i)
		{
			ofMeshFace triangle = *i;

			ofVec3f   u, v, n;              // Vecs du triangle
			ofVec3f   dir, w0, w;           // Vecs du ofRay
			float     r, a, b;              // params to calc ray-plane intersect

											// get triangle edge vectors and plane normal
			u = triangle.getVertex(1) - triangle.getVertex(0);
			v = triangle.getVertex(2) - triangle.getVertex(0);
			n = u * v;              // cross product
			if (!(n == ofVec3f(0, 0, 0)))           // if triangle is not degenerate
			{

				dir = ray.getEnd() - ray.getStart();              // ray direction vector
				w0 = ray.getStart() - triangle.getVertex(0);
				a = -dot(n, w0);
				b = dot(n, dir);
				if (!(fabs(b) < SMALL_NUM))
				{     // if ray is not parallel to triangle

					  // get intersect point of ray with triangle plane
					r = a / b;
					if (!(r < 0.0))                    // ray goes toward the triangle
					{
						// for a segment, also test if (r > 1.0) => no intersect

						*inter = ray.getStart() + r * dir;            // intersect point of ray and plane

																	  // is I inside T?
						float    uu, uv, vv, wu, wv, D;
						uu = dot(u, u);
						uv = dot(u, v);
						vv = dot(v, v);
						w = *inter - triangle.getVertex(0);
						wu = dot(w, u);
						wv = dot(w, v);
						D = uv * uv - uu * vv;

						// get and test parametric coords
						float s, t;
						s = (uv * wv - vv * wu) / D;
						if (!(s < 0.0 || s > 1.0))         // I is inside T
						{
							t = (uv * wu - uu * wv) / D;
							if (!(t < 0.0 || (s + t) > 1.0))  // I is inside T
								return true;                       // I is in T
						}
					}
				}
			}
		}
	}
	return false;
}
