#ifndef CUTANDSAVE_H
#define CUTANDSAVE_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <utility>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace cv;


class CutAndSave
{
public:
    Mat org_img;
    Mat dst_img;
    CutAndSave();
    Mat CutImage(Point, Point, Mat, string);
    HWND ShotWindow(string);
    Point getWindowPos(HWND);
    double getDpiScaling();
};

#endif // CUTANDSAVE_H
