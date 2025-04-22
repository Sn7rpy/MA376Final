#include "MainWindow.h"
#include "graph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString filePath = "inclassSample.png";
    QString outPath = "inclassBW.png";
    QString edgesPath = "edgesClass.dat";
    QString nodesPath = "nodesClass.dat";
    QImage bwImg = processessGraphImg(filePath);

    w.getEdgeFile(edgesPath);
    w.getNodeFile(nodesPath);

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
