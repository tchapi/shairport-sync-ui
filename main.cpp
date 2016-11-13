#include <QApplication>
 
#include "metadataWindow.h"
 
#include <sys/time.h>
#include <sys/types.h>

// #include <stdio.h>
// #include <unistd.h>
// #include <string.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <arpa/inet.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOverrideCursor(Qt::BlankCursor);

    metadataWindow window;
    fd_set rfds;
 
    window.setStyleSheet("background-color: black; color: white;");
    window.show(); //FullScreen();

    return app.exec();
}