#pragma once
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QColor>

class NodeItem : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
public:
	NodeItem(const QPointF& pos, qreal radius = 5.0, const QBrush& color = Qt::red);
	QString index;
	QPointF point;
	QGraphicsTextItem* label = nullptr;

	void setIndex(const QString& newValue);
	QString getIndex();
	QPointF getPoint();

signals:
	void nodeClicked(NodeItem* self);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

};

