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

#ifndef CHECKERS_IA_CORE_H
#define CHECKERS_IA_CORE_H

/**
 * ia_core.h
 *
 * Ce fichier contient des fonctions permettant de construire et d'entraîner des réseaux de neuronnes
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

#include "helper.h"
#include "matrix.h"
 
 
 
/**********************\
*                      *
*       STRUCT         *
*                      *
\**********************/

// NEURAL NETWORK
typedef struct neural_network neural_network;
struct neural_network {
    
    // weights
    matrix *weights;
    int size;
    
    // bias
    matrix *bias;
    
    // matrices temporaires utilisés pour la backpropagation
    matrix *tempforward;
    
};
 
 
 
/**********************\
*                      *
*       GLOBAL         *
*                      *
\**********************/
 
// MATRIX
int __precision = 8;
double __scale = (double)1.0;
 
// NEURAL NETWORK
///////// ACTIVATION FUNCTION /////////
double sigmoid(double x) {
    return (double)1 / ((double)1 + exp(-x));
}
double sigmoid_deriv(double x) {
    return x * ( (double)1 - x);
}
///////////////////////////////////////
 
void putcharline(char c, char **line, int *index, int *size) {
    char *temp = NULL;
    if (*index+1 >= *size) {
        (*size) *= 2;
        temp = (char *)realloc(*line, (*size+1) * sizeof(char));
        if (temp == NULL) bad_memory_allocation();
        *line = temp;
    }
    (*line)[*index] = c;
    (*line)[*index+1] = '\0';
    (*index)++;
}
char * trygetdata(char v, char *data, int *index, int length) {
    int tempsize = 2;
    char *tempdata = (char *)malloc(tempsize * sizeof(char));
    if (tempdata == NULL) bad_memory_allocation();
    int tempindex = 0;
    while (data[*index] != v && *index < length) {
        putcharline(data[*index], &tempdata, &tempindex, &tempsize);
        (*index)++;
    }
    if (*index > length) {
        free(tempdata);
        return NULL;
    }
    (*index)++;
    return tempdata;
}
char * trygetdatan(int n, char *data, int *index, int length) {
    int tempsize = 2;
    char *tempdata = (char *)malloc(tempsize * sizeof(char));
    if (tempdata == NULL) bad_memory_allocation();
    int tempindex = 0;
    int i = 0;
    while (i < n && *index < length) {
        putcharline(data[*index], &tempdata, &tempindex, &tempsize);
        (*index)++;
        i++;
    }
    if (*index > length) {
        free(tempdata);
        return NULL;
    }
    return tempdata;
}
void nextline(FILE *f) {
    int c = fgetc(f);
    while (c != EOF && c != '\n') {c = fgetc(f);}
}
int isNumeric(char *s) {
    if (s == NULL || *s == '\0' || isspace(*s)) return 0;
    char * p;
    strtod(s, &p);
    return *p == '\0';
}
char * doubleToBinary(double d) {
    char *r = (char *)malloc(sizeof(d)*8+1);
    if (r == NULL) bad_memory_allocation();
    unsigned char temp[sizeof(d)];
    memcpy(&temp, &d, sizeof(d));
    r[sizeof(d)*8] = '\0';
    for (int i = sizeof(d)*8-1; i >= 0; i--) {
        r[sizeof(d)*8-1-i] = (temp[i/8] & (1 << (i%8) )) ? '1' : '0';
    }
    return r;
}
double binaryToDouble(char* d) {
    double r;
    unsigned long long x = 0;
    for (; *d; ++d) {
        x = (x << 1) + (*d - '0');
    }
    memcpy(&r, &x, sizeof(r));
    return r;
}
char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}
char *rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}
char *trim(char *s) {
    return rtrim(ltrim(s)); 
}







/**********************\
*                      *
*      FUNCTIONS       *
*                      *
\**********************/

neural_network neural_network_create(int *arr, int length) {
    
    matrix *weights = (matrix *)malloc((length-1) * sizeof(matrix));
    if (weights == NULL) bad_memory_allocation();
    matrix *bias = (matrix *)malloc((length-1) * sizeof(matrix));
    if (bias == NULL) bad_memory_allocation();
    matrix *temp = (matrix *)malloc(length * sizeof(matrix));
    if (temp == NULL) bad_memory_allocation();
    
    
    for (int i = 0; i < length-1; i++) {
        weights[i] = matrix_create(arr[i],arr[i+1]);
        bias[i] = matrix_create(1,arr[i+1]);
        matrix_random_fill(weights[i],-1,1);
        matrix_random_fill(bias[i],-1,1);
        temp[i] = matrix_create(1,1);
        matrix_random_fill(temp[i],-1,1);
    }
    temp[length-1] = matrix_create(1,1);
    
    neural_network nn = { weights, length-1, bias, temp };
    
    return nn;
    
}
void neural_network_print(neural_network nn) {
    for (int i = 0; i < nn.size; i++) {
        printf("weight %d:\n",i);
        matrix_print(nn.weights[i]);
        printf("-------------------------------------\n");
    }
}
matrix neural_network_forward(neural_network nn, double *arr, int length) {
    matrix input = matrix_create_from_array(arr, length);
    matrix_free(nn.tempforward[0]);
    nn.tempforward[0] = matrix_tr('c',input);
    double (*fun_ptr)(double) = &sigmoid; 
    matrix temp = matrix_create(1,1);
    for (int i = 0; i < nn.size; i++) {
        matrix_free(nn.tempforward[i+1]);
        if (i==0) {
            matrix _dot = matrix_op('.',input, nn.weights[i]);
            matrix _add = matrix_op('+',_dot,nn.bias[i]);
            matrix_free(temp);
            temp = matrix_map(_add,fun_ptr);
            nn.tempforward[i+1] = matrix_tr('c',temp);
            matrix_free(_add);
            matrix_free(_dot);
        } else {
            matrix _dot = matrix_op('.',temp, nn.weights[i]);
            matrix _add = matrix_op('+',_dot,nn.bias[i]);
            matrix_free(temp);
            temp = matrix_map(_add,fun_ptr);
            nn.tempforward[i+1] = matrix_tr('c',temp);
            matrix_free(_add);
            matrix_free(_dot);
        }
    }
    matrix_free(input);
    return temp;
}
void neural_network_train(neural_network nn, double *arrinput, int lengthinput, double *arrtarget, int lengthtarget) {
    double (*fun_ptr)(double) = &sigmoid_deriv;
    matrix target = matrix_create_from_array(arrtarget, lengthtarget);
    matrix output = neural_network_forward(nn, arrinput, lengthinput);
    matrix outputerror = matrix_op('-',target, output);
    matrix outputderive = matrix_map(output, fun_ptr);
    matrix outputdelta = matrix_op('*',outputerror, outputderive);
    matrix hiddenderive,hiddenerror;
    matrix hiddendeltalast = matrix_tr('c',outputdelta);
    matrix hiddendelta = matrix_tr('c',outputdelta);
    for (int i = 0; i < nn.size; i++) {
        matrix _t = matrix_tr('t',nn.weights[nn.size-i-1]);
        hiddenerror = matrix_op('.',hiddendelta,_t);
        matrix_free(_t);
        matrix_free(hiddendelta);
        hiddenderive = matrix_map(nn.tempforward[nn.size-i-1], fun_ptr);
        hiddendelta = matrix_op('*',hiddenerror, hiddenderive);
        matrix_free(hiddenerror);
        matrix_free(hiddenderive);
        _t = matrix_tr('t',nn.tempforward[nn.size-i-1]);
        matrix _dot = matrix_op('.',_t,hiddendeltalast);
        matrix _temp = nn.weights[nn.size-i-1];
        nn.weights[nn.size-i-1] =
        matrix_op('+',
            nn.weights[nn.size-i-1],
            _dot
        );
        matrix_free(_temp);
        matrix_free(_dot);
        matrix_free(_t);
        _temp = nn.bias[nn.size-i-1];
        nn.bias[nn.size-i-1] =
        matrix_op('+',
            nn.bias[nn.size-i-1],
            hiddendeltalast
        );
        matrix_free(_temp);
        matrix_free(hiddendeltalast);
        hiddendeltalast = matrix_tr('c',hiddendelta);
 
        
    }
    matrix_free(hiddendelta);
    matrix_free(hiddendeltalast);
    matrix_free(target);
    matrix_free(output);
    matrix_free(outputerror);
    matrix_free(outputderive);
    matrix_free(outputdelta);
}
void neural_network_free(neural_network nn) {
    for (int i = 0; i < nn.size; i++) {
        matrix_free(nn.weights[i]);
        matrix_free(nn.bias[i]);
        matrix_free(nn.tempforward[i]);
    }
    matrix_free(nn.tempforward[nn.size]);
    free(nn.weights);
    free(nn.bias);
    free(nn.tempforward);
}
void neural_network_export(neural_network nn, FILE *f, int binary) {
    
    // format:
    // <size>-(<rows>-<cols>-<matrix>)...
    
    if (f == NULL) {
        printf("%d-",nn.size);
    } else {
        if (binary == 1) {
            fwrite(&nn.size, sizeof(nn.size), 1, f);
        } else {
            fprintf(f,"%d-",nn.size);
        }
    }
    for (int i = 0; i < nn.size; i++) {
        matrix_print_inline(nn.weights[i],f,binary);
    }
    for (int i = 0; i < nn.size; i++) {
        matrix_print_inline(nn.bias[i],f,binary);
    }
    for (int i = 0; i <= nn.size; i++) {
        matrix_print_inline(nn.tempforward[i],f,binary);
    }
    
    
    
    
}
char * neural_network_importbinary(FILE *f) {
    
    int size = 0;
    fread(&size, sizeof(int), 1, f);
    
    int tempsize = 2;
    char *s = malloc(tempsize * sizeof(char));
    if (s == NULL) bad_memory_allocation();
    int len = 0;
    
    int length;
    char *str;
    
    length = snprintf(NULL, 0, "%d", size);
    str = (char *)malloc((length + 1) * sizeof(char));
    if (str == NULL) bad_memory_allocation();
    snprintf( str, length + 1, "%d", size);
    for (int i = 0; i < length; i++) putcharline(str[i], &s, &len, &tempsize);
    free(str);
    
    putcharline('-', &s, &len, &tempsize);
    
    int nbmatrix = size * 3 + 1;
    int index = 0;
    
    while (index < nbmatrix) {
        
        int rows = 0;
        fread(&rows, sizeof(int), 1, f);
        int cols = 0;
        fread(&cols, sizeof(int), 1, f);
        
        length = snprintf(NULL, 0, "%d", rows);
        str = (char *)malloc((length + 1) * sizeof(char));
        if (str == NULL) bad_memory_allocation();
        sprintf( str, "%d", rows);
        for (int i = 0; i < length; i++) putcharline(str[i], &s, &len, &tempsize);
        free(str);
        
        putcharline('-', &s, &len, &tempsize);
        
        length = snprintf(NULL, 0, "%d", cols);
        str = (char *)malloc((length + 1) * sizeof(char));
        if (str == NULL) bad_memory_allocation();
        sprintf(str, "%d", cols);
        for (int i = 0; i < length; i++) putcharline(str[i], &s, &len, &tempsize);
        free(str);
        
        putcharline('-', &s, &len, &tempsize);
        
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                
                double d = (double)0;
                fread(&d, sizeof(double), 1, f);
                
                length = snprintf(NULL, 0, "%.17f", d);
                str = (char *)malloc((length + 1) * sizeof(char));
                if (str == NULL) bad_memory_allocation();
                snprintf( str, length + 1, "%.17f", d);
                for (int i = 0; i < length; i++) putcharline(str[i], &s, &len, &tempsize);
                free(str);
                
            }
        }
        
        index++;
        
    }
    
    return s;
}
int neural_network_importcheck(char *data) {
    
    char *temp;
    
    // format:
    // <size>-(<rows>-<cols>-<matrix>)...
    
    int length = strlen(data);
    int j = 0;
    
    // get neural network size
    temp = trygetdata('-',data,&j,length);
    if (temp == NULL) return 1;
    if (isNumeric(temp) == 0) {free(temp);return 1;}
    int size = atoi(temp);
    free(temp);
    
    int k = 0;
    int l = 0;
    
 
    int check = 0;
    for (int i = j; i < length; i++) {
        
        // get matrix rows
        temp = trygetdata('-',data,&i,length);
        if (temp == NULL) return 1;
        if (isNumeric(temp) == 0) {free(temp);return 1;}
        int rows = atoi(temp);
        free(temp);
        
        // get matrix cols
        temp = trygetdata('-',data,&i,length);
        if (temp == NULL) return 1;
        if (isNumeric(temp) == 0) {free(temp);return 1;}
        int cols = atoi(temp);
        free(temp);
        
        // get matrix data
        matrix m = matrix_create(rows,cols);
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                
                // get next double
                char *firstpart = NULL;
                char *secondpart = NULL;
                firstpart = trygetdata('.',data,&i,length);
                if (firstpart == NULL) {
                    matrix_free(m);
                    return 1;
                }
                secondpart = trygetdatan(17,data,&i,length);
                if (secondpart == NULL) {
                    matrix_free(m);
                    return 1;
                }
                if (isNumeric(firstpart) == 0) {matrix_free(m);free(firstpart);return 1;}
                if (isNumeric(secondpart) == 0) {matrix_free(m);free(secondpart);return 1;}
                
                char *_double = (char *)malloc((strlen(firstpart)+17+1) * sizeof(char));
                if (_double == NULL) bad_memory_allocation();
                strcpy(_double, firstpart);
                strcat(_double, ".");
                strcat(_double, secondpart);
                
                m.data[y][x] = atof(_double);
               
                
                free(_double);
                free(firstpart);
                free(secondpart);
                
            }
        }
        matrix_free(m);
        
        if (i == length) {
            break;
        } else if (l > length) {
           return 1;
        }
        
        l++;
        if (k < 2 && l == size) {
            l = 0;
            k++;
        }
        i--;
        
        
        if (check >= length) {
            return 1;
        }
        
        check++;
        
    }
    
    return 0;
}
neural_network neural_network_import(char *data, int *success) {
    
    *success = 0;
    
    if (neural_network_importcheck(data) == 1) {
        neural_network nn;
        return nn;
    }
    
    char *temp;
    
    // format:
    // <size>-(<rows>-<cols>-<matrix>)...
    
    int length = strlen(data);
    int j = 0;
    
    // get neural network size
    temp = trygetdata('-',data,&j,length);
    int size = atoi(temp);
    free(temp);
    
    matrix *weights = (matrix *)malloc((size) * sizeof(matrix));
    if (weights == NULL) bad_memory_allocation();
    matrix *bias = (matrix *)malloc((size) * sizeof(matrix));
    if (bias == NULL) bad_memory_allocation();
    matrix *tempn = (matrix *)malloc((size+1) * sizeof(matrix));
    if (tempn == NULL) bad_memory_allocation();
    
    int k = 0;
    int l = 0;
 
    int check = 0;
    for (int i = j; i < length; i++) {
        
        // get matrix rows
        temp = trygetdata('-',data,&i,length);
        int rows = atoi(temp);
        free(temp);
        
        // get matrix cols
        temp = trygetdata('-',data,&i,length);
        int cols = atoi(temp);
        free(temp);
        
        // get matrix data
        if (k == 0) {
            weights[l] = matrix_create(rows,cols);
        } else if (k == 1) {
            bias[l] = matrix_create(rows,cols);
        } else {
            tempn[l] = matrix_create(rows,cols);
        }
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                
                // get next double
                char *firstpart = NULL;
                char *secondpart = NULL;
                firstpart = trygetdata('.',data,&i,length);
                secondpart = trygetdatan(17,data,&i,length);
                
                char *_double = (char *)malloc((strlen(firstpart)+17+1) * sizeof(char));
                if (_double == NULL) bad_memory_allocation();
                strcpy(_double, firstpart);
                strcat(_double, ".");
                strcat(_double, secondpart);
                
                if (k == 0) {
                    weights[l].data[y][x] = atof(_double);
                } else if (k == 1) {
                    bias[l].data[y][x] = atof(_double);
                } else {
                    tempn[l].data[y][x] = atof(_double);
                }
               
                
                free(_double);
                free(firstpart);
                free(secondpart);
                
            }
        }
        
        if (i == length) {
            break;
        } else if (l > length) {
            fprintf(stderr, "Err: bad data\n");
            exit(1);
        }
        
        l++;
        if (k < 2 && l == size) {
            l = 0;
            k++;
        }
        i--;
        
        if (check >= length) {
            fprintf(stderr, "Err: bad data\n");
            exit(1);
        }
        
        check++;
        
    }
    
    neural_network nn = { weights, size, bias, tempn };
    
    *success = 1;
    
    return nn;
    
}



#endif