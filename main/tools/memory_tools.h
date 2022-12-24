#ifndef MEMORY_TOOLS
#define MEMORY_TOOLS

#define ROTATE_LEFT_32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTATE_RIGHT_32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define SWAP_32(x) ( (((x) >> 24) & 0x000000FF) | (((x) >>  8) & 0x0000FF00) | (((x) <<  8) & 0x00FF0000) | (((x) << 24) & 0xFF000000) )

#endif