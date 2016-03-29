#include "opencv2/pencilfilters.hpp"

#include "boxFilter.h"

#include "opencv2/core/cvstd.hpp"
#include "opencv2/cudafilters.hpp"

#include <cuda.h>
//#include <cuda_runtime.h>
#include <npp.h>


using namespace cv;
using namespace cv::pencil::cuda;

namespace {
    void normalizeAnchor(int& anchor, int ksize)
    {
        if (anchor < 0)
            anchor = ksize >> 1;

        CV_Assert( 0 <= anchor && anchor < ksize );
    }

    void normalizeAnchor(Point& anchor, Size ksize)
    {
        normalizeAnchor(anchor.x, ksize.width);
        normalizeAnchor(anchor.y, ksize.height);
    }

	class NPPBoxFilter : public Filter
    {
    public:
        NPPBoxFilter(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal);

        void apply(InputArray src, OutputArray dst, Stream& stream = Stream::Null());

    private:
        //typedef NppStatus (*nppFilterBox_t)(const Npp8u* pSrc, Npp32s nSrcStep, Npp8u* pDst, Npp32s nDstStep,
        //                                    NppiSize oSizeROI, NppiSize oMaskSize, NppiPoint oAnchor);

        Size ksize_;
        Point anchor_;
        int type_;
        //nppFilterBox_t func_;
        int borderMode_;
        Scalar borderVal_;
        //GpuMat srcBorder_;
    };

    NPPBoxFilter::NPPBoxFilter(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal) :
        ksize_(ksize), anchor_(anchor), type_(srcType), borderMode_(borderMode), borderVal_(borderVal)
    {
        CV_Assert(borderMode == BORDER_REFLECT_101); // Other border modes not implemented.
        CV_Assert(srcType == CV_8UC1); // Other types not implemented

        //static const nppFilterBox_t funcs[] = {0, nppiFilterBox_8u_C1R, 0, 0, nppiFilterBox_8u_C4R};

        CV_Assert( srcType == CV_8UC1 || srcType == CV_8UC4 );
        CV_Assert( dstType == srcType );

        normalizeAnchor(anchor_, ksize);

        //func_ = funcs[CV_MAT_CN(srcType)];
    }

    void NPPBoxFilter::apply(InputArray _src, OutputArray _dst, Stream& _stream) {
        CV_Assert(!_stream); // Implementation does not support streaming

		GpuMat src = _src.getGpuMat();
        CV_Assert(src.type() == type_);

        _dst.create(src.size(), src.type());
        GpuMat dst = _dst.getGpuMat();


        CV_Assert(src.step % sizeof(unsigned char) == 0);
        size_t srcLineStride = src.step1(); // src.step / sizeof(unsigned char);

        CV_Assert(dst.step % sizeof(unsigned char) == 0);
        size_t dstLineStride = dst.step1(); // dst.step / sizeof(unsigned char);

        applyBoxFilter(src.cols, src.rows, srcLineStride, src.ptr<Npp8u>(), dstLineStride, dst.ptr<Npp8u>(), ksize_.width, ksize_.height, anchor_.x, anchor_.y);

#if 0
        GpuMat src = _src.getGpuMat();
        CV_Assert( src.type() == type_ );

        //                   src  dst         top            bottom         left          right         bortderType  val
        cuda::copyMakeBorder(src, srcBorder_, ksize_.height, ksize_.height, ksize_.width, ksize_.width, borderMode_, borderVal_, _stream);
        // srcBorder_ size: Size(left+src.cols+right, top+src.rows+bottom)

        _dst.create(src.size(), src.type());
        GpuMat dst = _dst.getGpuMat();

        GpuMat srcRoi = srcBorder_(Rect(ksize_.width, ksize_.height, src.cols, src.rows));

        cudaStream_t stream = StreamAccessor::getStream(_stream);
        NppStreamHandler h(stream);

        NppiSize oSizeROI;
        oSizeROI.width = src.cols;
        oSizeROI.height = src.rows;

        NppiSize oMaskSize;
        oMaskSize.height = ksize_.height;
        oMaskSize.width = ksize_.width;

        NppiPoint oAnchor;
        oAnchor.x = anchor_.x;
        oAnchor.y = anchor_.y;

        nppSafeCall( func_(srcRoi.ptr<Npp8u>(), static_cast<int>(srcRoi.step),
                           dst.ptr<Npp8u>(), static_cast<int>(dst.step),
                           oSizeROI, oMaskSize, oAnchor) );

        if (stream == 0)
            cudaSafeCall( cudaDeviceSynchronize() );
#endif
    } // NPPBoxFilter::apply


	class NPPBoxFilterCpu : public Filter
    {
    public:
        NPPBoxFilterCpu(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal);

        void apply(InputArray src, OutputArray dst, Stream& stream = Stream::Null());

    private:
        //typedef NppStatus (*nppFilterBox_t)(const Npp8u* pSrc, Npp32s nSrcStep, Npp8u* pDst, Npp32s nDstStep,
        //                                    NppiSize oSizeROI, NppiSize oMaskSize, NppiPoint oAnchor);

        Size ksize_;
        Point anchor_;
        int type_;
        //nppFilterBox_t func_;
        int borderMode_;
        Scalar borderVal_;
        //GpuMat srcBorder_;
    };

    NPPBoxFilterCpu::NPPBoxFilterCpu(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal) :
        ksize_(ksize), anchor_(anchor), type_(srcType), borderMode_(borderMode), borderVal_(borderVal)
    {
        CV_Assert(borderMode == BORDER_REFLECT_101); // Other border modes not implemented.
        CV_Assert(srcType == CV_8UC1); // Other types not implemented

        //static const nppFilterBox_t funcs[] = {0, nppiFilterBox_8u_C1R, 0, 0, nppiFilterBox_8u_C4R};

        CV_Assert( srcType == CV_8UC1 || srcType == CV_8UC4 );
        CV_Assert( dstType == srcType );

        normalizeAnchor(anchor_, ksize);

        //func_ = funcs[CV_MAT_CN(srcType)];
    }

    void NPPBoxFilterCpu::apply(InputArray _src, OutputArray _dst, Stream& _stream) {
        CV_Assert(!_stream); // Implementation does not support streaming

		Mat src = _src.getMat();
        CV_Assert(src.type() == type_);

        _dst.create(src.size(), src.type());
        Mat dst = _dst.getMat();


        CV_Assert(src.step % sizeof(unsigned char) == 0);
        size_t srcLineStride = src.step1(); // src.step / sizeof(unsigned char);

        CV_Assert(dst.step % sizeof(unsigned char) == 0);
        size_t dstLineStride = dst.step1(); // dst.step / sizeof(unsigned char);

        applyBoxFilter_cpu(src.cols, src.rows, srcLineStride, src.ptr<Npp8u>(), dstLineStride, dst.ptr<Npp8u>(), ksize_.width, ksize_.height, anchor_.x, anchor_.y);

#if 0
        GpuMat src = _src.getGpuMat();
        CV_Assert( src.type() == type_ );

        //                   src  dst         top            bottom         left          right         bortderType  val
        cuda::copyMakeBorder(src, srcBorder_, ksize_.height, ksize_.height, ksize_.width, ksize_.width, borderMode_, borderVal_, _stream);
        // srcBorder_ size: Size(left+src.cols+right, top+src.rows+bottom)

        _dst.create(src.size(), src.type());
        GpuMat dst = _dst.getGpuMat();

        GpuMat srcRoi = srcBorder_(Rect(ksize_.width, ksize_.height, src.cols, src.rows));

        cudaStream_t stream = StreamAccessor::getStream(_stream);
        NppStreamHandler h(stream);

        NppiSize oSizeROI;
        oSizeROI.width = src.cols;
        oSizeROI.height = src.rows;

        NppiSize oMaskSize;
        oMaskSize.height = ksize_.height;
        oMaskSize.width = ksize_.width;

        NppiPoint oAnchor;
        oAnchor.x = anchor_.x;
        oAnchor.y = anchor_.y;

        nppSafeCall( func_(srcRoi.ptr<Npp8u>(), static_cast<int>(srcRoi.step),
                           dst.ptr<Npp8u>(), static_cast<int>(dst.step),
                           oSizeROI, oMaskSize, oAnchor) );

        if (stream == 0)
            cudaSafeCall( cudaDeviceSynchronize() );
#endif
    } // NPPBoxFilter::apply
} // namespace


cv::Ptr<Filter> cv::pencil::cuda::createBoxFilter(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal)
{
    if (dstType < 0)
        dstType = srcType;

    dstType = CV_MAKE_TYPE(CV_MAT_DEPTH(dstType), CV_MAT_CN(srcType));

    return makePtr<NPPBoxFilter>(srcType, dstType, ksize, anchor, borderMode, borderVal);
}

cv::Ptr<Filter> cv::pencil::cuda::createBoxFilter_cpu(int srcType, int dstType, Size ksize, Point anchor, int borderMode, Scalar borderVal)
{
    if (dstType < 0)
        dstType = srcType;

    dstType = CV_MAKE_TYPE(CV_MAT_DEPTH(dstType), CV_MAT_CN(srcType));

    return makePtr<NPPBoxFilterCpu>(srcType, dstType, ksize, anchor, borderMode, borderVal);
}
