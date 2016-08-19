__kernel void square(                                                     
	__global int* matrix1,                                              
	__global int* matrix2,
	__global int* col1,
	__global int* col2,
	__global int* output)                                           
{                                                                    
   int i = get_global_id(0); 
   int j = get_global_id(1);   
   int result = 0;
   
   for (int k = 0; k < col1 ; ++k)
   {
      result += matrix1[i*col1 + k] * matrix2[k*col2 + j];
   }
   output[i*col2 + j] = result;                                                                                            
 }                                                                      

