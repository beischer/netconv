/****************************************************************************
**
** Copyright (C) 2004-2010 Magnus Beischer. All rights reserved.
**
****************************************************************************/

#ifndef KICAD2013_H
#define KICAD2013_H


class KiCAD2013
{
  
public:
    KiCAD2013();
    QStringList MakeNetList(QTextEdit * con, QStringList TinyCADnetlist, QString mapfile);
    QStringList LoadMapfile(QString mapfilename);
    QString getSymbol(QTextEdit * console, QString product_number, QStringList mapfilelist);
        
private slots:
   
private:


};

#endif
