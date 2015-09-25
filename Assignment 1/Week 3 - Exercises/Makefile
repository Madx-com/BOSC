all: testFB redirect testIn testOut testPipe testInOut

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

testInOut: testInOut.o
	gcc -o testInOut testInOut.o
testInOut.o: testInOut.c
	gcc -c testInOut.c

testOut: testOut.o
	 gcc -o testOut testOut.o
testOut.o: testOut.c redirect.h
	   gcc -c testOut.c

testPipe: testpipe.o pipe.o
	  gcc -o testPipe pipe.o testpipe.o
pipe.o: pipe.c pipe.h
	gcc -c pipe.c
testpipe.o: testpipe.c
	    gcc -c testpipe.c
