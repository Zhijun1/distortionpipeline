
#define PARALLEL_UNITS 16

/* SAD window size must be an odd number and it must be less than minimum of image height and width and less than the
 * tested size '21' */
#define SAD_WINDOW_SIZE 15

// Configure this based on the number of rows needed for Remap function
#define XF_REMAP_BUFSIZE 128

#define INPUT_PTR_WIDTH 32
#define OUTPUT_PTR_WIDTH 32

#define XF_USE_URAM false
