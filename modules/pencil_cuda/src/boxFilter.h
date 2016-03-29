
#ifdef __cplusplus
extern "C" {
void applyBoxFilter(int cols, int rows, int srcCols, unsigned char * const src, int dstCols, unsigned char * const dst, int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);
}
#else
void applyBoxFilter(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][cols], int dstCols, unsigned char dst[static const restrict rows][cols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);
#endif

#ifdef __cplusplus
extern "C" {
void applyBoxFilter_cpu(int cols, int rows, int srcCols, unsigned char * const src, int dstCols, unsigned char * const dst, int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);
}
#else
void applyBoxFilter_cpu(int cols, int rows, int srcCols, unsigned char src[static const restrict rows][cols], int dstCols, unsigned char dst[static const restrict rows][cols], int filterSizeX, int filterSizeY, int filterAnchorX, int filterAnchorY);
#endif
