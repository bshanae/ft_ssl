#ifndef BITFLAGS_TOOLS
#define BITFLAGS_TOOLS

#define IS_BIT(x, y) ((x) == (y))
#define HAS_BIT(x, y) (((x) & (y)) == (y))
#define PUT_BIT(x, y) ((x) | (y))

#endif
