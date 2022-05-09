#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "../include/PARSER/PARSER.h"
#include "../include/PARSER/LEXICAL.h"
#include "../include/SEMANTICS/WTITEM.h"
#include "../include/SEMANTICS/CODE.h"
#include "../include/SEMANTICS/OBJECT.h"
#include <QPainter>
vector<POINT> paintPoints;
vector<QLine>paintLines;
extern PARSER grammaranalyser;
extern LEXICAL lexical;
extern OBJECT object;
QString strFIRST;
QString strFOLLOW;
QString strLR1;
QString strSTACK;
QString strSEMANTICES;
QString strERROR;
QString strOBJECT;
#include <QBoxLayout>
#include <QPushButton>
MainWindow::MainWindow(MainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    s = new QScrollArea(this);
    s->setGeometry(0, 0, 800, 1000);
    QPushButton *FIRST = new QPushButton("FIRST");
    QPushButton *FOLLOW = new QPushButton("FOLLOW");
    QPushButton *LR1 = new QPushButton("LR1");
    QPushButton *ANALYZE = new QPushButton("ANALYZE");
    QPushButton *SEMANTICS = new QPushButton("SEMANTICES");
    QPushButton *OBJECT = new QPushButton("OBJECTCODE");
    QPushButton *ERROR = new QPushButton("ERROR");

    text = new QTextBrowser();
    QHBoxLayout * hlayout= new QHBoxLayout;
    hlayout->addWidget(FIRST,0,Qt::AlignTop);
    hlayout->addWidget(FOLLOW,0,Qt::AlignTop);
    hlayout->addWidget(LR1,0,Qt::AlignTop);
    hlayout->addWidget(ANALYZE,0,Qt::AlignTop);
    hlayout->addWidget(SEMANTICS,0,Qt::AlignTop);
    hlayout->addWidget(OBJECT,0,Qt::AlignTop);
    hlayout->addWidget(ERROR,0,Qt::AlignTop);

    QVBoxLayout * vlayout= new QVBoxLayout;
    vlayout->addItem(hlayout);
    vlayout->addWidget(text);
    w1 = new QWidget();

    w = new QWidget(s);

    s->setWidget(w);
    w->setGeometry(0, 0, 10000,10000 );/**/
    w1->setGeometry(0, 0, 800, 1000);
    w1->setLayout(vlayout);

    w->installEventFilter(this);
    w1->show();

    //connect函数
    strFIRST=QString::fromStdString(grammaranalyser.PrintFirst());
    connect(FIRST,SIGNAL(clicked()),this,SLOT(ShowFIRST()));
    strFOLLOW=QString::fromStdString(grammaranalyser.PrintFollow());
    connect(FOLLOW,SIGNAL(clicked()),this,SLOT(ShowFOLLOW()));
    strLR1=QString::fromStdString(grammaranalyser.PrintLR1Table());
    connect(LR1,SIGNAL(clicked()),this,SLOT(ShowLR1()));
    strSTACK=QString::fromStdString(grammaranalyser.PrintStack(lexical.GetData()));
    connect(ANALYZE,SIGNAL(clicked()),this,SLOT(ShowANALYZE()));
    strSEMANTICES=QString::fromStdString(grammaranalyser.PrintSemantics());
    connect(SEMANTICS,SIGNAL(clicked()),this,SLOT(ShowSEMANTICS()));
    strOBJECT=QString::fromStdString(object.PrintCode());
    connect(OBJECT,SIGNAL(clicked()),this,SLOT(ShowOBJECT()));
    strERROR=QString::fromStdString(grammaranalyser.PrintError()+"\n"+object.PrintERROR());
    connect(ERROR,SIGNAL(clicked()),this,SLOT(ShowERROR()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == w && event->type() == QEvent::Paint)
    {
        magicTime();
    }
    return QWidget::eventFilter(obj,event);
}
void MainWindow::magicTime()
{
    QPainter painter(w);

    QPen pen(Qt::SolidLine);
    pen.setWidth(1);//粗细
    pen.setColor(Qt::black);//颜色
    painter.setPen(pen);

    paintPoints=grammaranalyser.PrintTreePoints();
    paintLines=grammaranalyser.PrintTreeLines();

    for(int i=0;i<(int)paintPoints.size();i++){
        QPoint p;
        p.setX(paintPoints[i].pos.x()*100+100);
        p.setY(paintPoints[i].pos.y()*100+100);
        painter.drawEllipse(p,25,25);
        QPoint textp;
        textp.setX(p.x()-10);
        textp.setY(p.y()+10);
        painter.drawText(textp,QString::fromStdString(paintPoints[i].content));
        if(i!=(int)paintPoints.size()-1){
            QLine l;
            l.setP1(QPoint(paintLines[i].p1().x()*100+100,paintLines[i].p1().y()*100+125));
            l.setP2(QPoint(paintLines[i].p2().x()*100+100,paintLines[i].p2().y()*100+75));
            painter.drawLine(l);
        }
    }
}
void MainWindow::ShowFIRST(){
    //清除结果区域前面缓存的结果
    text->clear();
    //将输出的内容用append输出到TextBrowser
    text->append(strFIRST);
}

void MainWindow::ShowFOLLOW(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strFOLLOW);
}

void MainWindow::ShowLR1(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strLR1);
}

void MainWindow::ShowANALYZE(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strSTACK);
}

void MainWindow::ShowSEMANTICS(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strSEMANTICES);
}
void MainWindow::ShowOBJECT(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strOBJECT);
}
void MainWindow::ShowERROR(){
    //清除结果区域前面缓存的结果
    text->clear();

    //将输出的内容用append输出到TextBrowser
    text->append(strERROR);
}
