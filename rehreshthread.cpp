#include "rehreshthread.h"

RehreshThread::RehreshThread(CubeMainWindow * mainWindow) {
    this->mainWindow = mainWindow;
}


void RehreshThread::run() {
    while(true){
        emit this->rehresh();
        usleep(1000000);
    }
}
