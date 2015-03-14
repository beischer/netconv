/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. All rights reserved.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
//#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
        
public:
    MainWindow(QWidget *parent = 0);
    
protected:
    
private slots:
    void Convert();
    void OpenNetlist();
    void OpenMapfile();
    void OpenOutputNetlistfileKiCAD();
    void saveSettings();
    int saveStringList(QString filename, QStringList netlist);
    QStringList loadStringList(QString netlistfilename);
    void exit();
    void aboutQt();
    void aboutNetconv();
   
private:

    QLabel * inputNetlistLabel;
    QLineEdit * inputNetlistLineEdit;
    QPushButton * chooseInputNetlistPushButton;

    QLabel * inputMapFileLabel;
    QLineEdit * inputMapFileLineEdit;
    QPushButton * chooseMapFilePushButton;


    QLabel * outputNetlistLabel;
    QLineEdit * outputNetlistLineEdit;
    QPushButton * chooseOutputNetlistPushButton;

    QPushButton * convertPushButton;

    QLabel * consoleLabel;
    QTextEdit * consoleTextEdit;

    QGridLayout * mainGridLayout;

    QWidget * mainWidget;

};

#endif
