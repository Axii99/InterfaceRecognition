#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QScreen>
#include <QApplication>
#include <QWindow>
#include <Windows.h>
#include <preprocessing.h>
#include <recognition.h>
#include <cutandsave.h>

using namespace std;
using namespace cv;
using namespace Qt;

static Mat g_org_img;
static Mat img, tmp, dst;

void on_mouse(int event,int x,int y,int flags,void *ustc) {
    static Point pre_pt = Point(-1, -1);//初始坐标
    static Point cur_pt = Point(-1, -1);//实时坐标
    static Point center = Point (-1,-1);
    char temp[16];

    if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆
    {
        g_org_img.copyTo(img);//将原始图片复制到img中
        sprintf(temp,"(%d,%d)",x,y);
        pre_pt = Point(x,y);
        putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
        //circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//划圆
        //imshow("img",img);
    }
    else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标
        sprintf(temp,"(%d,%d)",x,y);
        cur_pt = Point(x,y);

        putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
        //imshow("img",tmp);
    }
    else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形
    {
        img.copyTo(tmp);
        sprintf(temp,"(%d,%d)",x,y);
        cur_pt = Point(x,y);
        center = Point((pre_pt.x+cur_pt.x)/2, (pre_pt.y+cur_pt.y)/2);
        circle(tmp, center, 2, Scalar(0, 0, 255), -1);
        putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        rectangle(tmp,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//在临时图像上实时显示鼠标拖动时形成的矩形
        imshow("img",tmp);
    }
    else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形
    {
        g_org_img.copyTo(img);
        sprintf(temp,"(%d,%d)",x,y);
        cur_pt = Point(x,y);
        putText(img,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
        circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);
        center = Point((pre_pt.x+cur_pt.x)/2, (pre_pt.y+cur_pt.y)/2);
        circle(img, center, 2, Scalar(0, 0, 255), -1);
        rectangle(img,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//根据初始点和结束点，将矩形画到img上
        imshow("img",img);
        img.copyTo(tmp);
        CutAndSave cas;
        cas.CutImage(pre_pt,cur_pt, g_org_img, "E:\\Projects\\_Save\\account.jpg");

    }
}


Point getCenterPoint(Rect rect)
{
    Point cpt;
    cpt.x = rect.x + cvRound(rect.width/2.0);
    cpt.y = rect.y + cvRound(rect.height/2.0);
    return cpt;
}


int main(int argc, char *argv[])
{

    Sleep(3000);
    string adr = "E:\\save.jpg";
    CutAndSave cas;
    HWND handle = cas.ShotWindow(adr);



    Preprocessing pp;
    Recognition recog;



    //Basice Recognition
    Mat Interface = pp.ShowImage(adr) ;
//    Interface.copyTo(g_org_img);
//    g_org_img.copyTo(img);
//    g_org_img.copyTo(tmp);

//    Mat binary_img = pp.ConvertToBinary(Interface);
//    vector<Rect> rects = pp.FindRectContours(Interface, binary_img);
//    pair<Rect, Rect> boxes = recog.BasicRecognize(rects);
//    recog.DrawBoxes(Interface, boxes);

//    Point wnd_pos = cas.getWindowPos(handle);


//    Point mouse_pos = getCenterPoint(boxes.first);
//    SetCursorPos(mouse_pos.x+wnd_pos.x, mouse_pos.y+wnd_pos.y);
//    mouse_event(0x0002, 0, 0, 0, 0);
//    mouse_event(0x0004, 0, 0, 0, 0);




    //Cut boxes for Matching
//    namedWindow("img");
//    setMouseCallback("img",on_mouse,0);//调用回调函数
//    imshow("img",g_org_img);
//    waitKey(0);



//        //Matching Recognition
//     string str1 = "E:\\Projects\\_Save\\account.jpg";
//     string str2 = "E:\\Projects\\_Save\\org.jpg";
//     Rect box = recog.MatchingRecognize(str1,str2);

//         Point wnd_pos = cas.getWindowPos(handle);
//         Point mouse_pos = getCenterPoint(box);
//         SetCursorPos(mouse_pos.x+wnd_pos.x, mouse_pos.y+wnd_pos.y);
//         mouse_event(0x0002, 0, 0, 0, 0);
//         mouse_event(0x0004, 0, 0, 0, 0);


}

