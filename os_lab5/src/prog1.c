#include "implementation2.h"
#include "implementation1.h"

#include "stdio.h"

int main(){
	int command;
	while(scanf("%d ", &command) != EOF){
		if (command == 1){
			float A, deltaX;
			scanf("%f %f", &A, &deltaX);
			printf("cos'(%f) = %f\n", A, Derivative(A, deltaX));
		}
		else {
			int k;
			scanf("%d", &k);
			printf("pi = %f\n", Pi(k));
		}
	}
	return 0;
}