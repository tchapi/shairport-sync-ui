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
    
    window.setStyleSheet("background-color: black; color: white;");
    window.showFullScreen();

    return app.exec();
}