#pragma once

#include <QImage>
#include <QPointF>
#include <QQueue>
#include <vector>
#include <iostream>

QImage processessGraphImg(const QString& pathToImage);

std::vector<QPointF> findNodes(QImage inputImg);
