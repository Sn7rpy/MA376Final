#include "MainWindow.h"
#include "graph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString filePath = "C:\\Users\\Diego\\Downloads\\sateliteNodes.png";
    QString outPath = "C:\\Users\\Diego\\Downloads\\sateliteBW.png";
    QString linesPath = "lines.dat";
    QImage bwImg = processessGraphImg(filePath);
    bwImg.save(outPath);
    std::vector<QPointF> cntrList = findNodes(bwImg);

    w.addNodesFromPoints(cntrList);
    w.resizeToImg(bwImg.size());
    w.setBackgroundImg(filePath);
    if (QFile::exists(linesPath)) {
        //std::cout << "loading lines";
        w.loadLinesFromFile(linesPath);
    }

    w.show();
    return a.exec();
}
