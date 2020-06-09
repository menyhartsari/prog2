#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct{
        unsigned int rows;
        unsigned int cols;
        float *elem;
    }MAT;
    
	#define ELEM(M,r,c) (M->elem[(M->cols)*r+c])
