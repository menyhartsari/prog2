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
       	printf("Matrix inic. rows: %d cols: %d 			%f\n", matrix->rows, matrix->cols, ELEM(matrix,0,0));
       	return matrix;
 	  }
 	  /*
 	MAT *mat_create_by_file(char *filename){
 	  	int f;
        MAT *mat;
        unsigned int col, row;                      
        int i,j;
        float u;
        if( (f = open(filename, O_BINARY | O_RDONLY)) < 0 )
		{
			fprintf(stderr, "File access problem.\n");
			exit(1);
		}
      	lseek(f,2*sizeof(char),SEEK_SET);
        read(f,&row,sizeof(unsigned int));
        printf("rows: %d ", row);
        read(f,&col,sizeof(unsigned int));
        printf("cols: %d\n", col);
 		mat = mat_create_with_type(row, col);
 		printf("\nrow:%d, col:%d, value:%f\n", mat->rows,mat->cols,ELEM(mat,0,0));
 	 	for(i = 0; i < row; i++){
            for(j = 0; j < col; j++){      
                read(f,&u,sizeof(float));
            	ELEM(mat,i,j) = u;
      	    	printf("i: %d j: %d val: %f\n",i,j,ELEM(mat,i,j));                	 
        	} 
    	} /*
    	for ( i = 0; i < mat->rows; i++) {
            for ( j = 0; j < mat->cols; j++) {
                printf(" %f", ELEM(mat,i,j));
            }
            printf("\n");
        }
    	
    	if(close(f) == EOF){
        printf("Unable to close file\n");
        
    }
        
        printf("Matrix naèit.\n");
        return mat;
    }*/
    
 	char mat_save(MAT *mat, char *filename){
     	int f,i,j;
     	char kod[2]={'M','1'};
    	if( (f = open(filename, O_BINARY | O_WRONLY)) < 0 ){
			fprintf(stderr, "File access problem.\n");
			exit(1);
		}
		write(f,kod,2*sizeof(char));
		write(f, &mat->rows, sizeof(unsigned int));
		write(f, &mat->rows, sizeof(unsigned int));
		for (i=0; i<mat->rows;i++){
			for (j=0;j<mat->cols;j++)
				write(f, &ELEM(mat,i,j), sizeof(float));
		}
    	if(close(f) == EOF){
        printf("Unable to close file\n");
        return -1;
   		}
    }
    
    void mat_destroy(MAT *mat){
        free(mat);
    }
    
    void mat_unit(MAT *mat){
        int i, j;
        for ( i = 0; i < mat->rows; i++){
            for ( j = 0; j < mat->cols; j++){
                if (i==j)
                    ELEM(mat,i,j)=1; 
				else
                    ELEM(mat,i,j)=0;
            }
        }
    }
    
    void mat_random(MAT *mat){
        int i, j;
        srand((unsigned int) time(NULL));
        for ( i = 0; i < mat->rows; i++){
            for ( j = 0; j < mat->cols; j++){
                ELEM(mat,i,j)=-1+rand()%3;
            }
        }
    }
    
    void mat_print(MAT *mat){
        int i, j;
        printf("Matrix rows: %d cols: %d \n", mat->rows, mat->cols);
        for ( i = 0; i < mat->rows; i++) {
            for ( j = 0; j < mat->cols; j++) {
                printf(" %f", ELEM(mat,i,j));
            }
            printf("\n");
        }
    }
    
    unsigned int mat_rank(MAT *mat){
		int rank, col, row, i, bin, justnull;
		double x;
		rank = mat->cols;
		for (row = 0; row < rank; row++){ 
        	if (ELEM(mat,row,col) != 0){ 
           		for (col = 0; col < mat->rows; col++){ 
               		if (col != row){ 
                 		x = (double)(ELEM(mat,row,col) / ELEM(mat,row,row)); 
                 		for (i = 0; i < rank; i++) 
				   			ELEM(mat,col,i) = ELEM(mat,col,i) - x * ELEM(mat,row,i); 
              		} 
           		} 
   			}
			else{
        		for (i = row + 1; i < mat->rows;  i++){ 
                	if (ELEM(mat, i, row) == 0){ 
                    	for (i = 0; i < col; i++){ 
       						bin = ELEM(mat,row,i); 
     						ELEM(mat,row,i) = ELEM(mat,row+1,i); 
     						ELEM(mat,row+1,i) = bin; 
    					} 
                    	break ;
					} 
            	} 
        	}
			row--; 
		}
		return rank;
	}
	
 	int main(){
 	  	mat_create_with_type(2,2);
 	// 	MAT *A = mat_create_by_file("matrix.txt");
 	  	return 0;
	}
