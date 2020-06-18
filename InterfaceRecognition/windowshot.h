#ifndef WINDOWSHOT_H
#define WINDOWSHOT_H

#include <Windows.h>
#include <iostream>
#include <QScreen>
#include <QWindow>
#include <opencv2/core/core.hpp>
#include <QApplication>

using namespace Qt;
using namespace cv;
using namespace std;

class WindowShot
{
public:
    WindowShot();
    Mat ShotActiveWindow(string);
};

#endif // WINDOWSHOT_H
