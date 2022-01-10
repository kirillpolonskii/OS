#include "implementation1.h"
#include "stdio.h"
float Derivative(float A, float deltaX){
	printf("in 1st impl\n");
	return (cos(A + deltaX) - cos(A)) / deltaX;
}

float Pi(int k){
	float sum = 0.0;
	for(float i = 0.0; i <= k; ++i){
		sum += pow(-1, i) / (2.0 * i + 1);
	}
	return sum * 4.0;
}