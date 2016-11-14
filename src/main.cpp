#include <QApplication>
 
#include "metadataWindow.h"
 
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    metadataWindow window;
    
    window.setStyleSheet("background-color: black; color: white;");
    window.setFixedSize(320,240);
    window.show();//FullScreen();

    return app.exec();
}