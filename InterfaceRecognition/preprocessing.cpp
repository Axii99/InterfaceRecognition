#include "preprocessing.h"

using namespace std;
using namespace cv;

Preprocessing::Preprocessing()
{}

Mat Preprocessing::ShowImage(string &adr) {
    Mat img = imread(adr);
    if (img.empty()) {
        cout << "NOT EXIST" << endl;
        return img;
    }
    namedWindow("IMG", CV_WINDOW_AUTOSIZE);
    imshow("IMG", img);
    waitKey(1000);
    return img;
}

Mat Preprocessing::ConvertToBinary(Mat &org_img) {
    Mat gray_img;
    Mat binary_img;
    Mat filter_img;
    Mat kernel = Mat::ones(5,5, CV_32F) / 25;
    filter2D(org_img, filter_img, -1, kernel);

    //get gray image
    cvtColor(filter_img, gray_img, CV_BGR2GRAY);
    medianBlur(gray_img, gray_img, 5);

    //get binary image
    adaptiveThreshold(gray_img, binary_img, 200, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,27,3);
//    namedWindow("Binary", CV_WINDOW_AUTOSIZE);
//    imshow("Binary", binary_img);
//    waitKey(1000);
    return binary_img;
}

vector<Rect> Preprocessing::FindRectContours(Mat &org_img, Mat &binary_img) {
    vector<vector<Point>> contours;
    findContours(binary_img, contours, noArray(), RETR_LIST,CV_CHAIN_APPROX_NONE);
    vector<Rect> boundRects(contours.size());
    vector<Rect> filteredRects;
    Mat drawing = org_img;

    for (int i = 0; i < contours.size(); ++i) {
        Rect rect = boundingRect(Mat(contours[i]));
        boundRects[i] = rect;
        double ratio = (double) rect.width / rect.height;
        if ((ratio >= 4 && ratio <= 9)
                && (rect.height >= 20 && rect.height <= 800)
                && (rect.width >= 80 && rect.width <= 800)) {
            filteredRects.push_back(rect);
            //rectangle(drawing,boundRects[i],Scalar(0,0,255),1,1,0);
        }
    }

//    namedWindow("Rects", CV_WINDOW_AUTOSIZE);
//    imshow("Rects",drawing);
//    waitKey(0);
    return filteredRects;
}
