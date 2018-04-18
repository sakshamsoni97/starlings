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
int Bird::count;
vec3 <float> Bird::roost;

Bird::Bird(vec3 <float> p){
	pos = p;
	dir.x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2 -1;
	dir.z = 0.0;
	dir = dir/dir.mag();
	rmet = rmax;
	serial = count;
	count++;
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
	serial = count;
	count++;
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
	count = 0;
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
	vec3 <float> temp = pos;
	temp.z = 0.0;
	int s = -sgn(frh*n);
	frh = frh*((float) s)*WRH*(0.5+(dir*n)*0.5)*temp.mag();
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


void Bird::_update_friends(){
	vector<Bird>::iterator i1;
	updateRmet();
	if(!friends.empty())
		friends.clear();
	for(i1 = flock.begin(); i1!=flock.end(); i1++){
		vector<Bird> frs;
		if(serial==i1->serial)
			continue;
		vec3 <float> pvec = pos-i1->pos; 
		float dist = pvec.mag();
		if(dist<rmet)
			friends.push_back(*i1);
	}
}


void Bird::calcNetForce(){
	_update_friends();
	vec3 <float> Fsep = _separation();
	vec3 <float> Fcoh = _cohesion();
	vec3 <float> Frst = _attraction_to_roost();
	vec3 <float> Fno = _generate_noise();
	vec3 <float> Fal = _allignment();
	vec3 <float> Ft = dir*100*mass*(v0 - vc);
	vec3 <float> Fsocial = Fsep + Fcoh + Frst + Fno + Fal;
	//vec3 <float> Fsocial = _separation() + _cohesion() + _attraction_to_roost() + _generate_noise() + _allignment();
	float vsq_ratio = (vc*vc)/(v0*v0);
	float temp = mass*G*(1-vsq_ratio); 

	vec3 <float> Fflight((CDCL)*temp, -temp, 0.0);

	net_force = Fsocial+Fflight+Ft;
	if(!isfinite(net_force.mag())){
		cout<<simulation_time<<" : net force value exploded "<<endl;
	}
}

void Bird::updateSpeedNPos(){
	vec3 <float> velocity = dir*vc;
	velocity = velocity + (net_force/mass)*DT;

	pos = pos + velocity*DT;

	vc = velocity.mag();
	if(!isfinite(pos.mag()))
		cout<<simulation_time<<" : "<<net_force.mag()<<endl;
	if(vc!=0.0)
		dir = velocity/vc;
	simulation_time += DT;
}

vector<Bird> flock;
float simulation_time;

void Env::_create_flock(int Num){
	if(!flock.empty())
		flock.clear();
	for(int i =0; i<Num; i++){
		Bird b;
		flock.push_back(b);
	}
}

Env::Env(vec3 <float> rst, int Num ,float rm, float rsp, float m, float v){
	Bird::Init(rm,rsp,m,v,rst);
	_create_flock(Num);
	simulation_time = 0;
}

void DrawCircle(float cx, float cy, float cz, float r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex3f(x + cx, y + cy, cz);//output vertex

    }
    glEnd();
}


void Env::display(){
	runTimeStep();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(0.0f, 0.0f, -10.0f);  
 
  /* glBegin(GL_LINE_LOOP);     
   	  glLineWidth(6);
      glColor3f(1.0f, 0.0f, 0.0f);  
      glVertex3f( 10.0f, 10.0f, 0.0f);
      glVertex3f( 10.0f, -10.0f, 0.0f);
      glVertex3f(-10.0f, -10.0f, 0.0f);
      glVertex3f(-10.0f, 10.0f, 0.0f);
    glEnd();*/  

	vector<Bird>::iterator it;
	for(it = flock.begin(); it!=flock.end(); it++){
		glTranslatef((GLfloat)it->pos.x, (GLfloat)it->pos.y, (GLfloat)it->pos.z);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);     
		vec3 <float> p1 = it->dir;
		vec3 <float> p2(-it->dir.y, it->dir.x, it->pos.z);
		vec3 <float> p3(it->dir.y, -it->dir.x, it->pos.z);
		glVertex3f((GLfloat)p1.x, (GLfloat)p1.y, (GLfloat)p1.z);
		glVertex3f((GLfloat)p2.x, (GLfloat)p2.y, (GLfloat)p2.z);
		glVertex3f((GLfloat)p3.x, (GLfloat)p3.y, (GLfloat)p3.z);
		//DrawCircle(0.0f, 0.0f, 0.0f, it->rmet, 50);

		glEnd();
		glTranslatef((GLfloat)-it->pos.x, (GLfloat)-it->pos.y, (GLfloat)-it->pos.z);
	}
   	glutSwapBuffers();
}

void Env::runTimeStep(){
	//_update_friends();
	vector<Bird>::iterator it;
	for(it=flock.begin(); it!=flock.end(); it++){
		it->calcNetForce();
		it->updateSpeedNPos();
	}
}