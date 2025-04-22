#pragma once
#include <QLine>
#include <QGraphicsLineItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QGraphicsPolygonItem>

struct weightStruct {
	int timeForw;
	int timeBack;
	int scene;
	bool inside;
	bool cover;
	double distance;
};

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
	
	weightStruct weights;

	QLineF lineC;
	size_t hash;
	size_t hashP1;
	size_t hashP2;


	void setWeights(weightStruct& newValues);
	void setIndex(QString& idx);
	QString getIndex();
	QLineF getLine();
	size_t getHash();
	size_t getHashP1();
	size_t getHashP2();

	int getTimeWeight(bool direction);

signals:
	void edgeClicked(EdgeItem* edge);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	
};

