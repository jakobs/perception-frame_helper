
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE "frame_helper"
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <highgui.h>
#include <cv.h>
#include "../src/FrameHelper.h"
#include <iostream>

using namespace base::samples::frame;
using namespace frame_helper;

//test if min max search is working
BOOST_AUTO_TEST_CASE(convert_image)
{
    //load image 
    cv::Mat image =  cv::imread("test.jpg");
    BOOST_CHECK(NULL != image.data);

    //check if convertion is working
    Frame frame;
    frame.init(image.cols,image.rows,8,MODE_BGR);
    memcpy(frame.getImagePtr(),image.ptr(),image.rows*image.cols * image.elemSize());
    cv::Mat image2 = frame.convertToCvMat();
    cv::namedWindow("Test");
    cv::imshow("Test",image2);
    cv::waitKey(1000);

    //resize image
    Frame frame2(100,100,8,MODE_BGR);
    FrameHelper::resize(frame,frame2);
    cv::imshow("Test",frame2.convertToCvMat());
    cv::waitKey(2000);
    
    FrameHelper frame_helper;
    frame_helper.convert(frame,frame2);
    cv::imshow("Test",frame2.convertToCvMat());
    cv::waitKey(2000);
    
    //convert color
    frame2.init(300,300,8,MODE_GRAYSCALE);
    frame_helper.convert(frame,frame2);
    cv::imshow("Test2",frame2.convertToCvMat());
    cv::waitKey(2000);

    //convert image 

//    BOOST_CHECK_EQUAL(6,result.size());
//    BOOST_CHECK_EQUAL(0,*result[0]);
//    BOOST_CHECK_EQUAL(5,*result[1]);
//    BOOST_CHECK_EQUAL(0,*result[2]);
//    BOOST_CHECK_EQUAL(0,*result[3]);
//    BOOST_CHECK_EQUAL(6,*result[4]);
//    BOOST_CHECK_EQUAL(0,*result[5]);
}

