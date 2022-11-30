#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <windows.h>


class MainWindow;
class ImageCellGenerator;
class Connector: public QObject
{
    Q_OBJECT

public:
    void timers_connects(QTimer*, ImageCellGenerator*);
    void imageGenerator_connects(MainWindow*, ImageCellGenerator*);
};
class Disconnector: public QObject
{
    Q_OBJECT

public:
    void timers_disconnects(QTimer*, ImageCellGenerator*);
    void imageGenerator_disconnects(MainWindow*, ImageCellGenerator*);
};


class CellRules
{
public:
    virtual bool calculateCelluarPixel(bool *chekingCells) = 0;
};
class Rule30: public CellRules
{
public:
    bool calculateCelluarPixel(bool *chekingCells) override
    {
        if((chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
    }
};
class Rule110: public CellRules
{
public:
    bool calculateCelluarPixel(bool *chekingCells) override
    {
        if((chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
    }
};
class Rule120: public CellRules
{
public:
    bool calculateCelluarPixel(bool *chekingCells) override
    {
        if((chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
    }
};
class Rule225: public CellRules
{
public:
    bool calculateCelluarPixel(bool *chekingCells) override
    {
        if((chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return true;}
    }
};
class Rule184: public CellRules
{
public:
    bool calculateCelluarPixel(bool *chekingCells) override
    {
        if((chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (chekingCells[2])) {return true;}
        else if((!chekingCells[0]) && (chekingCells[1]) && (!chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (chekingCells[2])) {return false;}
        else if((!chekingCells[0]) && (!chekingCells[1]) && (!chekingCells[2])) {return false;}
    }
};

class ImageCellGenerator: public QObject
{
    Q_OBJECT

    int numRules = 5;
    int numberString = 0;
    int printRulesCounter = 0;
    int sizePickture =  128; //width
    QImage *image;
    CellRules *currentRull;
    QVector<CellRules*> rules;
    QVector<bool> cellPickture;
    QVector<bool> renderingPickture_buffer;

    void calculateStartImageState();

    void consolePaint();

    void renderingStringImage();

public:

    ImageCellGenerator();

    QImage *get_currentImage();

public slots:
    void paintCellRulles();

signals:
    void callRep();
};

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QTimer *timer;
    QRectF *target;
    QRectF *source;
    QThread *thread;
    Connector *connector;
    Disconnector *disconnector;
    ImageCellGenerator *imageGenerator;

    void paintEvent(QPaintEvent *);
    void settingsThread();
    void installWindowStartConfig();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void startCalculateCells();

public slots:
    void callRepraint();

private slots:
    void on_pushButton_clicked();
};
#endif
