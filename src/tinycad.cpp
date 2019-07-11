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

#define COMPONENT_TEXT_LEN 10
#define NET_TEXT_LEN	    5

#include "tinycad.h"

TinyCAD::TinyCAD(){

}


QStringList TinyCAD::cleanNetlist(QStringList netlist){
    QStringList netlistclean;
    //qDebug() << "Cleaning up netlist";
    // Loop through the netlist and create another without the bugs
    QString instr, outstr, netlistrow, netname, cleannetlistrow, tempstring;
    int startpos1, startpos2;
    for(int j = 0; j <= netlist.size() - 1 ; j++){
    	netlistrow = netlist.at(j);
    	
    	// Count the number of components in the netlist 
    	if( netlist.at(j).startsWith(QString("COMPONENT '") )){
    		//number_of_components++;
            //consoleTextEdit->append(QString::number(number_of_components, 10) + ", ");
    	}
    	
    	// Fix the byggy nets
    	if( netlist.at(j).startsWith(QString("NET  ") )){
    		// qDebug() << "Found net row in file";
    		// Here comes a net, start by getting the net name
    		startpos1 = netlistrow.indexOf("'", 0); // First occurence of '
    		startpos2 = netlistrow.indexOf("'", startpos1 + 1); // Second occurence of '
    		netname = netlistrow.mid(startpos1 + 1, startpos2 - startpos1 - 1); // Net name
            
    		// Case 1, starting the net string with a buggy node
    		tempstring = netname;
    		tempstring.append(",");
    		netlistrow.remove(tempstring);
            
    		// Case 2, a buggy node name inside the list or last in the list
    		tempstring = ",";
    		tempstring.append(netname);
    		netlistrow.remove(tempstring);
    	}
    	else{
    	}
		netlistclean.append(netlistrow);
        //qDebug() << netlistrow;
    }
    return netlistclean;
}


int TinyCAD::NumberOfComponents(QStringList netlist){
    int number_of_components = 0;
    for(int j = 0; j <= netlist.size() - 1 ; j++){
    	// Count the number of components in the netlist 
    	if( netlist.at(j).startsWith(QString("COMPONENT ") )){
    		number_of_components++;
    	}
    }
    return number_of_components;
}

int TinyCAD::NumberOfNets(QStringList netlist){
    int number_of_nets = 0;
    for(int j = 0; j <= netlist.size() - 1 ; j++){
    	// Count the number of components in the netlist 
    	if( netlist.at(j).startsWith(QString("NET ") )){
    		number_of_nets++;
    	}
    }
    return number_of_nets;
}

int TinyCAD::NumberOfNodes(QStringList netlist){
    int number_of_pins = 0;
    QString netlistrow;
    for(int j = 0; j <= netlist.size() - 1 ; j++){
    	if( netlist.at(j).startsWith(QString("NET ") )){
            netlistrow = netlist.at(j);
            number_of_pins += netlistrow.count("(");
    	}
    }
    return number_of_pins;
}        

QStringList TinyCAD::getPosList(QStringList netlist){
    // Returns only a list of components in the net list
    QStringList components;
    QString pos, posline;
    for(int j = 0; j <= netlist.size() - 1 ; j++){
    	if( netlist.at(j).startsWith(QString("COMPONENT ") )){
            // Get the whole row with component information
            posline = netlist.at(j);
            //qDebug() << "Netlist component row: " << posline;
            // Get the pos name only
            pos = posline.section(" ", 1, 1);
            pos = pos.remove("'");
            //qDebug() << "Pos only: " << pos;
            // Append it to the list
            components.append(pos);
    	}
    }
    return components;    
}

QString TinyCAD::getProductNumberByPos(QTextEdit * console, QStringList TinyCADnetlist, QString pos){
    QString netlistrow, error_message;
    QString product_number = "";

    for(int j = 0; j <= TinyCADnetlist.size() - 1 ; j++){
    	if( TinyCADnetlist.at(j).startsWith(QString("COMPONENT ") )){
            if( TinyCADnetlist.at(j).contains("'" + pos + "'")){
                // This row contains the product number
                //consoleTextEdit->append(TinyCADnetlist.at(j));
                netlistrow = TinyCADnetlist.at(j);
                product_number = netlistrow.section("= ", 1, 1);
            }
    	}
    }
    if(product_number == ""){
        error_message = "Error: Product number for " + pos + " not found in netlist";
        console->append(error_message);
    }
    return product_number;
}

QString TinyCAD::getOptionByPos(QTextEdit * console, QStringList TinyCADnetlist, QString pos, QString optionName){
    QString netlistrow, error_message;
    QString product_number = "";

    for(int j = 0; j <= TinyCADnetlist.size() - 1 ; j++){
        if( TinyCADnetlist.at(j).startsWith(QString("COMPONENT ") )){
            if( TinyCADnetlist.at(j).contains("'" + pos + "'")){
                // This row contains the product number and the searched pos
                for (int k = j; k<=TinyCADnetlist.size() - 1;k++){
                    // Starting at the row after the one first found
                    // Is the row an "OPTION" row?
                    if(TinyCADnetlist.at(k).contains("OPTION '" + optionName +"'")){
                        QString tmpRow = TinyCADnetlist.at(k);
                        QStringList tmpList = tmpRow.split(" = ");
                        if(tmpList.size() > 1){
                            return tmpList.at(1); // returning the value of the option name
                        }
                        else{
                            return "";
                        }
                    }
                    if(TinyCADnetlist.size() > k){
                        if(TinyCADnetlist.at(k+1).contains("COMPONENT ")){
                            // Stop looking for option
                            return "";
                        }
                    }
                }
                //error_message = "Error: Option not found for " + pos;
                //console->append(error_message);
                return "";
            }
        }
    }
}

QStringList TinyCAD::getListOfPinsAndNetNames(QStringList TinyCADnetlist, QString pos){
    QStringList list_of_pins_and_net_names;
    QString netlist_row, net_name, pin_name;
    int number_of_pos_in_row;
    //qDebug() << "looking for pos:" << pos;
    for(int j = 0; j <= TinyCADnetlist.size() - 1 ; j++){
        if(TinyCADnetlist.at(j).startsWith(QString("NET "))){
            // This is a net list row
            if( TinyCADnetlist.at(j).contains("(" + pos + ",")){
                // This row contains the pos we are looking for
                // How many occurrences are there of that pos?
                netlist_row = TinyCADnetlist.at(j);
                //qDebug() << "Found a row that contains our pos:" << netlist_row;
                //qDebug() << "Netlist row:" << netlist_row << "Pos:" << pos;
                number_of_pos_in_row = netlist_row.count(pos + ",");
                //qDebug() << "This row comtains" << number_of_pos_in_row << "ocurrances ofpos";
                for(int k = 0; k < number_of_pos_in_row; k++){
                    //For every pos in the row, get pin and net name
                    net_name = netlist_row.section("'", 1, 1);
                    //qDebug() << "Net name:" << net_name;
                    pin_name = netlist_row.section(pos + ",", k+1, k+1);
                    //qDebug() << "Pin name 1:" << pin_name;
                    // Get the string part until a ) appears
                    pin_name = pin_name.section(")", 0, 0);
                    //qDebug() << "Pin name 2:" << pin_name;
                    //qDebug() << "  (" << pin_name << net_name << " )";
                    list_of_pins_and_net_names.append("  ( " + pin_name + " " + net_name + " )");
                }
            }
        }
    }
    return list_of_pins_and_net_names;
}
