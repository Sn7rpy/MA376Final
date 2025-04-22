#pragma once

#include <QImage>
#include <QPointF>
#include <QQueue>
#include <vector>
#include <iostream>
#include <tuple>
#include "EdgeItem.h"
#include "NodeItem.h"
#include "connStruct.h"


QImage processessGraphImg(const QString& pathToImage);

std::vector<QPointF> findNodes(QImage inputImg);


