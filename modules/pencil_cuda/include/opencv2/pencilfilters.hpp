#ifndef _OPENCV_PENCILFILTERS_HPP
#define _OPENCV_PENCILFILTERS_HPP

#include "opencv2/cudafilters.hpp"

using namespace cv::cuda;

namespace cv { namespace pencil { namespace cuda {

CV_EXPORTS Ptr<Filter> createBoxFilter(int srcType, int dstType, Size ksize, Point anchor = Point(-1,-1),
                                       int borderMode = BORDER_DEFAULT, Scalar borderVal = Scalar::all(0));


CV_EXPORTS Ptr<Filter> createBoxFilter_cpu(int srcType, int dstType, Size ksize, Point anchor = Point(-1,-1),
                                       int borderMode = BORDER_DEFAULT, Scalar borderVal = Scalar::all(0));

}}}

#endif /* _OPENCV_PENCILFILTERS_HPP */
