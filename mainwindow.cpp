#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer;
    thread = new QThread;
    connector = new Connector;
    disconnector = new Disconnector;
    imageGenerator = new ImageCellGenerator;

    target = new QRectF(10.0, 20.0, 700.0, 570.0);
    source = new QRectF(0.0, 0.0, 150.0, 150.0);

    connector->timers_connects(timer, imageGenerator);
    connector->imageGenerator_connects(this, imageGenerator);

    settingsThread();

    installWindowStartConfig();
    setWindowTitle("Wolfram code renderer");
}
MainWindow::~MainWindow()
{
    disconnector->timers_disconnects(timer, imageGenerator);
    disconnector->imageGenerator_disconnects(this, imageGenerator);

    delete imageGenerator;
    delete ui;
}
void MainWindow::settingsThread()
{
    imageGenerator->moveToThread(thread);
    thread->start();
}
void MainWindow::installWindowStartConfig()
{
    for(int i = 0; i < imageGenerator->get_currentImage()->height(); i++)
        for(int j = 0; j < imageGenerator->get_currentImage()->width(); j++)
            imageGenerator->get_currentImage()->setPixel(i, j, 0);

    QPainter painter(this);
    painter.drawImage(*target, *imageGenerator->get_currentImage(), *source);
}

void MainWindow::on_pushButton_StartPainting_clicked()
{
    emit startCalculateCells();
    ui->pushButton_StartPainting->setCheckable(false);
    ui->pushButton_StartPainting->setCheckable(false);
}

void MainWindow::callRepraint()
{
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    painter.drawImage(*target, *imageGenerator->get_currentImage(), *source);
}


ImageCellGenerator::ImageCellGenerator()
{
    calculateStartImageState();
    installImageConfig();
}
ImageCellGenerator::~ImageCellGenerator()
{

}

void ImageCellGenerator::calculateStartImageState()
{
    renderingPickture_buffer.resize(numSegmentsPickture);
    cellPickture.resize(numSegmentsPickture);
    renderingPickture_buffer.fill(0);
    cellPickture.fill(0);

    cellPickture[numSegmentsPickture - 1] = 1;
}
void ImageCellGenerator::installImageConfig()
{
    image = new QImage(144, 144, QImage::Format_Indexed8);
    QRgb value;

    //value = qRgb(220, 220, 220);
    value = qRgb(120, 140, 250);
    image->setColor(0, value);

    //value = qRgb(120, 120, 120);
    value = qRgb(80, 250, 51);
    image->setColor(1, value);
}

void ImageCellGenerator::consolePaint()
{
    for(size_t c = 0; c < numSegmentsPickture; c++)
    {
        for(int s = 7, b = 0; s >= 0; --s, b++)
        {
            size_t pixel = (((cellPickture[c] & (1 << s)) != 0) ? 1 : 0);
            image->setPixel(((c * 8) + b), numberString, pixel);
        }
    }
    emit callRep();
}
void ImageCellGenerator::renderingStringImage()
{
    for(size_t i = 0; i < numSegmentsPickture; i++)
    {
        checkingSegmentBuffeer = 0;
        size_t indexBeforSegment = ((i == 0) ? (numSegmentsPickture - 1) : (i - 1));
        size_t indexAfterSegment = (i == (numSegmentsPickture - 1)) ? 0 : (i + 1);

        checkingSegmentBuffeer = (cellPickture[indexBeforSegment] << 9);
        checkingSegmentBuffeer |= (cellPickture[i] << 1);
        checkingSegmentBuffeer |= quint8(cellPickture[indexAfterSegment] >> 7);

        renderingPickture_buffer[i] = calculateCheckingPixels(&checkingSegmentBuffeer);
    }

    for(size_t i = 0; i < numSegmentsPickture; i++) cellPickture[i] = renderingPickture_buffer[i];
}
quint8 ImageCellGenerator::calculateCheckingPixels(quint16 *checkingSegmentBuffeer)
{
    quint8 checkingPixels = 0;
    quint8 segmentPixelsBuffeer = 0;

    for(quint8 j = 8; j > 0; j--)
    {
        checkingPixels = 0;

        checkingPixels = ((((*checkingSegmentBuffeer & (1 << (j + 1))) == (1 << (j + 1))) ? 1 : 0) << 2);
        checkingPixels |= ((((*checkingSegmentBuffeer & (1 << j)) == (1 << j)) ? 1 : 0) << 1);
        checkingPixels |= (((*checkingSegmentBuffeer & (1 << (j - 1))) == (1 << (j - 1))) ? 1 : 0);

        segmentPixelsBuffeer <<= 1;
        segmentPixelsBuffeer |= currentRule->calculateCelluarPixel(checkingPixels);
    }

    return segmentPixelsBuffeer;
}

void ImageCellGenerator::paintCellRulles()
{
    rules.append(new Rule110);
    rules.append(new Rule30);
    rules.append(new Rule225);
    rules.append(new Rule184);
    rules.append(new Rule120);
    currentRule = rules.at(printRulesCounter);

    for(int i = 0; i != -1; i++)
    {
        Sleep(70);
        renderingStringImage();
        consolePaint();
        numberString++;

        currentRulesControl();
    }
}
void ImageCellGenerator::currentRulesControl()
{
    if(numberString == targetStrings)
    {
        numberString = 0;
        printRulesCounter++;

        if(printRulesCounter == numRules)
            printRulesCounter = 0;
        currentRule = rules.at(printRulesCounter);
    }
}


QImage *ImageCellGenerator::get_currentImage()
{
    return image;
}


void Connector::timers_connects(QTimer *currentTimer, ImageCellGenerator *currentImgGen)
{
    connect(currentTimer, SIGNAL(timeout()), currentImgGen, SLOT(paintCellRulles()));
}
void Connector::imageGenerator_connects(MainWindow *currentUI, ImageCellGenerator *currentImgGen)
{
    connect(currentUI, SIGNAL(startCalculateCells()), currentImgGen, SLOT(paintCellRulles()));
    connect(currentImgGen, SIGNAL(callRep()), currentUI, SLOT(callRepraint()));
}

void Disconnector::timers_disconnects(QTimer *currentTimer, ImageCellGenerator *currentImgGen)
{
    disconnect(currentTimer, SIGNAL(timeout()), currentImgGen, SLOT(paintCellRulles()));
}
void Disconnector::imageGenerator_disconnects(MainWindow *currentUI, ImageCellGenerator *currentImgGen)
{
    disconnect(currentUI, SIGNAL(startCalculateCells()), currentImgGen, SLOT(paintCellRulles()));
    disconnect(currentImgGen, SIGNAL(callRep()), currentUI, SLOT(callRepraint()));
}
