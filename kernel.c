#include "loader.h"
#include <stdlib.h>
#include <string.h>

/** Returns p1 with each channel multiplied by scalar. */
struct pixel mul(struct pixel p1, float scalar) {
    return (struct pixel){r: p1.r * scalar, g: p1.g * scalar, b: p1.b * scalar};
}
/** Returns the channel-wise sum of p1 and p2. */
struct pixel add(struct pixel p1, struct pixel p2) {
    return (struct pixel){r: p1.r + p2.r, g: p1.g + p2.g, b: p1.b + p2.b};
}

/**
 * Applies a square kernel to an image (cross-correlation).
 *
 * Produces a new image where each output pixel is the weighted sum of
 * the ksize x ksize neighborhood centered on the corresponding input
 * pixel, multiplied by normalize. The kernel is applied as-is (not
 * flipped), so this is technically cross-correlation; the result is
 * identical to convolution for symmetric kernels.
 *
 * The input img is padded so that kernel operations that fall outside of the 
 * original image are multiplied by a black pixel (zero padding).
 *
 * img        Source image. Not modified.
 * kernel     Kernel weights in row-major order, containing ksize * ksize elements.
 * ksize      Width and height of the kernel. Should be odd
 * normalize  Scale factor applied to each weighted sum
 *                       (e.g., 1.0f / 9 for a 3x3 box blur).
 *
 * Returns a pointer to a newly allocated image with the same dimensions as img.
 *
 */
struct image* apply_kernel(struct image* img, int* kernel, int ksize, float normalize) {
    
    int origin = ksize /2;

    if (img == NULL || img-> pixels == NULL || kernel == NULL || img-> width <= 0 || img-> height <= 0 || ksize <= 0 || ksize %2 == 0) {

        return NULL;
    }

    struct image* out = malloc(sizeof(*out)); 
    if (out == NULL) {

        return NULL;
    }

    out-> width = img-> width;
    out-> height = img-> height;
    out-> pixels = malloc(sizeof(*out-> pixels) * out-> width * out-> height);

    if(out-> pixels == NULL){
        free(out);

        return NULL;
    }

    for (int y = 0; y < img-> height; y++) {
        for (int x =0; x < img-> width; x++) {
            struct pixel sum = {0,0,0};

            for (int kernel_y = 0; kernel_y < ksize; kernel_y ++) {
                for (int kernel_x= 0; kernel_x < ksize; kernel_x++) {

                    int image_x = x + kernel_x - origin;
                    int image_y = y + kernel_y - origin;

                    if (image_x < 0 || image_x >= img-> width || image_y < 0 || image_y >= img-> height) {
                        continue;
                    }
                    int image_index = image_y * img-> width + image_x;
                    int kernel_index = kernel_y * ksize + kernel_x;
                    
                    struct pixel weighted = mul(img-> pixels[image_index], kernel [kernel_index]);
                    sum = add(sum, weighted);

                }
            }
            int output_index = y * out-> width + x;
            out-> pixels[output_index] = mul(sum, normalize);
        }
    }
    return out;
}

