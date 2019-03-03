/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. All rights reserved.
**
****************************************************************************/

#include <QFile>
#include <QStringList>
#include <QSettings>
#include <QTextStream>
#include <QtDebug>
#include <QTextEdit>
#include <QCoreApplication>
#include <QDateTime>

#define COMPONENT_TEXT_LEN 10
#define NET_TEXT_LEN	    5

#include "tinycad.h"
#include "kicad2013.h"

KiCAD2013::KiCAD2013(){
    //infile_name = infile;
    //qDebug("Creating KiCAD object");
}

QStringList KiCAD2013::MakeNetList(QTextEdit * console, QStringList TinyCADnetlist, QString mapfile){
    //QStringList mapfilelist;
    QHash<QString, QString> mapfilelist;
    QStringList poslist;
    QStringList kicad_netlist;

    // Read the mapfile info a string list
    mapfilelist = LoadMapfile(mapfile);

    // Start the kicad netlist
    kicad_netlist.append("(export (version D)");
    /* An example of the file that shall be created: // The (version D) is needed to support KiCAD 2015- (at least)
       (export (version D)
         (components
           (comp (ref Q1)
             (value BC237)
             (footprint TO92-EBC)
            )
         )
         (nets
           (net (name Kalle)
             (node (ref Q1) (pin B))
             (node (ref Q1) (pin E)))
           (net (name N001)
             (node (ref Q1) (pin C)))
         )
       )
*/
    TinyCAD * my_tinyCAD = new TinyCAD;
    poslist = my_tinyCAD->getPosList(TinyCADnetlist);

    QString symbol, product_number;

    // New
    // The component part
    kicad_netlist.append("  (components");
    for(int j = 0; j <= poslist.size() - 1 ; j++){
        QCoreApplication::processEvents();
        // symbol = "0603"
        // pos = "R100"
        // product_number = "HMR001/0"

        product_number = my_tinyCAD->getProductNumberByPos(console, TinyCADnetlist, poslist.at(j));
        symbol = getSymbol(console, product_number, mapfilelist);

        kicad_netlist.append("    (comp (ref " + poslist.at(j) + ")");
        kicad_netlist.append("      (value " + product_number + ")");
        kicad_netlist.append("      (footprint " + symbol + ")");
        kicad_netlist.append("    )");  // Ending this compoent
    }
    // Ending all components
    kicad_netlist.append("  )");
    kicad_netlist.append("  (nets");

    // Loop through the net list
    for(int row = 0;row<TinyCADnetlist.size();row++){
        QString theRow = TinyCADnetlist.at(row);
        if(theRow.startsWith("NET  '")){
            // This is a net list row    NET  'N000000' =  (D1,2),(D1,3)
            // Get the net name
            QStringList rowParts = theRow.split("'");
            QString netName = rowParts.at(1); //N000000
            kicad_netlist.append("    (net (name " + netName + ")");  // Starting the net tag
            QString componentPartOfNetRow = rowParts.at(2); //  =  (D1,2),(D1,3)
            componentPartOfNetRow = componentPartOfNetRow.remove(" =  ");  // (D1,2),(D1,3)
            QStringList componentPinList;
            componentPinList = componentPartOfNetRow.split(","); // D1 2 D1 3  (in a list)
            //qDebug() << componentPinList;
            int node = 0;
            while(node < componentPinList.size()){
                QString ref = componentPinList.at(node);
                QString pin = componentPinList.at(node + 1);
                // ref must be a real component (pos) included in the component list (not a hierarcical symbol)
                if(poslist.contains(ref.remove("("))){
                    kicad_netlist.append("      (node (ref " + ref.remove("(") + ") (pin " + pin.remove(")") + "))");
                }
                node = node + 2;
            }
            kicad_netlist.append("    )"); // Ending the net tag
        }
    }
    kicad_netlist.append("  )");
    kicad_netlist.append(")");

    return kicad_netlist;
}

QHash<QString, QString> KiCAD2013::LoadMapfile(QString mapfilename){
	QFile infile(mapfilename);
    QHash<QString, QString> mapfilelist;
    QTextStream in(&infile);
    QString readRow;
    
    if (!infile.open(QIODevice::ReadOnly | QIODevice::Text)){
    	qDebug("KiCAD::Could not open file");
    	return mapfilelist;
    }
  
    while (!in.atEnd()) {
    	// Read entire netlist into the application
        readRow = in.readLine();
        // replace tab with space
        readRow = readRow.replace(QRegExp("[\t]"), " ");
        readRow = readRow.trimmed();
        //qDebug() << readRow;
        if(readRow.contains(" ")){
            // We have both product number and symbol
            // Split the string
            QStringList splitString;
            splitString = readRow.split(" ", QString::SkipEmptyParts);
            mapfilelist.insert(splitString.at(0), splitString.at(1));
        }
    }

    infile.close();
    return mapfilelist;
}

QString KiCAD2013::getSymbol(QTextEdit * console, QString product_number_tinycad, QHash<QString, QString> mapfilelist){
    QString mapfilelistrow, pn, productNumberMapFile, symbol = "";
    if(mapfilelist.contains(product_number_tinycad)){
        return mapfilelist.value(product_number_tinycad);
    }
    else{
        console->append("Error: " + product_number_tinycad + " missing in mapfile");
        return "FAKE-SYMBOL";
    }
}
