#include "Birds.h"
#include <iostream>
using namespace std;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <class T>
vec3<T>::vec3(T _x, T _y, T _z){
	x = _x;
	y = _y;
	z = _z; 
}

template <class T>
vec3<T>::vec3(){
	x = 0.0;
	y = 0.0;
	z = 0.0; 
}

template <class T>
vec3 <T> vec3<T>::operator+(const vec3 <T> &v){
	vec3 <T> res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

template <class T>
vec3 <T> vec3<T>::operator-(const vec3 <T> &v){
	vec3 <T> res;
	res.x = x+v.x;
	res.y = y+v.y;
	res.z = z+v.z;
	return res;
}

template <class T>
vec3 <T> vec3<T>::operator/(const float &A){
	vec3 <T> res;
	res.x = x/A;
	res.y = y/A;
	res.z = z/A;
	return res;
}

template <class T>
vec3 <T> vec3<T>::operator*(const float &A){
	vec3 <T> res;
	res.x = x*A;
	res.y = y*A;
	res.z = z*A;
	return res;
}

template <class T>
T vec3<T>::operator*(const vec3 <T> &v){
	T res;
	res = x*v.x + y*v.y + z*v.z;
	return res;
}

template <class T>
float vec3<T>::mag(){
	return sqrt(x*x + y*y + z*z);
}

float Bird::rsep;
float Bird::v0; // magnitude of SS speed
float Bird::mass;
float Bird::rmax;
vec3 <float> Bird::roost;

Bird::Bird(vec3 <float> p){
	pos = p;
	dir.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.z = 0.0;
	dir = dir/dir.mag();
	rmet = rmax;
}

Bird::Bird(){
	pos.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10 -5;
	pos.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10 -5;
	pos.z = 0.0;
	dir.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.z = 0.0;
	dir = dir/dir.mag();
	rmet = rmax;
}

void Bird::set_rmax(float rm){
	rmax = rm;
}

void Bird::set_roost(vec3 <float> rs){
	roost = rs;
}

void Bird::set_mass(float m){
	mass = m;
}

void Bird::set_v0(float v){
	v0 = v;
}

void Bird::Init(float rm, float rsp, float m, float v, vec3 <float> rst){
	rmax = rm;
	rsep = rsp;
	roost = rst;
	mass = m;
	v0 = v;
}


vec3 <float> Bird::_separation(){
	if(friends.size()==0)
		return vec3 <float>(0.0, 0.0, 0.0);

	vec3 <float> fs;
	vector<Bird>::iterator it;
	float g, D, temp;
	vec3 <float> d;
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

vec3 <float> Bird::_cohesion(){
	vec3 <float> fc;
	vector<Bird>::iterator it;
	int ni = friends.size();
	if(ni==0)
		return vec3 <float>(0.0, 0.0, 0.0);

	// TODO: add crowding correction factor
	vec3 <float> d;
	float ang, D;
	for(it = friends.begin(); it!=friends.end(); it++){
		d = it->pos - pos;
		D = d.mag();
		if(D==0.0)
			continue;
		ang = (d*dir)/(D*dir.mag());
		if(ang<-0.9){
			ni--;
			continue;
		}
		if(D>rsep){
			fc = fc + d/D;
		}
	}
	if(ni==0)
		return vec3 <float>(0.0, 0.0, 0.0);
	fc = fc*(WC/((float)ni));

	return fc;
}

vec3 <float> Bird::_allignment(){
	vec3 <float> fa;
	
	int ni = friends.size();
	if(ni==0)
		return vec3 <float>(0.0, 0.0, 0.0);

	float ang, D;
	vec3 <float> d;
	vector<Bird>::iterator it; 
	for(it = friends.begin(); it!=friends.end(); it++){
		d = it->dir;
		D = d.mag();
		ang = (d*dir)/(D*dir.mag());
		if(ang<-0.99){
			ni--;
			continue;
		}
		fa = fa + d;
	}
	fa = fa - dir;
	float FA = fa.mag();
	if(FA!=0.0)
		fa = fa/fa.mag();
	fa = fa*WA;

	return fa;	
}

vec3 <float> Bird::_attraction_to_roost(){
	vec3 <float> fr(0.0, 0.0, 0.0);
	fr.z = -WRV*(pos.z - roost.z);
	vec3 <float> n = pos - roost;
	n.z = 0;
	float N = n.mag();
	if(N == 0.0)
		return fr;
	n = n/N;
	vec3 <float> frh(dir.y, -dir.x, 0);
	int s = -sgn(frh*n);
	frh = frh*((float) s)*WRH*(0.5+(dir*n)*0.5);
	fr = fr + frh;
	return fr;
}

vec3 <float> Bird::_generate_noise(){
	vec3 <float> ns;
	ns.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*1 -0.5;
	ns.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*1 -0.5;
	ns.z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*1 -0.5;
	float N = ns.mag();
	if(N!=0.0)
		ns = ns/N;
	return ns;
}

void Bird::updateRmet(){
	rmet = (1-S)*rmet + S*rmax*(1 - ((float)friends.size()/Nc));
}

void Bird::setFriends(vector<Bird> new_friends){
	friends = new_friends;
}

void Bird::calcNetForce(){
	/*vec3 <float> Fsep = _separation();
	vec3 <float> Fcoh = _cohesion();
	vec3 <float> Frst = _attraction_to_roost();
	vec3 <float> Fno = _generate_noise();*/
	vec3 <float> Ft = dir*100*mass*(v0 - vc);
	//vec3 <float> Fsocial = Fsep + Fcoh + Frst + Fno ;
	vec3 <float> Fsocial = _separation() + _cohesion() + _attraction_to_roost() + _generate_noise();
	float vsq_ratio = (vc*vc)/(v0*v0);
	float temp = mass*G*(1-vsq_ratio); 

	vec3 <float> Fflight((CDCL)*temp, -temp, 0.0);

	net_force = Fsocial+Fflight+Ft;
	if(!isfinite(net_force.mag())){
		cout<<"noooooooooooo"<<endl;
		float x = vsq_ratio;
	}
}

void Bird::updateSpeedNPos(){
	vec3 <float> velocity = dir*vc;
	velocity = velocity + (net_force/mass)*DT;

	pos = pos + velocity*DT;

	vc = velocity.mag();
	if(!isfinite(pos.mag()))
		cout<<net_force.mag()<<endl;
	if(vc!=0.0)
		dir = velocity/vc;
}

vector<Bird> flock;

void Env::_create_flock(int Num){
	if(!flock.empty())
		flock.clear();
	for(int i =0; i<Num; i++){
		Bird b;
		//cout<<"this---->"<<flock.size()<<endl;
		flock.push_back(b);
	}
}

Env::Env(vec3 <float> rst, int Num ,float rm, float rsp, float m, float v){
	Bird::Init(rm,rsp,m,v,rst);
	_create_flock(Num);
}

void Env::display(){
	runTimeStep();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen
 
   glBegin(GL_LINE_LOOP);                // Begin drawing the color cube with 6 quads
   	  glLineWidth(6);
      glColor3f(1.0f, 0.0f, 0.0f);     // Green
      glVertex3f( 10.0f, 10.0f, 0.0f);
      glVertex3f( 10.0f, -10.0f, 0.0f);
      glVertex3f(-10.0f, -10.0f, 0.0f);
      glVertex3f(-10.0f, 10.0f, 0.0f);

    glEnd();  // End of drawing color-cube
 

	glTranslatef(0.0f, 0.0f, 50.0f);
	vector<Bird>::iterator it;
	for(it = flock.begin(); it!=flock.end(); it++){
		glTranslatef(SF*it->pos.x, SF*it->pos.y, SF*it->pos.z);
		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);     
		vec3 <float> p1 = it->dir;
		vec3 <float> p2(-it->dir.y, it->dir.x, it->pos.z);
		vec3 <float> p3(it->dir.y, -it->dir.x, it->pos.z);
		glVertex3f(SF*p1.x, SF*p1.y, SF*p1.z);
		glVertex3f(SF*p2.x, SF*p2.y, SF*p2.z);
		glVertex3f(SF*p3.x, SF*p3.y, SF*p3.z);
		glEnd();
		glTranslatef(-SF*it->pos.x, -SF*it->pos.y, -SF*it->pos.z);
	}
   	glutSwapBuffers();
}

void Env::_update_friends(){
	vector<Bird>::iterator i1;
	vector<Bird>::iterator i2;
	int i = 0;
	for(i1 = flock.begin(); i1!=flock.end(); i1++){
		i1->updateRmet();
		vector<Bird> frs;
		for(i2 = next(i1,1); i2!= flock.end(); i2++){
			vec3 <float> pvec = i1->pos-i2->pos; 
			float dist = pvec.mag();
			if(dist<i1->rmet)
				frs.push_back(*i2);
		}
		//cout<<"check-->"<<frs.size()<<endl;
		i1->setFriends(frs);
	}
}

void Env::runTimeStep(){
	_update_friends();
	vector<Bird>::iterator it;
	for(it=flock.begin(); it!=flock.end(); it++){
		it->calcNetForce();
		it->updateSpeedNPos();
	}
}