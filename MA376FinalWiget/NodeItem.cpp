#include "NodeItem.h"
#include <QFont>


NodeItem::NodeItem(const QPointF& pos, qreal radius, const QBrush& color)
	:QGraphicsEllipseItem(-radius, -radius, 2 * radius, 2 * radius) {

	index = "";
	point = pos;
	setPos(pos);
	setBrush(color);
	setPen(QPen(Qt::black, 1));
	setFlag(QGraphicsItem::ItemIsSelectable);

	label = new QGraphicsTextItem(this);
	label->setPlainText("");
	label->setDefaultTextColor(Qt::black);
	QFont font;
	font.setPointSize(10);
	font.setBold(true);
	label->setFont(font);
	label->setPos(-radius/2,-radius/2);

}

void NodeItem::setIndex(const QString& newValue)
{
	index = newValue;
	label->setPlainText(newValue);
}

QString NodeItem::getIndex()
{
	return index;
}

QPointF NodeItem::getPoint()
{
	return point;
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	emit nodeClicked(this);
	QGraphicsEllipseItem::mousePressEvent(event);
}