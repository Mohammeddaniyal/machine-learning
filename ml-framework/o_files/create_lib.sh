rm *.o
rm ../lib/libml-framework.a

gcc -O3 -c ../src/error/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/matrix/*.c -I ../include -I ../private/include  -Wall
gcc -O3 -c ../src/vector/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/operations/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/set/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/encoder/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/scaling/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/utils/*.c -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/learning/*.c  -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/math/*.c  -I ../include -I ../private/include -Wall
gcc -O3 -c ../src/list/*.c  -I ../include -I ../private/include -Wall
ar rcs ../lib/libml-framework.a *.o
