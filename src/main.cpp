/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. LGPL license.
**
****************************************************************************/

#include <QApplication>
//#include <QIcon>
//#include <QStringList>
//#include <QPixmap>
//#include <QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
    
}
