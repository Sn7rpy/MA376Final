#include "NodeItem.h"
#include <QFont>
#include <QHash>


NodeItem::NodeItem(const QPointF& pos, qreal radius, const QBrush& color)
	:QGraphicsEllipseItem(-radius, -radius, 2 * radius, 2 * radius) {

	index = "";
	point = pos;
	hash = qHash(QPair<qreal, qreal>(point.x(), point.y()));

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

uint NodeItem::getHash()
{
	return hash;
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	emit nodeClicked(this);
	QGraphicsEllipseItem::mousePressEvent(event);
}