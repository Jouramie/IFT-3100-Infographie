#include "primitive.h"

const ofVec3f primitive::getGlobalPosition() const
{
	ofNode dummy = ofNode();
	dummy.setPosition(getLocalPosition());
	dummy.setTransformMatrix(transfoMatrix);
	return dummy.getGlobalPosition();
}

struct hit {
	int faceIndex;
	float distance;
};

struct by_distance {
	bool operator()(hit const &a, hit const &b) {
		return a.distance < b.distance;
	}
};

bool primitive::intersectsMesh(ofRay ray, const ofMesh &mesh, const ofMatrix4x4 &toWorldSpace, vector<int> *meshHit) {
	const vector<ofMeshFace>& faces = mesh.getUniqueFaces();
	bool intersection = false;
	bool intersectedOnce = false;
	vector<hit> distances = vector<hit>();
	for (int i = 0; i < faces.size(); i++) {
		const ofMeshFace &face = faces[i];
		// intersections are done worldSpace
		ofVec3f one = face.getVertex(0) * toWorldSpace;
		ofVec3f two = face.getVertex(1) * toWorldSpace;
		ofVec3f three = face.getVertex(2) * toWorldSpace;
		one = one * transfoMatrix;
		two = two * transfoMatrix;
		three = three * transfoMatrix;

		float t;
		intersection = calcTriangleIntersection(one, two, three, ray, &t);

		if (intersection && t > 0) {
			hit newHit = hit();
			newHit.distance = t;
			newHit.faceIndex = i;
			distances.push_back(newHit);
			intersectedOnce = true;
			//break;
		}
	}

	if (intersectedOnce)
	{
		std::sort(distances.begin(), distances.end(), by_distance());
		for (int i = 0; i < distances.size(); i++)
		{
			meshHit->push_back(distances[i].faceIndex);
		}
	}

	return intersectedOnce;
}

bool primitive::calcTriangleIntersection(const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, ofRay ray, float *result) const
{

	ofVec3f edge1, edge2, tvec, pvec, qvec;
	float det;
	float u, v;
	const float EPSILON = 0.000001f;

	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;

	pvec = ray.getTransmissionVector().getCrossed(edge2);
	det = edge1.dot(pvec);

#if 0 // we don't want to backface cull
	if (det < EPSILON)
		return false;
	tvec = getOrigin() - vert0;

	u = tvec.dot(pvec);
	if ((u < 0.0f) || (u > det))
		return false;

	qvec = tvec.getCrossed(edge1);
	v = getDirection().dot(qvec);
	if (v < 0.0f || u + v > det)
		return false;

	*result = edge2.dot(qvec) / det;
	return true;
#else
	if (det > -EPSILON && det < EPSILON)
		return false;

	float inv_det = 1.0f / det;
	tvec = ray.getStart() - vert0;
	u = tvec.dot(pvec) * inv_det;
	if (u < 0.0f || u > 1.0f)
		return false;

	qvec = tvec.getCrossed(edge1);

	v = ray.getTransmissionVector().dot(qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
		return 0;

	*result = edge2.dot(qvec) * inv_det;
	return true;
#endif
}

bool primitive::calcPlaneIntersection(const ofVec3f &planeOrigin, const ofVec3f &planeNormal, ofRay ray, float *result) const
{
	float denom = planeNormal.dot(ray.getTransmissionVector());

	if (denom != 0.0f) {
		*result = planeNormal.dot(planeOrigin - ray.getStart()) / denom;
		return true;
	}
	return false;
}
