all: testFB redirect testIn testOut

testFB: forback.o testForAndBackground.o
	gcc -o testFB forback.o testForAndBackground.o
forback.o: forback.c forback.h
	gcc -c forback.c
testForAndBackground.o: testForAndBackground.c
	gcc -c testForAndBackground.c


redirect: redirect.o
	gcc -o redirect redirect.o
redirect.o: redirect.c redirect.h
	gcc -c redirect.c

testIn: testIn.o
	gcc -o testIn testIn.o
testIn.o: testIn.c redirect.h
	gcc -c testIn.c

testOut: testOut.o
	 gcc -o testOut testOut.o
testOut.o: testOut.c redirect.h
	   gcc -c testOut.c
