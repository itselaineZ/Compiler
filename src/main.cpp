#include "../include/mainwindow.h"
#include "../include/ourclass.h"
#include "../include/base.h"
#include "outcpp.cpp"

#include <QApplication>

PARSER grammaranalyser;
LEXICAL lexical;
OBJECT object;

int main(int argc, char *argv[])
{
    grammaranalyser.Begin("D:\\Course\\Grammar\\gram\\src\\grammar3.txt");
    //grammaranalyser.Begin("D:\\complier\\parser\\src\\grammar1.txt");

    lexical.Begin("D:\\Course\\Grammar\\gram\\src\\data3.txt");
    //lexical.TestBegin("D:\\complier\\parser\\src\\data.txt");
    //lexical.GetData().Print();
    if(grammaranalyser.Analyse(lexical.GetData())){
        //grammaranalyser.sematics->Print();
        cout<<"Successfully identified\n";
    }
    else{
        cout<<"Can not identify\n";
    }
    grammaranalyser.PrintTreeArray();
    object = OBJECT(grammaranalyser.GetSymbol());
    object.GenObjectCode();
    object.Print();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();/**/
    return 0;
}
