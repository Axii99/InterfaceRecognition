#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <utility>
#include <iostream>

using namespace std;
using namespace cv;


class Recognition
{
public:
    Recognition();
    pair<Rect, Rect> BasicRecognize(vector<Rect>&);
    double getDist(Point, Point);
    pair<Rect, Rect> DrawBoxes(Mat&, pair<Rect,Rect>);
    Rect MatchingRecognize(string&, string&);
};

#endif // RECOGNITION_H
