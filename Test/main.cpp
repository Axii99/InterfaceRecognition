
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
//    HOGDescriptor hog(Size(50, 30), Size(10,6), Size(5,3), Size(5,3),7);
//    int DescriptorDim;

//    Mat data,labels;   //特征矩阵

//    String pattern = "E:/CutImage/*.JPG";
//    vector<String> names;
//    glob(pattern, names);

//    for (int i = 0; i < names.size() ; ++i) {
//        cout<<names[i]<<endl;
//        Mat temp = imread(names[i]);
//        resize(temp, temp, Size(50,30));
//        vector<float> descriptors;
//        hog.compute(temp,descriptors, Size(5,3));
//        if (i == 0) {
//            DescriptorDim = descriptors.size();
//            data = Mat::zeros(names.size(),DescriptorDim,CV_32F);
//            labels = Mat::zeros(names.size(), 1, CV_32F);
//        }
//        //        Mat gimg;
//        //        cvtColor(temp,gimg,CV_BGR2GRAY);
//        //        resize(gimg, gimg, Size(50,30));
//        //        data.push_back(gimg.reshape(0,1));
//        for (int j = 0; j < DescriptorDim; j++){
//            data.at<float>(i, j) = descriptors[j];
//        }

//        if (i <= 9) {
//            labels.at<int>(i,0) = 1;
//        }
//        else {
//            labels.at<int>(i, 0) = 0;
//        }

//    }

//    data.convertTo(data, CV_32F); //uchar型转换为cv_32f
//    labels.convertTo(labels,CV_32F);
//    Mat trainData, trainLabels;
//    int samplesNum = data.rows;
//    int trainNum = 3000;



//    trainData = data(Range(0, 14), Range::all());
//    trainLabels = labels(Range(0, 14), Range::all());

//    //使用KNN算法
//    int K = 5;
//    Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainLabels);
//    Ptr<KNearest> model = KNearest::create();
//    model->setDefaultK(K);
//    model->setIsClassifier(true);
//    model->train(tData);
//    model->KNearest::save("knn_hog.xml");

//    model->StatModel::load<KNearest>("knn_hog.xml");

//    float re = model->predict(data.row(19));
//    cout<< re <<endl;


    String adr = "E:\\traindata\\data\\cascade.xml";
    CascadeClassifier cc;
    cc.load(adr);
    Mat img = imread("E:\\traindata\\a2.JPG");
    vector<Rect> boxes;
    cc.detectMultiScale(img,boxes,1.1,3,0);

    for (int i = 0; i < boxes.size(); i++){
        rectangle(img,boxes[i].tl(),boxes[i].br(),Scalar(255,0,0),1,1,0);
    }
    imshow("aaa",img);
    waitKey(0);
    String adrr = "E:\\traindata\\neg_samples\\*.jpg";
    vector<String> vect;
    glob(adrr,vect);
    ofstream ff("E:\\traindata\\negdata.txt");
    for (int i = 0 ; i < vect.size(); i ++){
        cout<<vect[i]<<endl;
        ff<<vect[i]<<endl;
    }
    return 0;

}
