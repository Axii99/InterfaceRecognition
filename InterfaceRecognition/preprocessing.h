#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <utility>

using namespace std;
using namespace cv;

class Preprocessing
{
public:
    Preprocessing();
    Mat ShowImage(string&);
    Mat ConvertToBinary(Mat&);
    vector<Rect> FindRectContours(Mat&, Mat&);

};

#endif // PREPROCESSING_H
