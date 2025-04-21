#pragma once

#include <QImage>
#include <QPointF>
#include <QQueue>
#include <vector>
#include <iostream>
#include <tuple>
#include "EdgeItem.h"
#include "NodeItem.h"


QImage processessGraphImg(const QString& pathToImage);

std::vector<QPointF> findNodes(QImage inputImg);

std::vector<std::tuple<std::string, std::string, double>> runDijkstras(NodeItem* startNode, QHash<uint, EdgeItem*> edgeMp, QHash<uint, NodeItem*> nodeMp, QMultiHash<uint, std::tuple<uint, uint>> connMp);
