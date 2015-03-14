TEMPLATE	= app

QT		+= designer

HEADERS		= mainwindow.h tinycad.h kicad2013.h

SOURCES		= main.cpp mainwindow.cpp tinycad.cpp kicad2013.cpp

RC_FILE         = netconv.rc

QTDIR_build:REQUIRES="contains(QT_CONFIG, large-config)"
