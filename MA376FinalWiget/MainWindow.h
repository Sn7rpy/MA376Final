#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include <QFile>
#include <QRadioButton>
#include "connStruct.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindowClass;
}
QT_END_NAMESPACE

enum class WindowState {
    DrawEdges,
    ChangeIndicies,
    DijkstraMode,
    DeleteEdges
};

struct predOut {
    size_t predNode;
    size_t predEdge;
};

struct dijOutput {
    predOut predecesor;
    double minPath;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
    

    void addNodesFromPoints(const std::vector<QPointF>& points);
    void resizeToImg(const QSizeF& size);
    void setBackgroundImg(const QString& filepath);
    void promptIndexChange();
    void handleClickedNode(NodeItem* clickedNode);
    void loadNodesFromFile(const QString& fileName = "nodes.dat");
    void loadLinesFromFile(const QString& fileName="edges.dat");
    void getNodeFile(const QString& fileName);
    void getEdgeFile(const QString& fileName);
    void setWeightsFromCSV(const QString& filepath);

    QHash<size_t, dijOutput> runDijkstras(NodeItem* startNode);
    std::vector<std::vector<double>> runFloydWar();



    ~MainWindow();

private slots:
    void saveFeaturesToFile();
    void onEdgeClicked(EdgeItem* edge);
    void loadWeights();
    void runDA();
    void runFW();

private:
    Ui::MainWindowClass *ui;
    QGraphicsScene* scene;
    NodeItem* firstSelected = nullptr;
    WindowState state;

    QString nodesFile;
    QString edgesFile;

    QHash<size_t, EdgeItem*> edgesMap;
    QHash<size_t, NodeItem*> nodesMap;
    QHash<QString, EdgeItem*> edgeIndexes;
    QHash<QString, NodeItem*> nodeIndexes;
    //stores the hash of the origin node as the key and a list of the hashes of the out nodes in a vector
    QMultiHash<size_t, connStruct> connections;

    QRadioButton* drawEdgeBtn;
    QRadioButton* changeIndexesBtn;
    QRadioButton* dijktraBtn;
    QRadioButton* deleteEdgesBtn;

    QHash<size_t, dijOutput> dijkstraResult;

    std::vector<std::vector<double>> fwResult;

};
