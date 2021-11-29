#include "implementation1.h"
#include "implementation2.h"
#include "stdio.h"
#include "dlfcn.h"

int main(){
	int command, curLib = 0;
	char* libs[] = {"lib1.so", "lib2.so"};
	void* lib_handler = dlopen(libs[curLib], RTLD_LAZY); // initial library is first (with implementation1)
	if (lib_handler == NULL){
		return 1;
	}
	float (*Derivative)(float A, float deltaX);
	float (*Pi)(int k);
	Derivative = dlsym(lib_handler, "Derivative");
	Pi = dlsym(lib_handler, "Pi");
	while(scanf("%d", &command) != EOF){
		if (command == 0){
			dlclose(lib_handler);
			curLib = 1 - curLib;
			lib_handler = dlopen(libs[curLib], RTLD_LAZY);
			if (lib_handler == NULL){
				return 1;
			}
			Derivative = dlsym(lib_handler, "Derivative");
			Pi = dlsym(lib_handler, "Pi");
		}
		else if (command == 1){
			float A, deltaX;
			scanf("%f %f", &A, &deltaX);
			printf("cos'(%f) = %f\n", A, Derivative(A, deltaX));
		}
		else{
			int k;
			scanf("%d", &k);
			printf("pi = %f\n", Pi(k));
		}
	}

	return 0;
}