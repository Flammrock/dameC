/*********************************************************************************\
* Copyright (c) 2020 - Flammrock                                                  *
*                                                                                 *
* Permission is hereby granted, free of charge, to any person obtaining a copy    *
* of this software and associated documentation files (the "Software"), to deal   *
* in the Software without restriction, including without limitation the rights    *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
* copies of the Software, and to permit persons to whom the Software is           *
* furnished to do so, subject to the following conditions:                        *
*                                                                                 *
* The above copyright notice and this permission notice shall be included in all  *
* copies or substantial portions of the Software.                                 *
*                                                                                 *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
* SOFTWARE.                                                                       *
\*********************************************************************************/

#ifndef CHECKERS_MATRIX_H
#define CHECKERS_MATRIX_H


/**
 * matrix.h
 *
 * Ce fichier contient les fonctions permettant de créer des matrices et
 * des fonctions permettant de faire des opérations sur les matrices
 */




/**********************\
*                      *
*       INCLUDE        *
*                      *
\**********************/
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>






/**********************\
*                      *
*        MACRO         *
*                      *
\**********************/
 
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#define _STR(arr) (char*)arr,LEN(arr)
#define _1D(arr) (int*)arr,LEN(arr)
#define _1Df(arr) (double*)arr,LEN(arr)
#define _2D(arr) (int*)arr,LEN(arr),LEN(arr[0])






/**********************\
*                      *
*       STRUCT         *
*                      *
\**********************/
 
// MATRIX
typedef struct matrix matrix;
struct matrix {
    int rows;
    int cols;
    double **data;
};








/**********************\
*                      *
*      FUNCTIONS       *
*                      *
\**********************/
 
int count_digit(int n) {
    if (n < 0) n*= -10;
    return (int)floor(log10(n))+1;
}
void matrix_free(matrix m) {
    int i;
    for (i = 0; i < m.rows; i++){  
        free(m.data[i]);
    }
    free(m.data);
}
matrix matrix_create(int rows, int cols) {
    
    int i,j;
    
    double **data = (double **)malloc(rows * sizeof(double *));
    if (data == NULL) bad_memory_allocation();
    for (i = 0; i < rows; i++) {
        data[i] = (double *)malloc(cols * sizeof(double));
        if (data[i] == NULL) bad_memory_allocation();
    }
  
 
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            data[i][j] = (double)0;
        }
    }
 
    matrix m = { rows, cols, data };
    
    return m;
}
matrix matrix_create_from_2Darray(double *arr, int rows, int cols) {
    
    int i,j;
    
    double **data = (double **)malloc(rows * sizeof(double *));
    if (data == NULL) bad_memory_allocation();
    for (i = 0; i < rows; i++) {
        data[i] = (double *)malloc(cols * sizeof(double));
        if (data[i] == NULL) bad_memory_allocation();
    }
  
 
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            data[i][j] = (double)*((arr+i*cols)+j);
        }
    }
 
    matrix m = { rows, cols, data };
    
    return m;
}
matrix matrix_create_from_array(double *arr, int cols) {
    
    int i,j;
    
    int rows = 1;
    
    double **data = (double **)malloc(rows * sizeof(double *));
    if (data == NULL) bad_memory_allocation();
    for (i = 0; i < rows; i++) {
        data[i] = (double *)malloc(cols * sizeof(double));
        if (data[i] == NULL) bad_memory_allocation();
    }
  
 
 
        for (j = 0; j < cols; j++) {
            data[0][j] = (double)(arr[j]);
        }
    
 
    matrix m = { rows, cols, data };
    
    return m;
}
double matrix_getmax(char t,matrix m) {
    int i,j;
    double max = m.data[0][0];
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            if (t == '+') {
                if (m.data[i][j] > max) {
                    max = m.data[i][j];
                }
            } else {
                if (m.data[i][j] < max) {
                    max = m.data[i][j];
                }
            }
        }
    }
    return max;
}
void matrix_random_fill(matrix m, double min, double max) {
    int i,j;
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            m.data[i][j] = ((double)rand()/(double)RAND_MAX) * (max - min) + min;
        }
    }
}
void matrix_fprint(matrix m, FILE *f) {
    int i,j;
    int dmax = count_digit((int)matrix_getmax('+',m));
    int dmin = count_digit((int)matrix_getmax('-',m));
    int d;
    if (dmax > dmin) {
        d = dmax;
    } else {
        d = dmin;
    }
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            int dt = count_digit(m.data[i][j]);
            for (int k = 0; k < d-dt; k++) {
                printf(" ");
            }
            fprintf(f,"%.*lf ", __precision, m.data[i][j] * __scale);
        }
        fprintf(f,"\n");
    }
}
void matrix_print(matrix m) {
    int i,j;
    int dmax = count_digit((int)matrix_getmax('+',m));
    int dmin = count_digit((int)matrix_getmax('-',m));
    int d;
    if (dmax > dmin) {
        d = dmax;
    } else {
        d = dmin;
    }
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            int dt = count_digit(m.data[i][j]);
            for (int k = 0; k < d-dt; k++) {
                printf(" ");
            }
            printf("%.*lf ", __precision, m.data[i][j] * __scale);
        }
        printf("\n");
    }
}
matrix matrix_tr(char tr, matrix m0) {
    int i,j;
    matrix m;
    if (tr == 't') {
        m = matrix_create(m0.cols, m0.rows);
    } else {
        m = matrix_create(m0.rows, m0.cols);
    }
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            if (tr == 't') {
                m.data[i][j] = m0.data[j][i];
            } else {
                m.data[i][j] = m0.data[i][j];
            }
        }
    }
    return m;
}
matrix matrix_map(matrix m0, double (*fun_ptr)(double)) {
    int i,j;
    matrix m = matrix_create(m0.rows, m0.cols);
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            m.data[i][j] = (*fun_ptr)(m0.data[i][j]);
        }
    }
    return m;
}
matrix matrix_op(char op, matrix m0, matrix m1) {
    int i,j;
    matrix m;
    if (op == '.') {
        if (m0.cols != m1.rows) {
            fprintf(stderr, "Err: Bad matrices dimensions dot\n");
            exit(1);
        }
        m = matrix_create(m0.rows, m1.cols);
    } else {
        if (m0.cols != m1.cols || m0.rows != m1.rows) {
            fprintf(stderr, "Err: Bad matrices dimensions '%c'\n",op);
            exit(1);
        }
        m = matrix_create(m0.rows, m0.cols);
    }
    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.cols; j++) {
            if (op == '+') {
               m.data[i][j] = m0.data[i][j] + m1.data[i][j]; 
            } else if (op == '-') {
                m.data[i][j] = m0.data[i][j] - m1.data[i][j]; 
            } else if (op == '*') {
                m.data[i][j] = m0.data[i][j] * m1.data[i][j]; 
            } else if (op == '.') {
                double sum = (double)0;
                for (int k = 0; k < m0.cols; k++) {
                    sum += m0.data[i][k] * m1.data[k][j];
                }
                m.data[i][j] = sum;
            }
            
        }
    }
    return m;
}
void matrix_print_inline(matrix m, FILE *f, int binary) {
    int i,j;
    if (f == NULL) {
        printf("%d-%d-",m.rows,m.cols);
    } else {
        if (binary == 1) {
            fwrite(&m.rows, sizeof(m.rows), 1, f);
            fwrite(&m.cols, sizeof(m.cols), 1, f);
        } else {
            fprintf(f,"%d-%d-",m.rows,m.cols);
        }
    }
    for (i = 0; i < m.rows; i++) {
 
            for (j = 0; j < m.cols; j++) {
                if (f == NULL) {
                    printf("%.17f\n", m.data[i][j]); 
                } else {
                    if (binary == 1) {
                        fwrite(&m.data[i][j], sizeof(double), 1, f);
                    } else {
                        fprintf(f,"%.17f", m.data[i][j]);
                    }
                }
            }
        
    }
}







#endif