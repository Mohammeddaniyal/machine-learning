#after moving the framework oriented functions to appropriate framework folders
#remove the -I ../../../ml-framework/private/include

gcc $1.c -I ../../../../ml-framework/include -I ../../../../ml-framework/private/include -L ../../../../ml-framework/lib -l ml-framework -lm -o ../build/$1 -Wall
