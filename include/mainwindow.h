#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QScrollArea>
#include <QScrollArea>
#include <QTextBrowser>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MainWindow *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event);
    void magicTime();
    QScrollArea *s;
    QWidget *w ;
    QWidget *w1 ;
    QTextBrowser * text;

signals:
public slots:
    void ShowFIRST();
    void ShowFOLLOW();
    void ShowLR1();
    void ShowANALYZE();
    void ShowSEMANTICS();
    void ShowOBJECT();
    void ShowERROR();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
