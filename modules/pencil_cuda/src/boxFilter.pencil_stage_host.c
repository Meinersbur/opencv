
#include "boxFilter.pencil_stage_kernel.h"

#include </home/meinersbur/src/pencilcc/pencil-headers/include/pencil_ppcg.h>
# 1 "boxFilter.pencil.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "boxFilter.pencil.c"
# 1 "boxFilter.h" 1






void applyBoxFilter(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][cols], int dstCols, unsigned char dst[static const restrict rows][cols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);







void applyBoxFilter(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][cols], int dstCols, unsigned char dst[static const restrict rows][cols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);
# 2 "boxFilter.pencil.c" 2
# 1 "/home/meinersbur/src/pencilcc/pencil-headers/include/pencil.h" 1
# 46 "/home/meinersbur/src/pencilcc/pencil-headers/include/pencil.h"
enum npr_mem_tags {
 PENCIL_NPR_MEM_NOWRITE = 1,
 PENCIL_NPR_MEM_NOREAD = 2,
 PENCIL_NPR_MEM_NOACCESS = PENCIL_NPR_MEM_NOWRITE | PENCIL_NPR_MEM_NOREAD,
 PENCIL_NPR_MEM_READ = 4,
 PENCIL_NPR_MEM_WRITE = 8,
 PENCIL_NPR_MEM_READWRITE = PENCIL_NPR_MEM_READ | PENCIL_NPR_MEM_WRITE
};
void __prl_npr_mem_tag(void *location, enum npr_mem_tags mode) __attribute__((weak));
static void __pencil_npr_mem_tag(void *location, enum npr_mem_tags mode) {
 if (&__prl_npr_mem_tag)
  __prl_npr_mem_tag(location, mode);
}
# 3 "boxFilter.pencil.c" 2
# 1 "/usr/include/assert.h" 1 3 4
# 35 "/usr/include/assert.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 364 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 402 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 403 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 365 "/usr/include/features.h" 2 3 4
# 388 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 389 "/usr/include/features.h" 2 3 4
# 36 "/usr/include/assert.h" 2 3 4
# 66 "/usr/include/assert.h" 3 4




# 69 "/usr/include/assert.h" 3 4
extern void __assert_fail (const char *__assertion, const char *__file,
      unsigned int __line, const char *__function)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));


extern void __assert_perror_fail (int __errnum, const char *__file,
      unsigned int __line, const char *__function)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));




extern void __assert (const char *__assertion, const char *__file, int __line)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__noreturn__));



# 4 "boxFilter.pencil.c" 2



# 6 "boxFilter.pencil.c"
void applyBoxFilter(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][srcCols], int dstCols, unsigned char dst[static const restrict rows][dstCols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY) {


    int se_dim = filterSizeX*filterSizeY;

    __ppcg_applyBoxFilter_prog0(srcCols, rows, dstCols, cols, se_dim, filterSizeY, filterSizeX, filterAnchorX, filterAnchorY, (unsigned char*)&dst[0], (unsigned char*)&src[0]);
}
