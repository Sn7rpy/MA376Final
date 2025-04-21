#pragma once
#include <QLine>
#include <QGraphicsLineItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGraphicsPolygonItem>

class EdgeItem : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

public:
	EdgeItem(const QLineF& line, const QString& idx = "", QGraphicsItem* parent = nullptr);

	QGraphicsTextItem* label = nullptr;
	QGraphicsPolygonItem* arrow = nullptr;

	double arrowSize;
	QPolygonF triangle;


	QString index;
	int weightForw;
	int weightBack;
	QLineF lineC;
	uint hash;
	uint hashP1;
	uint hashP2;


	void setWeight(int& newValue);
	void setIndex(QString& idx);
	QString getIndex();
	QLineF getLine();
	uint getHash();
	uint getHashP1();
	uint getHashP2();

	int getWeight(bool direction);

signals:
	void edgeClicked(EdgeItem* edge);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	
};

