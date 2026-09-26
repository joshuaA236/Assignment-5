#include "kernel.h"
#include <string.h>

int generate_pagefault() {
    return 0;

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

    int width = atoi(argv[3]);
    int height = atoi(argv[4]);

    if (width <= 0 || height <= 0) {

        return 1;
    }

    if (strcmp(argv[1], "fault") ==0) {

        return generate_pagefault();
}

    struct image input = { .pixels = NULL, .width = width, .height = height};
    struct image* output = NULL;
    int result = 1;


    if (strcmp(argv[1], "kernel") ==0) {
        if(loadimage(argv[2], &input) !=0 || input.pixels == NULL) {
            free(input.pixels);
            return 1;
        }

    int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    output = apply_kernel(&input, &kernel[0][0], 3, 1.0f/9.0f);

    if (output == NULL) {
        free(input.pixels);
        return 1;
    }
    result = saveimage(argv[5], output);

} else if (strcmp(argv[1], "mmap") == 0) {
    if (loadimage_mmap(argv[2], &input) !=0 || input.pixels == NULL) {
        free(input.pixels);
        return 1;
    }

    int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    output = apply_kernel(&input, &kernel[0][0], 3, 1.0f/9.0f);

    if(output == NULL) {
        free(input.pixels);
        return 1;
    }

    result =saveimage_mmap(argv[5], output);

} else if(strcmp(argv[1], "convert") == 0) {
    if (loadimage(argv[2], &input) != 0 || input.pixels == NULL) {

        free(input.pixels);
        return 1;
    }

    
    result =saveimage_mmap(argv[5], &input);

} else if(strcmp(argv[1], "uconvert") == 0) {
    if (loadimage_mmap(argv[2], &input) != 0 || input.pixels == NULL) {

        free(input.pixels);
        return 1;
    }

    result = saveimage(argv[5], &input);

} else {

    free(input.pixels);
    return 1;
}

free(input.pixels);
if(output != NULL) {
    free(output->pixels);
    free(output);
}

return result == 0 ? 0 : 1;

}
