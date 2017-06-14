#include "drug.h"

void Drug::change(mat4& matrix) {
	int index = rand() % 16;
	double value = (1.0 - (rand() % 1000000 / 1000000.0) * 2) / 1000000.0 * strength;
	matrix.elements[index] += (float) value;
}
