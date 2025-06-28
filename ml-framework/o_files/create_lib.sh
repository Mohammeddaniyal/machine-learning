rm *.o
rm ../lib/libml-framework.a

gcc -O3 -c ../src/matrix/*.c -I ../include -Wall
gcc -O3 -c ../src/vector/*.c -I ../include -Wall
gcc -O3 -c ../src/operations/*.c -I ../include -Wall
gcc -O3 -c ../src/set/*.c -I ../include -Wall
gcc -O3 -c ../src/encoder/*.c -I ../include -Wall
gcc -O3 -c ../src/scaling/*.c -I ../include -Wall
gcc -O3 -c ../src/utils/*.c -I ../include -Wall
gcc -O3 -c ../src/learning/*.c  -I ../include -Wall
gcc -O3 -c ../src/math/*.c  -I ../include -Wall
ar rcs ../lib/libml-framework.a *.o
