#pragma once
#include<iostream>
#include<cstdlib>
#include <cmath>
#include<stack>
#include<ctime>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "src\maths\maths.h"

#define CAFFEINE_ID 1
#define SUCROSE_ID 2
#define ALCOHOL_ID 3
#define COCAINE_ID 4

using namespace troyberry;
using namespace maths;

class Drug {
private:
	double speedModifier, strength;
	int id;
public:
	Drug(double speedModifier, double strength, int id) : speedModifier(speedModifier), strength(strength), id(id) {}
	virtual ~Drug() {}
	inline double getSpeed() { return pow(speedModifier, strength); }
	inline double getStrength() { return strength; }
	inline int getId() { return id; }
	void change(mat4& matrix);

};
