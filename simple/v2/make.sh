gcc -fPIC -o myname.o -c myname.c
gcc -shared -Wl,--version-script=myname.v2 -o libmyname.so.2 myname.o
ln -s -f libmyname.so.2 libmyname.so

gcc         -o simple_v1  ../simple.o -lmyname -L..
gcc         -o simple_v2  ../simple.o -lmyname -L.

echo "=> Run ./simple_v1 with libmyname.so without version (Implying v1)"
LD_LIBRARY_PATH=../ ./simple_v1

echo ""
echo "=> Run ./simple_v2 with libmyname.so without version (Implying v1)"
LD_LIBRARY_PATH=../ ./simple_v2

echo ""
echo "=> Run ./simple_v1 with libmyname.so.2"
LD_LIBRARY_PATH=./ ./simple_v1

echo ""
echo "=> Run ./simple_v2 with libmyname.so.2"
LD_LIBRARY_PATH=./ ./simple_v2