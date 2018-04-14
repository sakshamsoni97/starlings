#include <math.h>
#include <vector>
#include <string>
#include <stdlib.h>

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
};


class Bird{
	vec3<float> pos; // position of the bird
	vec3<float> dir; // direction of heading
	float v0; // magnitude of SS speed
	float vc; // magnitude of current speed
	vector<Bird> *friends;

protected:
	vec3<float> _cohesion();
	vec3<float> _allignment();
	vec3<float> _separation();

public:
	Bird(vec3<float> p);
	Bird();

	void getFriends(vector<Bird> *new_friends;);

	void updateSpeed();

	void updatePos();
};

class Env{
	vector<Bird> flock;
	float range;

public:
	Env();

	void updateFriends();

	void display();

	void runTimeStep();
};