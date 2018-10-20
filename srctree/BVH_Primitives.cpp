#include "BVH.hpp"

using namespace std;
using namespace glm;

BVH_BBox bvhTriangle::getBBox(){

BVH_BBox newbb;

vec3 ea = pb - pa;
vec3 eb = pc - pa;
vec3 zc = cross( normalize(ea), normalize(eb));
vec3 yc = cross( zc, normalize(ea));

vec3 xb = ea;
vec3 yb = dot(yc, eb) * yc;
vec3 zb = 0.001f * length(ea) * zc; // adaptive thickness for small polygons

newbb.points.clear();
newbb.points.push_back(pa);
newbb.points.push_back(pa + xb);
newbb.points.push_back(pa + xb + yb);
newbb.points.push_back(pa + yb);
newbb.points.push_back(pa + zb);
newbb.points.push_back(pa + zb + xb);
newbb.points.push_back(pa + zb + xb + yb);
newbb.points.push_back(pa + zb + yb);

newbb.updateBasis();
newbb.isPrim = 1;
return newbb;

}
