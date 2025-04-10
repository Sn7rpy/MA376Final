#include "NodeItem.h"


NodeItem::NodeItem(const QPointF& pos, qreal radius, const QBrush& color)
	:QGraphicsEllipseItem(-radius, -radius, 2 * radius, 2 * radius) {

	index;
	setPos(pos);
	setBrush(color);
	setPen(QPen(Qt::black, 1));
	setFlag(QGraphicsItem::ItemIsSelectable);

}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	emit nodeClicked(this);
	QGraphicsEllipseItem::mousePressEvent(event);
}