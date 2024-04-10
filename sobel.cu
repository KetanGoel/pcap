#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <float.h>
#include "mypgm.h"

//kernel: function executed by GPU threads

__global__ void sobel_gpu(float* image1, float* image2, int w, int h) {

    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;

    float dx, dy;

    if (x > 0 && y > 0 && x < w - 1 && y < h - 1){

        dx = (-1 * image1[(y - 1) * w + (x - 1)]) + (-2 * image1[y * w + (x - 1)]) + (-1 * image1[(y + 1) * w + (x - 1)]) +
            (image1[(y - 1) * w + (x + 1)]) + (2 * image1[y * w + (x + 1)]) + (image1[(y + 1) * w + (x + 1)]);

        dy = (image1[(y - 1) * w + (x - 1)]) + (2 * image1[(y - 1) * w + x]) + (image1[(y - 1) * w + ( x + 1)]) +
            (-1 * image1[(y + 1) * w + (x - 1)]) + (-2 * image1[(y + 1) * w + x]) + (-1 * image1[(y + 1 ) * w + (x + 1)]);

        image2[y * w + x] = sqrt((dx * dx) + (dy * dy));
    }
}
 
//host: a function that executes on the processor

__host__ void sobel_cpu(float image1[1024][1024], float image3[1024][1024], int w, int h) {

    float dx, dy;

    for (int i = 0; i < w-2; i++) {
        for (int j = 0; j < h-2; j++) {

           
                dy = (-1 * image1[i][j]) + (-2 * image1[i][j+1]) + (-1 * image1[i][j+2]) +
                    (image1[i+2][j]) + (2 * image1[i+2][j+1]) + (image1[i+2][j+2]);


                dx = (-1 * image1[i][j]) + (1 * image1[i][j + 2])
                    + (-2* image1[i+1][j]) + (2 * image1[i+1][j+2])
                    + (-1 * image1[i+2][j]) + (1 * image1[i+2][j+2]);

                image3[i][j] = sqrt((dx * dx) + (dy * dy));
        }
    }
}

int main() {

    //Load Image (image image1 is a global variable for simplicity)
    load_image_data();

    int i, j;

    float* deviceInputImageData;
    float* deviceOutputImageData;

    //Allocating memory on the graphics card
    cudaMalloc((void**)&deviceInputImageData, x_size1 * y_size1 * sizeof(float));
    cudaMalloc((void**)&deviceOutputImageData, x_size1 * y_size1 * sizeof(float));

    //copying values ​​from main memory to graphics card memory
    cudaMemcpy(deviceInputImageData, image1, x_size1 * y_size1 * sizeof(float), cudaMemcpyHostToDevice);

    //Initialize the clock so we can track GPU execution time
    std::clock_t start_gpu;
    double duration_gpu;
    start_gpu = std::clock();

    printf(" ----------- GPU -----------\n");

    //Filling matrix2 (new images)//
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("INITIALIZE image2 (IMAGE_GPU)\n");
    x_size2 = x_size1;
    y_size2 = y_size1;
    for (i = 0; i < y_size2; i++) {
        for (j = 0; j < x_size2; j++) {
            image2[i][j] = 0;
        }
    }

    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks(64, 64);

    printf("\nBEGINNING: Sobel filter on GPU\n");
    //Function <<<dimGrid, dimBlock>>>
    sobel_gpu<<<numBlocks, threadsPerBlock>>>(deviceInputImageData, deviceOutputImageData, x_size1, y_size1);

    //End GPU clock
    duration_gpu = (std::clock() - start_gpu) / (double)CLOCKS_PER_SEC;
    std::cout << "\nCOMPLETE: Sobel filter on GPU: " << duration_gpu << "s - SUCCESS!" << '\n';
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");

    //copy the value from the graphics card to the main memory
    cudaMemcpy(image2, deviceOutputImageData, x_size2 * y_size2 * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(deviceInputImageData);
    cudaFree(deviceOutputImageData);

    save_image_data_img2();

    //initialize the clock so we can monitor CPU execution time
    std::clock_t start_cpu;
    double duration_cpu;
    start_cpu = std::clock();

    printf(" ----------- CPU -----------\n");

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("INITIALIZE image3 (IMAGE_CPU)\n");
    x_size3 = x_size1;
    y_size3 = y_size1;
    for (i = 0; i < y_size3; i++) {
        for (j = 0; j < x_size3; j++) {
            image3[i][j] = 0;
        }
    }

    printf("\nBEGINNING: Sobel filter on CPU\n");

    sobel_cpu(image1, image3, x_size3, y_size3);

    ////End of clock CPU
    duration_cpu = (std::clock() - start_cpu) / (double)CLOCKS_PER_SEC;
    std::cout << "\nCOMPLETE: Sobel filter on CPU: " << duration_cpu << "s - SUCCESS!" << '\n';
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");

    //Difference in execution time:
    double duration;
    double times;

    duration = duration_cpu - duration_gpu;
    times = duration_cpu / duration_gpu * 100;

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    std::cout << "\nEXECUTION TIME DIFFERENCE CPU - GPU: " << duration << '\n';
    std::cout << "\nGPU is FASTER than CPU by: " << times << " %" << '\n';
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
    
    save_image_data_img3();

    return 0;
}
