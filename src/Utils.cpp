#include "Utils.h"
#include <math.h>
#include <cstdlib>
#include <random>

int Utils::random(int from, int to)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(from, to);
	return dis(gen);
}
