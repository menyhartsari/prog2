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

#ifndef O_BINARY
#define O_BINARY 0
#endif

typedef struct{
    unsigned int rows;
    unsigned int cols;
    float *elem;
}MAT;

#define ELEM(M,r,c) (M->elem[(M->cols)*r+c])

void mat_destroy(MAT *mat){
	free(mat->elem);
    free(mat);
}

MAT *mat_create_with_type(unsigned int rows, unsigned int cols){
    MAT *m;
	
	m=(MAT*)malloc(sizeof(MAT));
	if (m==0){
		mat_destroy(m);
		return NULL;
	}
    m->rows = rows;
    m->cols = cols;
    m->elem = (float*)malloc(sizeof(float)*rows*cols);
	if (m->elem==0){			
		mat_destroy(m);
		return NULL;
	}
	return m;
}

MAT *mat_create_by_file(char *filename){
	int f;
    MAT *mat;
    unsigned int col, row;                      
    int i,j;
    float u;
    char kod[2];
    
    if( (f = open(filename, O_BINARY | O_RDONLY)) < 0 ){
		return NULL;
	}
	read(f,&kod,2*sizeof(char));
	if( (kod[0]!='M')||(kod[1]!='1')){
		close(f);
		return NULL;
	}
    read(f,&row,sizeof(unsigned int));
    if( row < 0 ){
    	close(f);
    	return NULL;
	}
    read(f,&col,sizeof(unsigned int));
    if( col < 0 ){
    	close(f);
    	return NULL;
	}
	mat = mat_create_with_type(row, col);
	if (mat==0){
    	close(f);
		return NULL;		
	}
	for (i = 0; i < mat->rows; i++){
        for (j = 0; j < mat->cols; j++){      
            if (read(f,&u,sizeof(float)) == -1){
            	mat_destroy(mat);
    			close(f);
				return NULL;
			}
			else
				ELEM(mat,i,j) = u;
			}	     	 
    	}
	close(f);
    return mat;
}

char mat_save(MAT *mat, char *filename){
 	int f,i,j;
 	char kod[2]={'M','1'};

	if( (f = open(filename, O_BINARY | O_WRONLY | O_CREAT)) < 0 )
		return -1;
	if (mat==0){
    	mat_destroy(mat);
    	close(f);
		return -1;		
	}
	write(f,kod,2*sizeof(char));
	write(f, &mat->rows, sizeof(unsigned int));
	write(f, &mat->rows, sizeof(unsigned int));
	for (i = 0; i < mat->rows; i++){
		for (j = 0; j < mat->cols; j++)
			write(f, &ELEM(mat,i,j), sizeof(float));
	}
	close(f);
	return 0;
}

void mat_unit(MAT *mat){
    int i, j;

    for (i = 0; i < mat->rows; i++){
        for (j = 0; j < mat->cols; j++){
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
    for (i = 0; i < mat->rows; i++){
        for (j = 0; j < mat->cols; j++){
            ELEM(mat,i,j)=-1+rand()%3;
        }
    }
}

void mat_print(MAT *mat){
    int i, j;

    printf("Matrix rows: %d cols: %d \n", mat->rows, mat->cols);
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->cols; j++) {
            printf(" %f	", ELEM(mat,i,j));
        }
        printf("\n");
    }
}

unsigned int mat_rank(MAT *mat){
    int rank, col, row, i, y, bin, justnull;
    float x;

    rank = mat->cols;
	for (row = 0; row < rank; row++){    
    	if (ELEM(mat,row,row) != 0){           
			for (col = 0; col < mat->rows; col++){ 
           		if (col != row){
				   	x = (ELEM(mat,col,row) / ELEM(mat,row,row));
            		for (i = 0; i < rank; i++) 
               			ELEM(mat,col,i) -= x * ELEM(mat,row,i); 
          		} 
       		} 
    	}
		else{
        	justnull=0;
    		for (i = row + 1; i < mat->rows;  i++){         
				if (ELEM(mat, i, row) == 0){
					y=i;
             		for (i = 0; i < rank; i++){
                    	bin = ELEM(mat,row,i); 
                   		ELEM(mat,row,i) = ELEM(mat,y,i); 
                    	ELEM(mat,y,i) = bin; 
                	}
                	i=y;
                	justnull=1;
                	break; 
            	} 
        	} 
    		if (justnull=0){  
            	rank--; 
            	for (i = 0; i < mat->rows; i ++) 
                	ELEM(mat,i,row) = ELEM(mat,i,rank);
        	} 
        	row--;
    	} 
    }

    return rank;
}

int main(){
 	MAT *A = mat_create_by_file("matrix.txt");
    MAT *B = mat_create_with_type(3,3);
    unsigned int rank;

    ELEM(B,0,0) = 1;
    ELEM(B,0,1) = 1;
    ELEM(B,0,2) = 1;
    ELEM(B,1,0) = 1;
    ELEM(B,1,1) = 1;
    ELEM(B,1,2) = 1;
    ELEM(B,2,0) = 1;
    ELEM(B,2,1) = 1;
    ELEM(B,2,2) = 1;

    rank = mat_rank(A);
 	printf("Hodnost matice A je : %d", rank);

    rank = mat_rank(B);
 	printf("Hodnost matice B je : %d", rank);

  	return 0;
}
