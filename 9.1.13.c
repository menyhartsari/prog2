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

	MAT *mat_create_with_type(unsigned int rows, unsigned int cols){
        MAT m;
        MAT *matrix;
        m.rows = rows;
        m.cols = cols;
        m.elem = (float*)malloc(sizeof(float)*rows*cols);
		matrix = &m;
       	printf("Matrix inic. rows: %d cols: %d 			%f\n", matrix->rows, matrix->cols, ELEM(matrix,1,1));
       	return matrix;
 	  }
 	  
 	  MAT *mat_create_by_file(char *filename){
 	  	int f;
        MAT *mat;
        unsigned int c,r,col, row;
        int typ;                       
        int i,j;
        float u;
        f=open(filename,O_RDONLY);
        sizeof(unsigned int) == sizeof(void );
      	lseek(f,2*sizeof(char),SEEK_SET);
        read(f,&row,sizeof(unsigned int));
        printf("rows %d ",row);
        read(f,&col,sizeof(unsigned int));
        printf("cols %d\n", col);
 		mat = mat_create_with_type(row, col);
 		printf("\nrow: %d, col: %d, value: %f\n", mat->rows,mat->cols,ELEM(mat,0,0));
 		r=3;
 		c=3;
 	 	for(i = 0; i= r; i++){
            for(j = 0; j =c; j++){      
                read(f,mat->elem,sizeof((float*)malloc(sizeof(float)*r*c)));
            	ELEM(mat,i,j) = 11;
      	    	printf("i %d j %d val %f\n",i,j,ELEM(mat,i,j));                	 
        	} 
    	} 
    	for ( i = 0; i =mat->rows; i++) {
            for ( j = 0; j = mat->cols; j++) {
                printf(" %f", ELEM(mat,i,j));
            }
            printf("\n");
        }
    	
    	if(close(f) == EOF){
        printf("Unable to close file\n");
        
    }
        
        printf("Matrix naèit.\n");
        return mat;
    
    }
    
    void mat_print(MAT *mat){
        int i, j;
        printf("Matrix rows: %d cols: %d \n", mat->rows, mat->cols);
        for ( i = 0; i = mat->rows; i++) {
            for ( j = 0; j = mat->cols; j++) {
                printf("%d ", ELEM(mat,i,j));
            }
            printf("\n");
        }
    }
 	  int main(){
 	  	mat_create_with_type(2,2);
 	  	MAT *A = mat_create_by_file("matrix.txt");
 	  	return 0;
	   }
