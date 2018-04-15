#ifndef __BIRDS_H
#define __BIRDS_H

#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <GL/freeglut.h>
#define Nc 7.0
#define DT 0.005
#define S 0.005
#define WS 1.0
#define WC 1.0
#define WA 0.5
#define WRV 0.2
#define WRH 0.01
#define SIGMA_SQ 1.8769
#define CDCL 0.3 
#define G 9.87


using namespace std;

struct vec3<T>{
	T x;
	T y;
	T z;
	vec3(T _x, T _y, T _z){
		x = _x;
		y = _y;
		z = _z; 
	}
	vec3<T> operator+(const vec3<T> &v);
	vec3<T> operator-(const vec3<T> &v);
	vec3<T> operator/(const float &A);
	vec3<T> operator*(const float &A);
	T operator*(const vec3<T> &v);
	float vec3::mag();
};


class Bird{
	vec3<float> pos; // position of the bird
	vec3<float> dir; // direction of heading
	vec3<float> net_force; // net force acting on the bird
	float vc; // magnitude of current speed
	float rmet;
	vector<Bird> *friends;
	
	static float rsep;
	static float v0; // magnitude of SS speed
	static float mass;
	static float rmax;
	static vec3<float> roost;

protected:
	vec3<float> _separation();
	vec3<float> _cohesion();
	vec3<float> _allignment();
	vec3<float> _attraction_to_roost();

public:
	Bird(vec3<float> p);
	Bird();

	static void set_rmax(float rm); 
	static void set_roost(vec3<float> rs);
	static void set_mass(float m);
	static void set_v0(float v);
	static void init(float rm, float rsp, float m, float v, vec3<float> rst);

	int getNumFriends();

	void setFriends(vector<Bird> *new_friends);

	void calcNetForce();

	void updateSpeedNPos();
};

extern vector<Bird> flock;

class Env{
	/* class to providing controls over the environment */
	float range;
	float Rmax;
protected:
	static void _update_friends();
	void _create_flock(int Num);

public:
	Env(float rm = 50, float rsp = 0.3, float m = 0.8, float v = 1.0, vec3<float> rst = vec3<float>(0.0, 0.0, 0.0), int Num = 20);

	static void display();

	static void runTimeStep();
};

#endif