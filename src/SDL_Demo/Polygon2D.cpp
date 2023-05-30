#include "Polygon2D.h"
#include <BBox2D.h>
#include <Plane2D.h>
#include <PlaneSet2D.h>

// setup infinite polygon capped by one plane
// normal facing outwards
void Polygon2D::fromPlane(const class Plane2D &pl, float maxWorldSize) {
	points.clear();
	Line2D l;
	pl.getLine(l,maxWorldSize);
	points.push_back(l.getA());
	points.push_back(l.getB());
	// normal faces outwards, so ground direction is negative normal
	points.push_back(l.getB() - pl.getNormal() * maxWorldSize);
	points.push_back(l.getA() - pl.getNormal() * maxWorldSize);
	
	ASSERT_FLOAT_EQUALS(pl.distanceTo(points[2]),-maxWorldSize,0.1f,"Far infinite point of poly to plane must be at maxWorldSize dist")
	ASSERT_FLOAT_EQUALS(pl.distanceTo(points[3]),-maxWorldSize,0.1f,"Far infinite point of poly to plane must be at maxWorldSize dist")
}
// removes everything in front of plane
void Polygon2D::clipByPlane(const Plane2D &pl) {
/*	for(int i = 0; i < points.size(); i++) {
		int j = (i+1)%points.size();
		Vec2D a = points[i];
		Vec2D b = points[j];
		Vec2D c;
		PlaneLineRelation rel = pl.intersectLineExt(a,b,&c);
		if(rel == PLR_Front){
		} else if(rel == PLR_Intersects) {

		}
	}*/
	byte cnts[4] = { 0,0,0,0 };
	float *dists = (float*) alloca(points.size()*sizeof(float));
	byte *rels = (byte*) alloca(points.size()*sizeof(byte));
	for(int i = 0; i < points.size(); i++){
		rels[i] = pl.getPointPlaneRelation(points[i],&dists[i]);
		cnts[rels[i]]++;
	}
	if(cnts[PS_FRONT] == 0) {
		// nothing to remove
		return;
	}
	if(cnts[PS_BACK] == 0) {
		// all to remove
		points.clear();
		return;
	}
	Array<Vec2D> nA;
	for(int i = 0; i < points.size(); i++){
		const Vec2D &p = points[i];
		if(rels[i] == PS_ON) {
			nA.push_back(p);
			continue;
		}
		if(rels[i] == PS_BACK) {
			nA.push_back(p);
		}
		int ni = (i+1)%points.size();
		if(rels[ni] == PS_ON || rels[ni] == rels[i]) {
			continue;
		}
		const Vec2D &p2 = points[ni];
		float d = dists[i] / (dists[i]-dists[ni]);
		nA.push_back(p.lerpTo(p2,d));
		//Vec2D tmp;
		//pl.intersectLine(p,p2,&tmp);
		//nA.push_back(tmp);
	}
	points = nA;
}
void Polygon2D::addToBB(class BBox2D &bb) const {
	for(int i = 0; i < points.size(); i++){
		bb.addPoint(points[i]);
	}
}
void Polygon2D::fromPlanes(const PlaneSet2D &planes) {
	points.clear();
	if(planes.size() < 3) {
		return;
	}
	BBox2D bb;
	for(int i = 0; i < planes.size(); i++){
		int j = (i+1)%planes.size();
		Vec2D p;
		PlaneRelation pr = planes[i].intersectPlane(planes[j],p);
		if(pr == PR_Intersecting) {
			bb.addPoint(p);
		}
	}
	float maxCoord = bb.getMaxExtentFromCenter();
	// TODO: first calculate bounding box for all simple plane vs plane intersections 
	// and then use it for max coord
	fromPlane(planes[0],maxCoord*2);
	for(int i = 1; i < planes.size(); i++) {
		clipByPlane(planes[i]);
	}
}
