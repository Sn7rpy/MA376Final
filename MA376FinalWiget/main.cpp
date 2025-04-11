#include "MainWindow.h"
#include "graph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString filePath = "sateliteNodes.png";
    QString outPath = "sateliteBW.png";
    QString edgesPath = "edges.dat";
    QString nodesPath = "nodes.dat";
    QImage bwImg = processessGraphImg(filePath);
    if (QFile::exists(nodesPath)) {
        w.loadNodesFromFile(nodesPath);
    }
    else {
        bwImg.save(outPath);
        std::vector<QPointF> cntrList = findNodes(bwImg);
        w.addNodesFromPoints(cntrList);
    }

    w.resizeToImg(bwImg.size());
    w.setBackgroundImg(filePath);
    if (QFile::exists(edgesPath)) {
        //std::cout << "loading lines";
        w.loadLinesFromFile(edgesPath);
    }

    w.show();
    return a.exec();
}
