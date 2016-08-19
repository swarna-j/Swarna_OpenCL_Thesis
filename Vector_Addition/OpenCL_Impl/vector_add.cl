__kernel void square(                                                     
	__global int* input1,                                              
	__global int* input2,
	__global int* output)                                           
{                                                                    
   int i = get_global_id(0);                                                                                                
   output[i] = (2*input1[i]) + (3*input2[i]) + 1;                                
}                                                                      

