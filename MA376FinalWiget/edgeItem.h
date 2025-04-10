#pragma once
#include <QLine>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>

class edgeItem : public QObject, public QLineF
{
	Q_OBJECT

public:
	edgeItem(const QPointF& outNode, const QPointF& inNode);
	QString index;
	int weight;
	QPointF outNd,inNd;
	void setWeight(int& newValue);
	void setIndex(QString& idx);
	
};

