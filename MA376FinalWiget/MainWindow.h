#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include <QFile>
#include "connStruct.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindowClass;
}
QT_END_NAMESPACE

enum class WindowState {
    DrawEdges,
    ChangeIndicies
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
    QHash<uint, EdgeItem*> edgesMap;
    QHash<uint, NodeItem*> nodesMap;
    //stores the hash of the origin node as the key and a list of the hashes of the out nodes in a vector
    QMultiHash<uint, connStruct> connections;

    void addNodesFromPoints(const std::vector<QPointF>& points);
    void resizeToImg(const QSizeF& size);
    void setBackgroundImg(const QString& filepath);
    void loadNodesFromFile(const QString& fileName = "nodes.dat");
    void loadLinesFromFile(const QString& fileName="lines.dat");



    ~MainWindow();

private slots:
    void saveFeaturesToFile();
    void onEdgeClicked(EdgeItem* edge);

private:
    Ui::MainWindowClass *ui;
    QGraphicsScene* scene;
    NodeItem* firstSelected = nullptr;
    WindowState state;

};
