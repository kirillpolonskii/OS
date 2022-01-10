#include "implementation2.h"
#include "stdio.h"
float Derivative(float A, float deltaX){
	printf("in 2st impl\n");
	return (cos(A + deltaX) - cos(A - deltaX)) / (2.0 * deltaX);
}

float Pi(int k){
	float product = 1.0;
	for(float i = 1.0; i <= k; ++i){
		product *= (4.0 * i * i) / (4.0 * i * i - 1);
	}
	return product * 2.0;
}