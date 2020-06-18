#include "recognition.h"

using namespace std;
using namespace cv;

Recognition::Recognition()
{
}

double Recognition::getDist(Point p1, Point p2){
    double distance;
    distance = powf((p1.x - p2.x),2) + powf((p1.y - p2.y),2);
    distance = sqrtf(distance);
    return distance;
}

pair<Rect, Rect> Recognition::BasicRecognize(vector<Rect> &rects) {

    pair<Rect,Rect> boxes;
    if (rects.empty()) {
        cout<<"No valid rects"<<endl;
        return boxes;
    }

    for (int i =  0; i < rects.size() - 1; ++i) {
        Rect current_box = rects[i];
        Rect candidate_box = rects[i];
        double ratio1 = (double) rects[i].width / rects[i].height;
        int isfound = 0;


        for (int j = i + 1; j < rects.size(); ++j) {

            //check area
            if (abs(rects[i].height - rects[j].height) > 10 || abs(rects[i].tl().x - rects[j].tl().x) > 10) {
                continue;
            }

            double ratio2 =(double) rects[j].width / rects[j].height;

            //check ratio
            if (abs(ratio1 - ratio2) <= 0.5) {

                if (candidate_box == rects[i] && isfound == 0) { //first similar box
                    boxes.first = rects[i];
                    boxes.second = rects[j];
                    candidate_box = rects[j];
                    isfound = 1;
                }
                else {
                    Rect tmp = rects[j];
                    double dist1 = getDist(current_box.tl(), candidate_box.tl());
                    double dist2 = getDist(current_box.tl(), tmp.tl());
                    if (dist1 > dist2) { //compare distance with other valid boxes
                        candidate_box = rects[j];
                    }
                }

            }
        }

        // compare with other pairs
        if (isfound == 1) {
            double dist1 = getDist(boxes.first.tl(), boxes.second.tl());
            double dist2 = getDist(current_box.tl(), candidate_box.tl());

            if (dist1 > dist2) {
                boxes.first = current_box;
                boxes.second = candidate_box;
            }
        }
    }

    //check positions
    if ((boxes.first.y - boxes.second.y) > 0) {
        Rect tmp = boxes.first;
        boxes.first = boxes.second;
        boxes.second = tmp;
    }
    return boxes;
}

pair<Rect, Rect> Recognition::DrawBoxes(Mat &org_img, pair<Rect, Rect> boxes) {

    rectangle(org_img,boxes.first,Scalar(255,0,0),2,1,0);
    rectangle(org_img,boxes.second,Scalar(255,0,0),2,1,0);

    string account = "Account";
    string passward = "Passward";

    putText(org_img,account,boxes.first.tl(),CV_FONT_ITALIC,2,Scalar(255,0,0),1);
    putText(org_img,passward,boxes.second.tl(),CV_FONT_ITALIC,2,Scalar(255,0,0),1);

    namedWindow("Boxes", CV_WINDOW_AUTOSIZE);
    imshow("Boxes",org_img);
    waitKey(0);
    return boxes;
}

Rect Recognition::MatchingRecognize(string& obj_adr, string& scene_adr) {
    Mat img1 = imread("E:\\Projects\\_Save\\account.jpg", 3);
    Mat img2 = imread(scene_adr, 3);
 //   Mat kernel = (Mat_<char>(3,3) << 1,1,1,1,1,1,1,1,1)/25;
 //   filter2D(img1, img1, -1, kernel);
 //   filter2D(img2, img2, -1, kernel);
    if (!img1.data||!img2.data){
       printf("图片未找到...");
    }

    imshow("input box", img1);
    waitKey(0);
//    imshow("input box_in_scene", img2);



    double t1 = getTickCount();
    //检测特征点(非线性)
    Ptr<AKAZE> detector = AKAZE::create(3,0,3,0.00001f,2,2, KAZE::DIFF_CHARBONNIER );

    //存放描述子
    Mat descriptor_obj,descriptor_scene;

    //img1特征点检测并计算描述子
    vector<KeyPoint> keypoints_obj;
    detector->detectAndCompute(img1, Mat(), keypoints_obj, descriptor_obj);
    if (keypoints_obj.empty()) {
        cout<<"NO POINTS"<<endl;
        Rect rec;
        return rec;
    }
    //img2特征点检测并计算描述子
    vector<KeyPoint> keypoints_scene;
    detector->detectAndCompute(img2, Mat(), keypoints_scene,descriptor_scene);
    double t2 = getTickCount();
    double t = (t2 - t1) * 1000 / getTickFrequency();//结果转化为毫秒
    cout<< "(ms):"<< t<<endl;;

    FlannBasedMatcher fbMatcher;
    BFMatcher bfMatcher;
    vector<DMatch>matches;
    bfMatcher.match(descriptor_obj, descriptor_scene, matches);

    if (matches.empty()) cout<<"EMPTY"<<endl;

    Mat resultImg;
    drawMatches(img1, keypoints_obj, img2, keypoints_scene, matches, resultImg,
    Scalar::all(-1), Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    namedWindow("AKAZE Matches",CV_WINDOW_NORMAL);
    imshow("AKAZE Matches", resultImg);
    waitKey(1000);

    /*那么上面的操作所显示的结果匹配到的特征点很多，为了减少多余的特征点，下面进行
    如下操作*/

    vector<DMatch>goodmatches;//保存从众多匹配点中找出的最优点
    /*
    1、遍历整个描述子；
    2、从描述子中找出最优匹配点(距离最小)
    */
    double minDist = 1000;//初始化
    double maxDist = 0;
    for (int i = 0; i < descriptor_obj.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist > maxDist)
        {
            maxDist = dist;
        }
        if (dist < minDist)
        {
            minDist = dist;
        }

    }
    for (int i = 0; i < descriptor_obj.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < max(2 * minDist, 0.02))
        {
            goodmatches.push_back(matches[i]);
        }

    }
    Mat goodmatchesImg;
//    drawMatches(img1, keypoints_obj, img2, keypoints_scene, goodmatches, goodmatchesImg,
//        Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


    Mat pptfImg= goodmatchesImg.clone();

    //画出轮廓

    vector<Point2f> obj;
    vector<Point2f> objinscene;

    for (size_t i = 0; i < matches.size(); i++)
    {
        obj.push_back(keypoints_obj[matches[i].queryIdx].pt);
        objinscene.push_back(keypoints_scene[matches[i].trainIdx].pt);
    }
    Mat H = findHomography(obj, objinscene, RANSAC);     //生成透视变换矩阵

    if (H.empty()) cout<<"aaaa"<<endl;

    vector<Point2f> obj_corner(4);//源图片4个角的坐标
    vector<Point2f> objinscene_corner(4);
    obj_corner[0] = Point(0,0);
    obj_corner[1] = Point(img1.cols, 0);
    obj_corner[2] = Point(img1.cols, img1.rows);
    obj_corner[3] = Point(0, img1.rows);

    perspectiveTransform(obj_corner, objinscene_corner, H);


    line(pptfImg, objinscene_corner[0] + Point2f(img1.cols, 0), objinscene_corner[1] + Point2f(img1.cols, 0), Scalar(0, 0, 255), 2, 8, 0);
    line(pptfImg, objinscene_corner[1] + Point2f(img1.cols, 0), objinscene_corner[2] + Point2f(img1.cols, 0), Scalar(0, 0, 255), 2, 8, 0);
    line(pptfImg, objinscene_corner[2] + Point2f(img1.cols, 0), objinscene_corner[3] + Point2f(img1.cols, 0), Scalar(0, 0, 255), 2, 8, 0);
    line(pptfImg, objinscene_corner[3] + Point2f(img1.cols, 0), objinscene_corner[0] + Point2f(img1.cols, 0), Scalar(0, 0, 255), 2, 8, 0);

    Mat org_img = imread(scene_adr);

//    line(org_img, objinscene_corner[0], objinscene_corner[1], Scalar(255, 0, 0), 2, 8, 0);
//    line(org_img, objinscene_corner[1], objinscene_corner[2], Scalar(255, 0, 0), 2, 8, 0);
//    line(org_img, objinscene_corner[2], objinscene_corner[3], Scalar(255, 0, 0), 2, 8, 0);
//    line(org_img, objinscene_corner[3], objinscene_corner[0], Scalar(255, 0, 0), 2, 8, 0);
    rectangle(org_img,objinscene_corner[0],objinscene_corner[2],Scalar(255,0,0),2,8,0);
    namedWindow("Boxes",CV_WINDOW_AUTOSIZE);
    imshow("Boxes", org_img);
    waitKey(0);
    int width = objinscene_corner[2].x - objinscene_corner[0].x;
    int height = objinscene_corner[2].y - objinscene_corner[0].y;
    return Rect(objinscene_corner[0].x,objinscene_corner[0].y,width,height);

}
