#ifndef __BIRDS_H
#define __BIRDS_H

#include <math.h>
#include <vector>
#include <stdlib.h>
#include <GL/freeglut.h>
#define Nc 7.0
#define DT 0.01
#define DTmsec 10
#define S 0.005
#define WS 7.0
#define WC 4.0
#define WA 2.0 	
#define WRV 1.0
#define WRVD 2.5
#define WRH 0.05
#define SIGMA_SQ 1.21
#define CDCL 0.3 
#define G 9.87

using namespace std;

template <class T>
class vec3{
public:
	T x;
	T y;
	T z;
	vec3(T _x, T _y, T _z);
	vec3();
	vec3 <T> operator+(const vec3 <T> &v);
	vec3 <T> operator-(const vec3 <T> &v);
	vec3 <T> operator/(const float &A);
	vec3 <T> operator*(const float &A);
	T operator*(const vec3 <T> &v);
	float mag();
};


class Bird{
	vec3 <float> net_force; // net force acting on the bird
	float vc; // magnitude of current speed
	vector<Bird *> friends;
	static float rsep;
	static float v0; // magnitude of SS speed
	static float mass;
	static float rmax;
	static vec3 <float> roost;
	
protected:
	vec3 <float> _separation();
	vec3 <float> _cohesion();
	vec3 <float> _allignment();
	vec3 <float> _interactions();
	vec3 <float> _attraction_to_roost();
	vec3 <float> _generate_noise();
	void _update_friends();

public:
	int serial;
	vec3 <float> pos; // position of the bird
	vec3 <float> dir; // direction of heading
	float rmet; // the metric range of interaction
	Bird(vec3 <float> p);
	Bird();

	static void set_rmax(float rm); 
	static void set_roost(vec3 <float> rs);
	static void set_mass(float m);
	static void set_v0(float v);
	static void Init(float rm, float rsp, float m, float v, vec3 <float> rst);
	static int count;

	int getNumFriends();

	void updateRmet();

	void calcNetForce();

	void updateSpeedNPos();
};

extern vector<Bird> flock;
extern int simulation_time;

class Env{
	/* class to providing controls over the environment */
	float range;
	float Rmax;
	static float mass;
protected:
	//static void _update_friends();
	void _create_flock(int Num);

public:
	Env(vec3 <float> rst, int Num = 1000, float rm = 45, float rsp = 0.7, float m = 0.8, float v = 5.0);

	static void display();

	static void runTimeStep();
};

#endif