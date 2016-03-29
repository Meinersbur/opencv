
set(the_description "Kernels written in PENCIL and compiled for CUDA")
ocv_define_module(pencil_cuda opencv_cudafilters)

set_target_properties(opencv_pencil_cuda PROPERTIES
	C_STANDARD 99
	C_STANDARD_REQUIRED ON)
