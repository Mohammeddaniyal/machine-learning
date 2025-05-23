rm *.o
rm ../lib/libml-framework.a

gcc -c ../src/matrix/*.c -I ../include -Wall
gcc -c ../src/vector/*.c -I ../include -Wall
gcc -c ../src/operations/*.c -I ../include -Wall
gcc -c ../src/set/*.c -I ../include -Wall
gcc -c ../src/encoder/*.c -I ../include -Wall
gcc -c ../src/scaling/*.c -I ../include -Wall
ar rcs ../lib/libml-framework.a *.o
