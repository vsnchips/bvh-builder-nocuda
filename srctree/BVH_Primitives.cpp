#include "BVH.hpp"

using namespace std;
using namespace glm;

BVH_BBox bvhTriangle::getBBox(){

BVH_BBox newbb;

vec3 ea = pb - pa; //Anti Clockwise Edge 
vec3 eb = pc - pa; //Clockwise side  Edge

vec3 zNorm = normalize( cross( ea, eb ));
vec3 yNorm = normalize( cross( zNorm, ea ));
vec3 xNorm = normalize(ea);

float A_obtusity = dot(xNorm,eb); // How wide is angle A

 vec3 boxA = pa + glm::min( 0.f, A_obtusity ) * xNorm;
 vec3 boxB = pa + glm::max( length(ea), A_obtusity) * xNorm;

 vec3 y_vec = yNorm * dot(yNorm,eb);
 vec3 boxC = boxB + y_vec;
 vec3 boxD = boxA + y_vec;

vec3 zb = 0.025f * zNorm * length(ea); // adaptive thickness for small polygons

// These points are NOT laid out in Z-order.
// They are right hand thread order.

newbb.points.clear();
newbb.points.push_back(boxA);
newbb.points.push_back(boxB);
newbb.points.push_back(boxC);
newbb.points.push_back(boxD);
newbb.points.push_back(boxA + zb);
newbb.points.push_back(boxB + zb);
newbb.points.push_back(boxC + zb);
newbb.points.push_back(boxD + zb);

newbb.updateBasis();
newbb.isPrim = 1;
return newbb;

}
