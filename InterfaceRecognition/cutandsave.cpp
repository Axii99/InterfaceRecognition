#include "cutandsave.h"

using namespace std;
using namespace cv;

CutAndSave::CutAndSave()
{
}

Mat CutAndSave::CutImage(Point tl, Point br, Mat org_img, string address) {
    Mat dst = org_img;
    int width = abs(tl.x - br.x);
    int height = abs(tl.y - br.y);
    if (width == 0 || height == 0)
    {
        printf("width == 0 || height == 0");
        return dst;
    }
    dst = dst(Rect(min(br.x, tl.x),min(br.y, tl.y),width,height));
    Mat tmp = dst.clone();
    imwrite(address, tmp);
//    namedWindow("dst");
//    imshow("dst",dst);
//    waitKey(0);
    return dst;
}

HWND CutAndSave::ShotWindow(string fileName) {

    //get dpi percent
    double dpiScaling = getDpiScaling();


    size_t origsize = fileName.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(fileName.length()-1));
    mbstowcs_s(&convertedChars, wcstring, origsize, fileName.c_str(), _TRUNCATE);

    HWND window = GetDesktopWindow();
     HDC _dc = GetWindowDC(window);
     HDC dc = CreateCompatibleDC(_dc);

     RECT re;
     GetWindowRect(window, &re);
     int w = (re.right - re.left) * dpiScaling / 100,
         h = (re.bottom - re.top) * dpiScaling / 100;
     void* buf = new char[w*h*4];

     HBITMAP bm = CreateCompatibleBitmap(_dc, w, h);
     SelectObject(dc, bm);

     BitBlt(dc, 0, 0, w, h, _dc, 0, 0,SRCCOPY);

     void* f =CreateFile(wcstring, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
     free(wcstring);
     GetObject(bm, sizeof(bm), &bm);

     tagBITMAPINFO bi;
     bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
     bi.bmiHeader.biWidth = w;
     bi.bmiHeader.biHeight = h;
     bi.bmiHeader.biPlanes = 1;
     bi.bmiHeader.biBitCount = 32;
     bi.bmiHeader.biCompression = BI_RGB;
     bi.bmiHeader.biSizeImage = w*h;

     CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buf, 0, 0);
     GetDIBits(dc, bm, 0, h, buf, &bi, DIB_RGB_COLORS);

     BITMAPFILEHEADER bif;
     bif.bfType = MAKEWORD('B','M');
     bif.bfSize = w*h*4+(DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
     bif.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

     BITMAPINFOHEADER bii;
     bii.biSize = sizeof(BITMAPINFOHEADER);
     bii.biWidth = w;
     bii.biHeight = h;
     bii.biPlanes = 1;
     bii.biBitCount = 32;
     bii.biCompression = BI_RGB;
     bii.biSizeImage = w*h*4;

     DWORD r;
     WriteFile(f, &bif, sizeof(bif), &r, NULL);
     WriteFile(f, &bii, sizeof(bii), &r, NULL);
     WriteFile(f, buf, w*h*4, &r, NULL);

     CloseHandle(f);
     DeleteDC(_dc);
     DeleteDC(dc);

     Mat screen = imread(fileName);
     HWND sub = GetForegroundWindow();
     RECT subrect;
     GetWindowRect(sub, &subrect);
     double R = dpiScaling / 100;


    resize(screen,screen,Size(screen.cols/R,screen.rows/R),0,0,INTER_LINEAR);
     Point tl = Point(subrect.left ,subrect.top) ;
     Point br = Point(subrect.right,subrect.bottom);



     CutImage(tl, br, screen , fileName);
     return sub;
}

Point CutAndSave::getWindowPos(HWND sub) {

    RECT subrect;
    GetWindowRect(sub, &subrect);
    Point tl =  Point(subrect.left,subrect.top);
    return tl;
}

double CutAndSave::getDpiScaling() {
    HWND wnd = ::GetDesktopWindow();
    HDC dc = GetDC(wnd);
    int desktopVerts = GetDeviceCaps(dc, DESKTOPVERTRES );
    int verts = GetDeviceCaps(dc, VERTRES );
    double dpiScaling = desktopVerts*100 / verts;
    return dpiScaling;
}

