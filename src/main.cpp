#include <QApplication>
 
#include "metadataWindow.h"
 
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    metadataWindow window;
    
    window.setStyleSheet("background-color: black; color: white;");
    window.showFullScreen();

    return app.exec();
}