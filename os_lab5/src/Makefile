files: prog1_1 prog1_2 prog2

prog1_1: lib1.so prog1.c
	gcc prog1.c -o prog1_1 -L. -l1 -Wl,-rpath,.

prog1_2: lib2.so prog1.c
	gcc prog1.c -o prog1_2 -L. -l2 -Wl,-rpath,.

prog2: prog2.c lib1.so lib2.so
	gcc prog2.c -o prog2 -L. -ldl -Wl,-rpath,.

lib1.so: implementation1.o
	gcc -shared implementation1.o -o lib1.so

lib2.so: implementation2.o
	gcc -shared implementation2.o -o lib2.so

implementation1.o: implementation1.c
	gcc -c -fPIC implementation1.c -o implementation1.o -lm

implementation2.o: implementation2.c
	gcc -c -fPIC implementation2.c -o implementation2.o -lm

clean:
	rm -f *.o *.so prog1_1 prog1_2 prog2