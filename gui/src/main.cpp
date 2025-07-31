#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "../include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("CellNinjia Desktop");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("NEMO Lab");
    app.setApplicationDisplayName("CellNinjia Desktop - Next-Generation Network Monitor");
    
    // Set application icon and style
    app.setStyle("Fusion");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}