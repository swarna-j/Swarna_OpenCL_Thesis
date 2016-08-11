// File:       MatrixMultiplication.c
// Abstract:   A simple "Matrix Multiplication" compute example illustrating 
//             matrix multiplication in 1D
// Version:    <1.0>
//----------------------------------------------------------------------------//
#include <stdio.h>
//----------------------------------------------------------------------------//
#define SIZE 5
#define true 1
#define false 0

int main()
{
    int matrix_1[SIZE], matrix_2[SIZE], matrix_out[5*SIZE];
    int row1, col1, row2, col2;
    int i, j, count = 0, idx = 0, sum = 0;
    
    printf("Enter the dimensions of the first matrix\t:\n");
    scanf("%d %d", &row1, &col1);
    printf("Enter the dimensions of the second matrix\t:\n");
    scanf("%d %d", &row2, &col2);
    
    if(col1 != row2)
    {
      printf("Matrix Multiplication is not possible..\n");
    }
    else
    {
      //---------------------------------------------------------
      //Receive the inputs : matrix 1 and 2 in form of a 1D array
      //---------------------------------------------------------     
      printf("\nEnter the elements of the first matrix\n");
      for(i = 0; i < row1*col1 ; i++)
      {
        scanf("%d", &matrix_1[i]); 
        printf("matrix_1[%d] = %d\n", i, matrix_1[i]);
      }
      
      printf("\nEnter the elements of the second matrix\n");
      for(j = 0; j < row2*col2 ; j++)
      {
        scanf("%d", &matrix_2[j]);
        printf("matrix_2[%d] = %d\n", j, matrix_2[j]); 
      }
      //---------------------------------------------------------
      //Compute & print the multiplication result of matrix 1 & 2
      //---------------------------------------------------------
      printf("\nThe Output matrix is : \n");       
      
      for(i = 0; i < (row1*col1)+ 1 ; ++i)
      {         
         if(count < col1)
         {
            sum += matrix_1[i]*matrix_2[count];            
            count++;
         }
         else
         {
            matrix_out[idx] = sum;                       
            sum = 0;
            count = 0;
            idx ++;
            i--; 
            //As an (if..else) statement has been used, the value of 'i' for which count >= col1 shall be skipped when calculating the sum.
            //So 'i' has to be decremented to compute the addition of next set of products
         }                     
       }          
       for(idx = 0; idx < row1*col2; idx++)
       {
         printf("\t%d\n", matrix_out[idx]);
       }       
    }
    return 0;
}

