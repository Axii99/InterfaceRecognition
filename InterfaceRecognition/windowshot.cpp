#include "windowshot.h"

using namespace std;
using namespace cv;
using namespace Qt;

WindowShot::WindowShot()
{
}

Mat WindowShot::ShotActiveWindow(string adr){
    Sleep(3000);
    QApplication a(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();

    HWND hCurrentCursor = GetForegroundWindow ();
    cout<<hCurrentCursor<<endl;
    screen->grabWindow((WId) hCurrentCursor).save(adr,"jpg");

    QWindow* window;
    window = QWindow::fromWinId((WId)hCurrentCursor);

    SetCursorPos(1000,480);
    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//down
    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);//up
}
