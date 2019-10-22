/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. All rights reserved.
**
****************************************************************************/

#include <QtWidgets>
//#include <QtGui>
//#include <QDialog>
//#include <QFile>
//#include <QStringList>
//#include <QSettings>
//#include <QTextStream>
//#include <QMessageBox>

#include "mainwindow.h"
#include "tinycad.h"
#include "kicad2013.h"

#define FILE_OPEN_BUTTON_WIDTH 25

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setGeometry(200, 100, 700, 350);

    inputNetlistLabel = new QLabel("Input TinyCAD netlist");
    inputNetlistLineEdit = new QLineEdit();
    chooseInputNetlistPushButton = new QPushButton("...");
    chooseInputNetlistPushButton->setMaximumWidth(FILE_OPEN_BUTTON_WIDTH);

    inputMapFileLabel = new QLabel("KiCAD product number to footprint map file");
    inputMapFileLineEdit = new QLineEdit();
    chooseMapFilePushButton = new QPushButton("...");
    chooseMapFilePushButton->setMaximumWidth(FILE_OPEN_BUTTON_WIDTH);

    outputNetlistLabel = new QLabel("Output KiCAD netlist");
    outputNetlistLineEdit = new QLineEdit();
    chooseOutputNetlistPushButton = new QPushButton("...");
    chooseOutputNetlistPushButton->setMaximumWidth(FILE_OPEN_BUTTON_WIDTH);

    convertPushButton = new QPushButton("Do Convert!");

    consoleLabel = new QLabel("Console");
    consoleTextEdit = new QTextEdit();

    mainGridLayout = new QGridLayout();

    mainWidget = new QWidget(this);

    mainGridLayout->addWidget(inputNetlistLabel, 0, 0, 1, 2);
    mainGridLayout->addWidget(inputNetlistLineEdit, 1, 0, 1, 2);
    mainGridLayout->addWidget(chooseInputNetlistPushButton, 1, 2, 1, 1);

    mainGridLayout->addWidget(inputMapFileLabel, 2, 0, 1, 2);
    mainGridLayout->addWidget(inputMapFileLineEdit, 3, 0, 1, 2);
    mainGridLayout->addWidget(chooseMapFilePushButton, 3, 2, 1, 1);

    mainGridLayout->addWidget(outputNetlistLabel, 4, 0, 1, 2);
    mainGridLayout->addWidget(outputNetlistLineEdit, 5, 0, 1, 2);
    mainGridLayout->addWidget(chooseOutputNetlistPushButton, 5, 2, 1, 1);

    mainGridLayout->addWidget(convertPushButton, 6, 0, 1, 1);

    mainGridLayout->addWidget(consoleLabel, 7, 0, 1, 1);
    mainGridLayout->addWidget(consoleTextEdit, 8, 0, 1, 3);

    mainGridLayout->setColumnStretch(0, 0);
    mainGridLayout->setColumnStretch(1, 1);
    mainGridLayout->setColumnStretch(2, 0);

    mainWidget->setLayout(mainGridLayout);
    setCentralWidget(mainWidget);


    setWindowIcon(QIcon("doneandnext.png"));
    //setWindowFlags(Qt::WindowMinimizeButtonHint);

    // The menu signals
//    connect(action_Exit, SIGNAL(triggered()), this, SLOT(exit()));
//    connect(convertPushButton, SIGNAL(triggered()), this, SLOT(Convert()));
//    connect(actionAbout_Netconv, SIGNAL(triggered()), this, SLOT(aboutNetconv()));
//    connect(actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));

    // File dialogs
    connect(chooseInputNetlistPushButton, SIGNAL(clicked()), this, SLOT(OpenNetlist()));
    connect(chooseMapFilePushButton, SIGNAL(clicked()), this, SLOT(OpenMapfile()));
    connect(chooseOutputNetlistPushButton, SIGNAL(clicked()),
            this, SLOT(OpenOutputNetlistfileKiCAD()));


    // Other signals
    connect(convertPushButton, SIGNAL(clicked()), this, SLOT(Convert()));

    // connect(actionAbout_Netconv, SIGNAL(trigged(), this, SLOT(about()));
    convertPushButton->setFocus();
    
    //Reading the old settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MySoft", "Netconv");
    
    if(settings.contains("Standard/Filename")){
        inputNetlistLineEdit->setText(settings.value("Standard/Filename").toString());
    }
    else{
        inputNetlistLineEdit->setText("No saved settings");
    }

    if(settings.contains("Standard/Mapfile")){
        inputMapFileLineEdit->setText(settings.value("Standard/Mapfile").toString());
    }
    else{
        inputMapFileLineEdit->setText("No saved settings");
    }

    if(settings.contains("Standard/KiCADoutputfile")){
        outputNetlistLineEdit->setText(settings.value("Standard/KiCADoutputfile").toString());
    }
    else{
        outputNetlistLineEdit->setText("No saved settings");
    }
    consoleTextEdit->append("Netconv 1.6.6 (release 2019-10-22)");
}

void MainWindow::Convert(){
    saveSettings();
    QStringList netlist_dirty, netlist_clean;
    saveSettings();
    consoleTextEdit->append("Starting to convert netlist.");

        //Convert to KiCAD 2013
        consoleTextEdit->clear();
        QCoreApplication::processEvents();
        // Get a clean net list
        TinyCAD * my_tinycad = new TinyCAD();
        netlist_dirty = loadStringList(inputNetlistLineEdit->text());
        netlist_clean = my_tinycad->cleanNetlist(netlist_dirty);

        // Print some design data
        consoleTextEdit->append("\nDesign data:");
        consoleTextEdit->append("Number of components: " +
                        QString::number(my_tinycad->NumberOfComponents(netlist_clean),10)
                        );
        consoleTextEdit->append("Number of nets: " +
                        QString::number(my_tinycad->NumberOfNets(netlist_clean),10)
                        );
        consoleTextEdit->append("Number of nodes: " +
                        QString::number(my_tinycad->NumberOfNodes(netlist_clean),10)
                        );

        // Generate the kicad netlist and save it
        KiCAD2013 * my_kicad_2013 = new KiCAD2013();
        QStringList kicad_netlist;

        kicad_netlist = my_kicad_2013->MakeNetList(consoleTextEdit, netlist_clean,
                                              inputMapFileLineEdit->text());

        saveStringList(outputNetlistLineEdit->text(), kicad_netlist);

        consoleTextEdit->append("\nConversion ready!");
        delete my_tinycad;
        delete my_kicad_2013;

}

void MainWindow::exit(){
    saveSettings();
    close();
}

void MainWindow::aboutNetconv(){
    QMessageBox::about(this, "About Netconv",
                       "Netconv is a net list conversion tool for TinyCAD and KiCAD pcbnew.\n\n"
                       "Copyright 2007-2018 Magnus Beischer.\n"
                       "Version: 1.6.5 (relase date 2019-10-01)\n\n"
                       "magnus@beischer.com");
}

void MainWindow::aboutQt(){
    QMessageBox::aboutQt(this, "Netconv");
}

void MainWindow::OpenNetlist(){
    consoleTextEdit->append(tr("Opening netlist file."));

    // Getting the old dir.
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MySoft", "Netconv");
    QString dir = settings.value("Standard/Filename").toString();
    QString s = QFileDialog::getOpenFileName(this,
                                             "Choose a net list",
                                             dir, "TinyCAD (*.net)");
    if( s.length() > 2){
        inputNetlistLineEdit->setText(s);
        
    }
    consoleTextEdit->append(tr("You have choosen: "));
    consoleTextEdit->append(s);
    saveSettings();
}

void MainWindow::OpenMapfile(){
    consoleTextEdit->append(tr("Opening the map file."));

    // Getting the old dir
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MySoft", "Netconv");
    QString dir = settings.value("Standard/Mapfile").toString();
    QString s = QFileDialog::getOpenFileName(this, "Choose a map file", dir,"(*.txt)");
    if( s.length() > 2){
        inputMapFileLineEdit->setText(s);
    }
    consoleTextEdit->append(tr("You have choosen: "));
    consoleTextEdit->append(s);
    saveSettings();
}


void MainWindow::OpenOutputNetlistfileKiCAD(){
    consoleTextEdit->append(tr("Setting the KiCAD output file"));

    // Getting the old dir
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MySoft", "Netconv");
    QString dir = settings.value("Standard/KiCADoutputfile").toString();
    QString s = QFileDialog::getSaveFileName(this, "Choose a netlist output file", dir,
                                             "(*.net)");
    if( s.length() > 2){
        outputNetlistLineEdit->setText(s);
        
    }
    consoleTextEdit->append(tr("You have choosen: "));
    consoleTextEdit->append(s);
    saveSettings();

}

void MainWindow::saveSettings(){
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MySoft", "Netconv");
    
    settings.setValue("Standard/Filename",  inputNetlistLineEdit->text());
    settings.setValue("Standard/Mapfile",  inputMapFileLineEdit->text());
    settings.setValue("Standard/KiCADoutputfile", outputNetlistLineEdit->text());
    settings.sync();
}

int MainWindow::saveStringList(QString filename, QStringList netlist){
    QFile outfile(filename);
    QTextStream out(&outfile);

    // Write the clean netlist to a file
    if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Could not open output file";
        return false;
    }
    else{
        for(int k = 0;k <= netlist.size() - 1; k++){
            out << netlist.at(k) << endl;
        }
        outfile.close();
    }
    qDebug() << "File saved!";
    return true;
}

QStringList MainWindow::loadStringList(QString netlistfilename){
    QFile infile(netlistfilename);
    QStringList netlist;
    QTextStream in(&infile);

    if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("TinyCAD::Could not open file");
        return netlist;
    }

    while (!in.atEnd()) {
        // Read entire netlist into the application
        netlist.append( in.readLine() );
    }
    infile.close();
    return netlist;
}

