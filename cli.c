#include "kernel.h"
#include <string.h>

int generate_pagefault() {

}

int main(int argc, char** argv){
    // TODO: parse the arguments in argv. 
    // You can expect argv[1] to be the mode
    // You can expect argv[2] to be the filepath
    // You can expect argv[3] to be the integer width
    // You can expect argv[4] to be the integer height
    // You can expect argv[5] to be the output filepath.

    if(argc != 6) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <MODE=kernel|mmap|convert|uconvert|fault> <input_image> <width> <height> <output_image_path>\n");
        return -1;
    }

    if (strcmp(argv[1], "kernel") !=0) {
        return 1;
    }

    int width = atoi(argv[3]);
    int height = atoi(argv[4]);

    if (width <= 0 || height <= 0) {

        return 1;
    }

    struct image input = { .pixels = NULL, .width = width, .height = height};

    if(loadimage(argv[2], &input) !=0 || input.pixels == NULL) {
        
        free(input.pixels);
        return 1;
    }

    // TODO: call correct function based on mode

    // TODO: allocate the space needed for one image and load the image

    int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

    // TODO: call apply kernel with 1/9 (as a float) as the normalization value

    struct image *output = apply_kernel(&input, &kernel[0][0], 3, 1.0f/9.0f);

    if(output == NULL) {
        free(input.pixels);
        return 1;

    }

    int save_result = saveimage(argv[5], output);
    free(input.pixels);
    free(output-> pixels);
    free(output);

    return save_result == 0 ? 0 : 1;

}