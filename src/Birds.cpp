#include "Birds.h"

using namespace std;

void normalize(vec3<float> &v){
	float V = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x = v.x/V;
	v.y = v.y/V;
	v.z = v.z/V;
}

Bird::Bird(vec3<float> p){
	pos = p;
	dir.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
}

Bird::Bird(){
	pos.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	pos.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	pos.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
}