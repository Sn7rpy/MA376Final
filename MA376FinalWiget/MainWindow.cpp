#include "MainWindow.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <algorithm>
#include <QPushButton>
#include <QDataStream>
#include <QDebug>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);

    MainWindow::scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    QPushButton* saveButton = new QPushButton("Save Lines", this);
    saveButton->move(10, 15);
    saveButton->resize(100, 30);

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveLinesToFile);
    
    QGroupBox* modeGroup = new QGroupBox("Mode", this);
    modeGroup->move(1300, 15);
    modeGroup->resize(150, 70);

    QRadioButton* drawEdgeBtn = new QRadioButton("Draw Edges", modeGroup);
    QRadioButton* nameNodeBtn = new QRadioButton("Index Nodes", modeGroup);

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(drawEdgeBtn);
    vbox->addWidget(nameNodeBtn);
    vbox->addStretch(1);
    modeGroup->setLayout(vbox);

    drawEdgeBtn->setChecked(true);
    state = WindowState::DrawEdges;

    connect(drawEdgeBtn, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            state = WindowState::DrawEdges;
        }
        });

    connect(nameNodeBtn, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            state = WindowState::NameNode;
        }
        });

}

void MainWindow::addNodesFromPoints(const std::vector<QPointF>& points)
{
    for (const QPointF& pt : points) {
        NodeItem* node = new NodeItem(pt);
        scene->addItem(node);

        QPen highlightPen(Qt::white, 2);
        QPen defaultPen(Qt::black, 1);

        connect(node, &NodeItem::nodeClicked, this, [=](NodeItem* clickedNode) {
            if (!firstSelected) {
                firstSelected = clickedNode;
                firstSelected->setPen(highlightPen);
                if (state == WindowState::NameNode) {
                    bool ok;
                    QString inputText;
                    if (firstSelected->index != "") {
                        QString inputTitle("Rename Node");
                    }
                    else {
                        QString inputTitle("Name Node");
                    }
                    QString newIndex = QInputDialog::getText(this, inputText, "Name:", QLineEdit::Normal, firstSelected->getIndex(),&ok);

                    if (ok && !newIndex.isEmpty()) {
                        firstSelected->setIndex(newIndex);
                        firstSelected->setPen(defaultPen);
                        firstSelected = nullptr;
                    }

                }
            }
            else if (firstSelected != clickedNode) {
                if (state == WindowState::DrawEdges){
                    QLineF line(firstSelected->scenePos(), clickedNode->scenePos());
                    //bool ok;

                    //QString newIdx = QInputDialog::getText(this, "Name Edge", "Name:", QLineEdit::Normal, "", &ok);

                    //if (ok && !newIdx.isEmpty()) {
                    //    line.setIndex(newIdx);
                    //}
                
                    if (std::find(linesVct.begin(), linesVct.end(), line) == linesVct.end()) {
                        scene->addLine(line, QPen(Qt::blue, 2));
                        linesVct.push_back(line);
                        firstSelected->setPen(defaultPen);
                        firstSelected = clickedNode;
                        firstSelected->setPen(highlightPen);
                    }
                    else {
                        firstSelected->setPen(defaultPen);
                        firstSelected = nullptr;
                    }
                }
                else if(state == WindowState::NameNode){
                    firstSelected->setPen(defaultPen);
                    firstSelected = nullptr;
                }
                
                
            }
            else {
                firstSelected->setPen(defaultPen);
                firstSelected = nullptr;
            }
            
            });
    }

    

}

void MainWindow::resizeToImg(const QSizeF& size) {
    ui->graphicsView->setSceneRect(QRectF(QPointF(0,0),size));
    //ui->graphicsView->resize(size.width(), size.height());

}
void MainWindow::setBackgroundImg(const QString& filepath) {
    QPixmap bg(filepath);
    QGraphicsPixmapItem* bgItem = scene->addPixmap(bg);
    bgItem->setZValue(-1000);
    bgItem->setPos(0, 0);
}

void MainWindow::saveLinesToFile() {
    QFile file("lines.dat");
    file.open(QIODeviceBase::WriteOnly);

    QDataStream out(&file);
    out << static_cast<quint32>(linesVct.size());
    for (const QLineF& line : linesVct) {
        out << line;
    }
    file.close();
}

void MainWindow::loadLinesFromFile(const QString& fileName) {
    QFile file(fileName);
    file.open(QIODeviceBase::ReadOnly);

    QDataStream in(&file);
    quint32 count = 0;
    in >> count;

    for (quint32 i = 0; i < count; i++) {
        QLineF line;
        in >> line;

        scene->addLine(line, QPen(Qt::blue, 2));
        linesVct.push_back(line);
    }

    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


