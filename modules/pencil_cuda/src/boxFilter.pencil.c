#include "boxFilter.h"
#include <pencil.h>
#include <assert.h>

#if 1
void applyBoxFilter_cpu(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][srcCols], int dstCols, unsigned char dst[static const restrict rows][dstCols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY) {

    //dimension of filter
    int se_dim = filterSizeX*filterSizeY;

#pragma scop
    // Assume there is work to do.
    __pencil_assume(0 <= rows);
    __pencil_assume(0 <= cols);

    // srcCols/dstCols are for padding/skip/pitch/stride; hence must be at least have space for col elements.
    __pencil_assume(cols <= srcCols);
    __pencil_assume(cols <= dstCols);

    // Larger filters would require reflect twice.
    __pencil_assume(0 < filterSizeX < cols);
    __pencil_assume(0 < filterSizeY < rows);

    // Filter anchor must be within box.
    __pencil_assume(0 <= filterAnchorX < filterSizeX);
    __pencil_assume(0 <= filterAnchorY < filterSizeY);


    //go through image
    for(int y=0;y<rows;y++) {
        for(int x=0;x<cols;x++) {
            //new pixel -> reset sum
            int avg_sum = 0;

            //go through the rectangular neighborhood of pixel
            for(int dy=0; dy<filterSizeY; dy++) {
                int orig_y = y+dy-filterAnchorY;

                //conditions for BORDER_TYPE = BORDER_REFLECT_101
                int curr_y = orig_y;
                if(curr_y < 0)
                    curr_y = -curr_y;
                else if(curr_y >= rows)
                    curr_y = 2*(rows-1)-curr_y;

                for(int dx=0; dx<filterSizeX; dx++) {
                    int orig_x = x+dx-filterAnchorX;

#if 1
                    //conditions for BORDER_TYPE = BORDER_REFLECT_101
                    int curr_x = orig_x;
                    if(curr_x< 0)
                        curr_x = -curr_x;
                    else if(curr_x >= cols)
                        curr_x = 2*(cols-1)-curr_x;

                    //add value of pixel from neighborhood to sum
                    avg_sum += src[curr_y][curr_x];
#else
                    if (orig_x < 0)
                        if (orig_y < 0)
                            avg_sum += src[-orig_y][-orig_x];
                        else if (orig_y >= rows)
                            avg_sum += src[2*(rows-1)-orig_y][-orig_x];
                        else
                            avg_sum += src[orig_y][-orig_x];
                    else if (orig_x >= cols)
                        if (orig_y < 0)
                            avg_sum += src[-orig_y][2*(cols-1)-orig_x];
                        else if (orig_y >= rows)
                            avg_sum += src[2*(rows-1)-orig_y][2*(cols-1)-orig_x];
                        else
                            avg_sum += src[orig_y][2*(cols-1)-orig_x];
                    else
                        if (orig_y < 0)
                            avg_sum += src[-orig_y][orig_x];
                        else if (orig_y >= rows)
                            avg_sum += src[2*(rows-1)-orig_y][orig_x];
                        else
                            avg_sum += src[orig_y][orig_x];
#endif
                }
            }

            //assign average neighborhood value to current pixel in the output array
            dst[y][x] = avg_sum/se_dim;
        }
    }
#pragma endscop
}
#else

void applyBoxFilter_cpu(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][srcCols], int dstCols, unsigned char dst[static const restrict rows][dstCols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY) {

    int se_dim = filterSizeX*filterSizeY;
    assert(filterSizeX==1);
    assert(filterAnchorX==0);

#pragma scop
    // Assume there is work to do.
    __pencil_assume(0 <= rows);
    __pencil_assume(0 <= cols);

    // srcCols/dstCols are for padding/skip/pitch/stride; hence must be at least have space for col elements.
    __pencil_assume(cols <= srcCols);
    __pencil_assume(cols <= dstCols);

    // Larger filters would require reflect twice.
    __pencil_assume(0 < filterSizeX < cols);
    __pencil_assume(0 < filterSizeY < rows);

    // Filter anchor must be within box.
    __pencil_assume(0 <= filterAnchorX < filterSizeX);
    __pencil_assume(0 <= filterAnchorY < filterSizeY);


    for(int x=0; x<cols; x+=1) {

        int sum = 0;
        for (int fy=0; fy<filterSizeY-1; fy+=1) {

            int orig_y = fy-filterAnchorY;
            int reflect_y = orig_y;
            if (reflect_y < 0)
                reflect_y = -reflect_y;
            else if (reflect_y >= rows)
                reflect_y = 2*(rows-1)-reflect_y;

            sum += src[reflect_y][x];
        }

        for(int y=0;y<rows;y++) {

            int last_y = y-filterAnchorY+filterSizeY-1;
            int last_reflect_y = last_y;
            if (last_reflect_y < 0)
                last_reflect_y = -last_reflect_y;
            else if (last_reflect_y >= rows)
                last_reflect_y = 2*(rows-1)-last_reflect_y;

            sum += src[last_reflect_y][x];


            dst[y][x] = sum/se_dim;


            int first_y = y-filterAnchorY;
            int first_reflect_y = first_y;
            if(first_reflect_y < 0)
                first_reflect_y = -first_reflect_y;
            else if(first_reflect_y >= rows)
                first_reflect_y = 2*(rows-1)-first_reflect_y;

            sum -= src[first_reflect_y][x];
        }
    }
#pragma endscop
}

#endif
