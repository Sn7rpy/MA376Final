#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "NodeItem.h"
#include "edgeItem.h"
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindowClass;
}
QT_END_NAMESPACE

enum class WindowState {
    DrawEdges,
    NameNode
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void addNodesFromPoints(const std::vector<QPointF>& points);
    void resizeToImg(const QSizeF& size);
    void setBackgroundImg(const QString& filepath);
    std::vector<QLineF> linesVct;
    void loadLinesFromFile(const QString& fileName="lines.dat");


    ~MainWindow();

private slots:
    void saveLinesToFile();

private:
    Ui::MainWindowClass *ui;
    QGraphicsScene* scene;
    NodeItem* firstSelected = nullptr;
    WindowState state;

};
