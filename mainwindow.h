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
    virtual quint8 calculateCelluarPixel(quint8) = 0;
};
class Rule30: public CellRules
{
public:
    quint8 calculateCelluarPixel(quint8 pixel) override
    {
        return (pixel == 0) ? 0 :
                    (pixel == 1) ? 1 :
                    (pixel == 2) ? 1 :
                    (pixel == 3) ? 1 :
                    (pixel == 4) ? 1 :
                    (pixel == 5) ? 0 :
                    (pixel == 6) ? 0 :
                    (pixel == 7) ? 0: 0;
    }
};
class Rule110: public CellRules
{
public:
    quint8 calculateCelluarPixel(quint8 pixel) override
    {
        return (pixel == 0) ? 0 :
                    (pixel == 1) ? 1 :
                    (pixel == 2) ? 1 :
                    (pixel == 3) ? 1 :
                    (pixel == 4) ? 0 :
                    (pixel == 5) ? 1 :
                    (pixel == 6) ? 1 :
                    (pixel == 7) ? 0: 0;
    }
};
class Rule120: public CellRules
{
public:
    quint8 calculateCelluarPixel(quint8 pixel) override
    {
        return (pixel == 0) ? 0 :
                    (pixel == 1) ? 0 :
                    (pixel == 2) ? 0 :
                    (pixel == 3) ? 1 :
                    (pixel == 4) ? 1 :
                    (pixel == 5) ? 1 :
                    (pixel == 6) ? 1 :
                    (pixel == 7) ? 0: 0;
    }
};
class Rule184: public CellRules
{
public:
    quint8 calculateCelluarPixel(quint8 pixel) override
    {
        return (pixel == 0) ? 0 :
                    (pixel == 1) ? 0 :
                    (pixel == 2) ? 0 :
                    (pixel == 3) ? 1 :
                    (pixel == 4) ? 1 :
                    (pixel == 5) ? 1 :
                    (pixel == 6) ? 0 :
                    (pixel == 7) ? 1 : 0;
    }
};
class Rule225: public CellRules
{
public:
    quint8 calculateCelluarPixel(quint8 pixel) override
    {
        return (pixel == 0) ? 1 :
                    (pixel == 1) ? 0 :
                    (pixel == 2) ? 0 :
                    (pixel == 3) ? 0 :
                    (pixel == 4) ? 0 :
                    (pixel == 5) ? 1 :
                    (pixel == 6) ? 1 :
                    (pixel == 7) ? 1 : 1;
    }
};

class ImageCellGenerator: public QObject
{
    Q_OBJECT

    size_t numRules = 5;
    size_t numberString = 0;
    size_t printRulesCounter = 0;
    size_t numSegmentsPickture =  18;
    size_t targetStrings =  numSegmentsPickture * 8;
    quint16 checkingSegmentBuffeer = 0;
    QImage *image;
    CellRules *currentRule;
    QVector<CellRules*> rules;
    QVector<quint8> cellPickture;
    QVector<quint8> renderingPickture_buffer;

    void calculateStartImageState();
    void installImageConfig();
    void currentRulesControl();

    void consolePaint();
    void renderingStringImage();
    quint8 calculateCheckingPixels(quint16 *);

public:
    ImageCellGenerator();
    ~ImageCellGenerator();

    QImage* get_currentImage();

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
    void on_pushButton_StartPainting_clicked();
};
#endif
