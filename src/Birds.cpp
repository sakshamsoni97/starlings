#include "Birds.h"

using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

vec3::vec3(T _x, T _y, T _z){
	x = _x;
	y = _y;
	z = _z; 
}

vec3<T> vec3::operator+(const vec3<T> &v){
	vec3<T> res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

vec3<T> vec3::operator-(const vec3<T> &v){
	vec3<T> res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

vec3<T> vec3::operator/(const float &A){
	vec3<T> res;
	res.x = x/A;
	res.y = y/A;
	res.z = z/A;
	return res;
}

vec3<T> vec3::operator*(const float &A){
	vec3<T> res;
	res.x = x*A;
	res.y = y*A;
	res.z = z*A;
	return res;
}

T vec3::operator*(const vec3<T> &v){
	T res;
	res = x*v.x + y*v.y + z*v.z;
	return res;
}

float vec3::mag(){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}


inline vec3<float> diff(vec3<float> v1, vec3<float> v2){
	return vec3<float>(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
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

void Bird::set_rmax(float rm){
	rmax = rm;
}

void Bird::set_roost(vec3<float> rs){
	roost = rs;
}


vec3<float> Bird::_separation(){
	rsep = (1-S)*rsep + S*rmax*(1 - ((float)friends.size())/Nc);

	if(friends.size()==0)
		return vec3<float>(0.0, 0.0, 0.0);

	vec3<float> fs;
	vector<Bird>::iterator it;
	float g, D, temp;
	vec3<float> d;
	for(it = friends.begin(); it!=friends.end(); it++){
		d = it->pos - pos;
		D = d.mag();
		if(D<=rsep){
			fs = fs + d/D;
		}else{
			temp = D-rsep;
			g = exp(-(temp*temp)/SIGMA_SQ);
			fs = fs + d*g;
		}
	}
	fs = fs*(-WS/((float)friends.size()));

	return fs;
}

vec3<float> Bird::_cohesion(){
	vec3<float> fc;
	vector<Bird>::iterator it;
	int ni = friends.size();
	if(ni==0)
		return vec3<float>(0.0, 0.0, 0.0);

	// TODO: add crowding correction factor
	vec3<float> d;
	float ang, D;
	for(it = friends.begin(); it!=friends.end(); it++){
		d = it->pos - pos;
		D = d.mag();
		ang = (d*dir)/(D*dir.mag())
		if(ang<-0.9){
			ni--;
			continue;
		}
		if(D>rsep){
			fc = fc + d/D;
		}
	}
	fc = fc*(WC/((float)ni));

	return fc;
}

vec3<float> Bird::_allignment(){
	vec3<float> fa;
	
	int ni = friends.size();
	if(ni==0)
		return vec3<float>(0.0, 0.0, 0.0);

	float ang;
	vec3<float> d;
	vector<Bird>::iterator it; 
	for(it = friends.begin(); it!=friends.end(); it++){
		d = it->dir;
		ang = (d*dir)/(D*dir.mag())
		if(ang<-0.9){
			ni--;
			continue;
		}
		fa = fa + d;
	}
	fa = fa - dir;
	fa = fa/fa.mag();
	fa = fa*WA;

	return fa;	
}

vec3<float> Bird::_attraction_to_roost(){
	vec3<float> fr(0.0, 0.0, 0.0);
	fr.z = -WRV*(pos.z - roost.z);
	vec3<float> n = pos - roost;
	n.z = 0;
	N = n.mag();
	if(N == 0.0)
		return fr;
	n = n/N;
	vec3<float> frh(dir.x, -dir.y, 0);
	int s = sgn(elat*n);
	frh = frh*((float) s)*WRH*(0.5+(dir*n)*0.5);
	fr = fr + frh;
	return fr;
}

void Bird::getFriends(vector<Bird> *new_friends){
	friends = new_friends;
}

void Bird::calcNetForce(){
	vec3<float> Fsocial = _separation() + _cohesion() + _allignment();
	vec3<float> Froost = _attraction_to_roost();
}