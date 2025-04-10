#pragma once
#include <QLine>
#include <QGraphicsLineItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>

class EdgeItem : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

public:
	EdgeItem(const QLineF& line, const QString& idx = "", QGraphicsItem* parent = nullptr);

	QGraphicsTextItem* label = nullptr;

	QString index;
	int weight;
	QLineF lineC;


	void setWeight(int& newValue);
	void setIndex(QString& idx);
	QString getIndex();
	QLineF getLine();

signals:
	void edgeClicked(EdgeItem* edge);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	
};

