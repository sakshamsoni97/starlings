#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>
#define Nc 7.0
#define S
#define WS
#define WC
#define WA
#define WRV
#define WRH
#define SIGMA_SQ


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
	float v0; // magnitude of SS speed
	float vc; // magnitude of current speed
	float rsep;
	float rmet;
	vector<Bird> *friends;

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

	void getFriends(vector<Bird> *new_friends);

	void calcNetForce();

	void updateSpeed();

	void updatePos();
};

class Env{
	vector<Bird> flock;
	float range;
	float Rmax;

public:
	Env();

	void updateFriends();

	void display();

	void runTimeStep();
};