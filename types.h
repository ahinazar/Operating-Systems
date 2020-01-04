typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef struct {
    int *tree;
    int *used_ids;
    int depth;
} trnmnt_tree;