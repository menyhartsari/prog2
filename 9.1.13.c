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
	write(f, &mat->cols, sizeof(unsigned int));
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

void mat_swap_rows(MAT *m, unsigned int i, unsigned int j)
{
    unsigned int k;
    float aux;

    for(k=0; k<m->cols; k++)
    {
        aux = ELEM(m,i,k);
        ELEM(m,i,k) = ELEM(m,j,k);
        ELEM(m,j,k) = aux;
    }
}

void mat_admul_rows(MAT *m, unsigned int i, float mul, unsigned int j)
{
    unsigned int k;

    for(k=0; k<m->cols; k++)
        ELEM(m,j,k) += mul*ELEM(m,i,k);
}

unsigned int mat_rank(MAT *mat){
    int rank, i, j, k, justnull, row;
    float x;
    
    row = 0;
    for (i = 0; i < mat->cols; i++){
        for (j = row; j < mat->rows; j++){
            if (ELEM(mat,j,i) != 0){
                mat_swap_rows(mat,j,row);
                row++;
            }
        }
    }
    for (i = 0; i < mat->rows; i++){
        for (j = 0; j < mat->cols; j++){
            if (ELEM(mat,i,j) != 0){
                for (k = 0; k < mat->cols; k++)
                    ELEM(mat,i,k) = ELEM(mat,i,k)/ELEM(mat,i,j);
            	for (k = 0; k < mat->rows; k++){
                    if (k != i && ELEM(mat,k,j) != 0){
                        x = ELEM(mat,k,j);
						mat_admul_rows(mat,i,-x,k);
                    }
                }
            	break;
            }
        } 
    }
    rank = 0;
    for (i = 0; i < mat->rows; i++){
        justnull = 0;
        for (j = 0; j < mat->cols; j++){
            if (ELEM(mat,i,j) != 0)
                justnull = 1;
	    	else
            	ELEM(mat,i,j) = 0;            
        }
        rank+=justnull;
    }    
    return rank;
}

int main(){
 	MAT *A = mat_create_by_file("matrix.txt");
    MAT *B = mat_create_with_type(3,3);
    MAT *C = mat_create_with_type(3,4);
    MAT *D = mat_create_with_type(5,3);
    unsigned int rank;
	
	rank = mat_rank(A);
	printf("Hodnost matice A je : %d\n", rank);

    ELEM(B,0,0) = 1;
    ELEM(B,0,1) = 1;
    ELEM(B,0,2) = 1;
    ELEM(B,1,0) = 1;
    ELEM(B,1,1) = 1;
    ELEM(B,1,2) = 1;
    ELEM(B,2,0) = 1;
    ELEM(B,2,1) = 1;
    ELEM(B,2,2) = 1;

    rank = mat_rank(B);
	printf("Hodnost matice B je : %d\n", rank);

    ELEM(C,0,0) = 1;
    ELEM(C,0,1) = 1;
    ELEM(C,0,2) = 1;
    ELEM(C,0,3) = 1;
    ELEM(C,1,0) = 1;
    ELEM(C,1,1) = 1;
    ELEM(C,1,2) = 1;
    ELEM(C,1,3) = 0;
    ELEM(C,2,0) = 1;
    ELEM(C,2,1) = 1;
    ELEM(C,2,2) = 1;
    ELEM(C,2,3) = 1;

    rank = mat_rank(C);
	printf("Hodnost matice C je : %d\n", rank);
	
	ELEM(D,0,0) = 1;
    ELEM(D,0,1) = 1;
    ELEM(D,0,2) = 1;
    ELEM(D,1,0) = 1;
    ELEM(D,1,1) = 0;
    ELEM(D,1,2) = 1;
    ELEM(D,2,0) = 1;
    ELEM(D,2,1) = 4;
    ELEM(D,2,2) = 1;
    ELEM(D,3,0) = 3;
    ELEM(D,3,1) = 1;
    ELEM(D,3,2) = 0;
    ELEM(D,4,0) = 1;
    ELEM(D,4,1) = 1;
    ELEM(D,4,2) = 1;

	rank = mat_rank(D);
	printf("Hodnost matice D je : %d\n", rank);
	return 0;
}
