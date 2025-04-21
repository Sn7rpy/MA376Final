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

    QPushButton* saveButton = new QPushButton("Save Graph", this); 
    saveButton->move(10, 15);
    saveButton->resize(100, 30);

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFeaturesToFile);
    
    QGroupBox* modeGroup = new QGroupBox("Mode", this);
    modeGroup->move(1300, 15);
    modeGroup->resize(150, 70);

    QRadioButton* drawEdgeBtn = new QRadioButton("Draw Edges", modeGroup);
    QRadioButton* changeIndexesBtn = new QRadioButton("Change Indicies", modeGroup);

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(drawEdgeBtn);
    vbox->addWidget(changeIndexesBtn);
    vbox->addStretch(1);
    modeGroup->setLayout(vbox);

    drawEdgeBtn->setChecked(true);
    state = WindowState::DrawEdges;

    connect(drawEdgeBtn, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            state = WindowState::DrawEdges;
        }
        });

    connect(changeIndexesBtn, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            state = WindowState::ChangeIndicies;
        }
        });

}

void MainWindow::addNodesFromPoints(const std::vector<QPointF>& points)
{
    QPen highlightPen(Qt::white, 2);
    QPen defaultPen(Qt::black, 1);

    for (const QPointF& pt : points) {
        NodeItem* node = new NodeItem(pt);
        nodesMap.insert(node->getHash(), node);
        scene->addItem(node);

        connect(node, &NodeItem::nodeClicked, this, [=](NodeItem* clickedNode) {
            if (!firstSelected) {
                firstSelected = clickedNode;
                firstSelected->setPen(highlightPen);
                if (state == WindowState::ChangeIndicies) {
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
                    EdgeItem* edge = new EdgeItem(QLineF(firstSelected->scenePos(), clickedNode->scenePos()));
                    
                
                    if (edgesMap.contains(edge->getHash())) {
                        scene->addItem(edge);
                        connect(edge, &EdgeItem::edgeClicked, this, &MainWindow::onEdgeClicked);
                        edgesMap.insert(edge->getHash(), edge);

                        connections.insert(firstSelected->getHash(),
                            {edge->getHash(),clickedNode->getHash(),0}
                        );

                        connections.insert(clickedNode->getHash(),
                            {edge->getHash(), firstSelected->getHash(),1}
                        );

                        firstSelected->setPen(defaultPen);
                        firstSelected = clickedNode;
                        firstSelected->setPen(highlightPen);
                    }
                    else {
                        firstSelected->setPen(defaultPen);
                        firstSelected = nullptr;
                    }
                }
                else if(state == WindowState::ChangeIndicies){
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

void MainWindow::loadNodesFromFile(const QString& fileName)
{
    QPen highlightPen(Qt::white, 2);
    QPen defaultPen(Qt::black, 1);

    QFile file(fileName);
    file.open(QIODeviceBase::ReadOnly);

    QDataStream in(&file);
    quint32 count = 0;
    in >> count;

    for (quint32 i = 0; i < count; i++) {
        QPointF point;
        QString index;
        in >> point;
        in >> index;

        NodeItem* node = new NodeItem(point);

        scene->addItem(node);
        node->setIndex(index);
        connect(node, &NodeItem::nodeClicked, this, [=](NodeItem* clickedNode) {
            if (!firstSelected) {
                firstSelected = clickedNode;
                firstSelected->setPen(highlightPen);
                if (state == WindowState::ChangeIndicies) {
                    bool ok;
                    QString inputText;
                    if (firstSelected->index != "") {
                        QString inputTitle("Rename Node");
                    }
                    else {
                        QString inputTitle("Name Node");
                    }
                    QString newIndex = QInputDialog::getText(this, inputText, "Name:", QLineEdit::Normal, firstSelected->getIndex(), &ok);

                    if (ok && !newIndex.isEmpty()) {
                        firstSelected->setIndex(newIndex);
                        firstSelected->setPen(defaultPen);
                        firstSelected = nullptr;
                    }

                }
            }
            else if (firstSelected != clickedNode) {
                if (state == WindowState::DrawEdges) {
                    EdgeItem* edge = new EdgeItem(QLineF(firstSelected->scenePos(), clickedNode->scenePos()));


                    if (edgesMap.contains(edge->getHash())) {
                        scene->addItem(edge);
                        connect(edge, &EdgeItem::edgeClicked, this, &MainWindow::onEdgeClicked);
                        edgesMap.insert(edge->getHash(), edge);
                        
                        connections.insert(firstSelected->getHash(),
                            { edge->getHash(),clickedNode->getHash(),0 }
                        );

                        connections.insert(clickedNode->getHash(),
                            { edge->getHash(), firstSelected->getHash(),1 }
                        );

                        firstSelected->setPen(defaultPen);
                        firstSelected = clickedNode;
                        firstSelected->setPen(highlightPen);
                    }
                    else {
                        firstSelected->setPen(defaultPen);
                        firstSelected = nullptr;
                    }
                }
                else if (state == WindowState::ChangeIndicies) {
                    firstSelected->setPen(defaultPen);
                    firstSelected = nullptr;
                }


            }
            else {
                firstSelected->setPen(defaultPen);
                firstSelected = nullptr;
            }

            });
        nodesMap.insert(node->getHash(),node);

    }

    file.close();
}

void MainWindow::saveFeaturesToFile() {
    QFile edgeFile("edges.dat");
    edgeFile.open(QIODeviceBase::WriteOnly);

    QDataStream out(&edgeFile);
    out << static_cast<quint32>(edgesMap.size());
    for (EdgeItem* edge : edgesMap.values()) {
        out << edge->getLine();
        out << edge->getIndex();
    }
    edgeFile.close();

    QFile nodeFile("nodes.dat");
    nodeFile.open(QIODeviceBase::WriteOnly);

    QDataStream out2(&nodeFile);
    out2 << static_cast<quint32>(nodesMap.size());
    for (NodeItem* node : nodesMap.values()) {
        out2 << node->getPoint();
        out2 << node->getIndex();
    }
    nodeFile.close();
}

void MainWindow::loadLinesFromFile(const QString& fileName) {
    QFile file(fileName);
    file.open(QIODeviceBase::ReadOnly);

    QDataStream in(&file);
    quint32 count = 0;
    in >> count;

    for (quint32 i = 0; i < count; i++) {
        QLineF line;
        QString index;
        in >> line;
        in >> index;

        EdgeItem* edge = new EdgeItem(line, index);
        scene->addItem(edge);
        connect(edge, &EdgeItem::edgeClicked, this, &MainWindow::onEdgeClicked);
        edgesMap.insert(edge->getHash(),edge);
        connections.insert(edge->getHashP1(),
            { edge->getHash(), edge->getHashP2(),0 }
        );
        connections.insert(edge->getHashP2(), 
            { edge->getHash(), edge->getHashP1(), 1 }
        );

    }

    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onEdgeClicked(EdgeItem* edge)
{
    if (state == WindowState::ChangeIndicies) {
        bool ok;

        QString newIdx = QInputDialog::getText(this, "Name Edge", "Name:", QLineEdit::Normal, edge->getIndex(), &ok);

        if (ok && !newIdx.isEmpty()) {
            edge->setIndex(newIdx);
        }
    }
}


