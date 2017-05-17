#ifndef REHRESHTHREAD_H
#define REHRESHTHREAD_H

#include "mainwindow.h"
#include <QThread>

class RehreshThread : public QThread
{
    Q_OBJECT
private:
    CubeMainWindow * mainWindow;
public:
    RehreshThread(CubeMainWindow *);
    void run();
signals:
    void rehresh();

};

#endif // REHRESHTHREAD_H
