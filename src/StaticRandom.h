#ifndef NAIVE_RAYTRACER_STATICRANDOM_H
#define NAIVE_RAYTRACER_STATICRANDOM_H

#include <random>

class StaticRandom {
public:
	StaticRandom();

	std::mt19937 rand;
};


#endif //NAIVE_RAYTRACER_STATICRANDOM_H
