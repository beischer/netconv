/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. All rights reserved.
**
****************************************************************************/

#ifndef TINYCAD_H
#define TINYCAD_H

#include <QtWidgets>


class TinyCAD
{
  
public:
    TinyCAD();
    QStringList cleanNetlist(QStringList dirtynetlist);
    QStringList getPosList(QStringList netlist);
    QString getProductNumberByPos(QTextEdit * console, QStringList TinyCADnetlist, QString pos);
    QString getOptionByPos(QTextEdit * console, QStringList TinyCADnetlist, QString pos, QString option);
    QStringList getListOfPinsAndNetNames(QStringList TinyCADnetlist, QString pos);
    int NumberOfComponents(QStringList netlist);
    int NumberOfNets(QStringList netlist);
    int NumberOfNodes(QStringList netlist);
        
private slots:
   
private:

};

#endif
