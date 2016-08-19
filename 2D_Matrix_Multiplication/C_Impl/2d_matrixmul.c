// File:       2d_matrixmul.c
// Abstract:   A simple "Matrix Multiplication" compute example illustrating 
//             2d matrix multiplication
// Version:    <1.0>
//----------------------------------------------------------------------------//
#include <stdio.h>
//----------------------------------------------------------------------------//
#define ROW_1 6
#define COL_1 5
#define ROW_2 COL_1
#define COL_2 2

int main()
{
   int matrix_1[ROW_1*COL_1], matrix_2[ROW_2*COL_2], matrix_out[ROW_1*COL_2];
   int i, j, k, count = 0;   
   //--------Feed inputs to the two matrices-----------
   for(i = 0; i < ROW_1*COL_1 ; i++)
   {
     matrix_1[i] = 1;
   }
   
   for(i = 0; i < ROW_2*COL_2 ; i++)
   {         
     matrix_2[i] = 1;
   } 
   //---------Print Input Matrices----------------------
   printf("\nFirst Matrix :\n");
   for(i = 0; i < ROW_1 ; i++)
   {
     for(j = 0; j < COL_1; j++)
     {
       printf("%d\t", matrix_1[(i*COL_1) + j]);
     }
     printf("\n"); //Next Row
   }
   
   printf("Second Matrix :\n");
   for(i = 0; i < ROW_2 ; i++)
   {
     for(j = 0; j < COL_2; j++)
     {
       printf("%d\t", matrix_2[(i*COL_2) + j]);
     }
     printf("\n"); //Next Row
   }   
   //------------Compute and print result-----------------
   for(i = 0; i < ROW_1 ; i++)
   {         
     for(j = 0; j< COL_2 ; j++)
     {
       int sum = 0;
       for (k = 0; k < ROW_2; k++)
       {                   
         int arg1, arg2;
         arg1 = matrix_1[(i * COL_1) + k];
         arg2 = matrix_2[(k * COL_2) + j];
         sum += arg1 * arg2;            
       }
       matrix_out[(i * COL_2) + j] = sum;           
     }
   }
   
   printf("\nOutput Matrix :\n");
   for(i = 0; i < ROW_1; i++)
   {
     for(j = 0; j< COL_2; j++)
     {
       printf("%d\t", matrix_out[(i*COL_2) +j]);
     }
     printf("\n");
   }    
  return 0;
}

