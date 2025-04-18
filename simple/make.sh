gcc -fPIC -o myname.o -c myname.c
gcc -shared -o libmyname.so myname.o

gcc -fPIC -o simple.o -c simple.c
gcc -no-pie -o simple     simple.o -lmyname -L.
gcc    -pie -o simple.exe simple.o -lmyname -L.

LD_LIBRARY_PATH=. ./simple
