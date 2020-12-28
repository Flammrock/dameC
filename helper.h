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


#ifndef CHECKERS_HELPER_H
#define CHECKERS_HELPER_H




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
#include <errno.h>







/**********************\
*                      *
*       HELPER         *
*                      *
\**********************/
 
void bad_argument() {
    fprintf(stderr,"Err: Bad arguments or Missing arguments, please see the documentation for more details\n");
    exit(1);
}
void bad_memory_allocation() {
    fprintf(stderr,"Err: unsuccessful allocation\n");
    exit(2);
}
int is_exist_file(char *name) {
  FILE *file;
  if ((file = fopen(name, "r")) == NULL) {
    if (errno == ENOENT) {
      return 1;
    } else {
      return 2;
    }
  } else {
    fclose(file);
  }
  return 0;
}


#endif