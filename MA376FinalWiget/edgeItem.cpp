#include "EdgeItem.h"
#include <QFont>



EdgeItem::EdgeItem(const QLineF& line, const QString& idx, QGraphicsItem* parent) : QGraphicsLineItem(line,parent)
{
	index = idx;
	lineC = line;
	weights = { 0,0,0,0,0,0 };

	hashP1 = qHash(QPair<qreal, qreal>(
		lineC.p1().x(), lineC.p1().y()
	));
	hashP2 = qHash(QPair<qreal, qreal>(
		lineC.p2().x(), lineC.p2().y()
	));

	hash = qHash(QPair<size_t,size_t>(hashP1,hashP2));

	
	setPen(QPen(Qt::blue, 2));
	setFlag(QGraphicsItem::ItemIsSelectable);

	label = new QGraphicsTextItem(this);
	label->setPlainText(index);
	label->setDefaultTextColor(Qt::black);
	QFont font;
	font.setPointSize(6);
	font.setItalic(true);
	label->setFont(font);
	label->setPos(line.center());

	arrow = new QGraphicsPolygonItem(this);
	arrow->setBrush(Qt::blue);

	arrowSize = 5.0;
	triangle << QPointF(0, 0) 
		<< QPointF(-arrowSize, arrowSize / 2) 
		<< QPointF(-arrowSize, -arrowSize / 2);
	arrow->setPolygon(triangle);
	arrow->setPos(lineC.center());
	arrow->setRotation(lineC.angleTo(QLine(QPoint(0,0),QPoint(1,0))));

}

void EdgeItem::setWeights(weightStruct& newValues)
{
	weights = newValues;
	arrow->setBrush(Qt::green);
}

void EdgeItem::setIndex(QString& idx)
{
	index = idx;
	label->setPlainText(idx);
	setPen(QPen(Qt::cyan, 1));
	arrow->setBrush(Qt::cyan);
}

QString EdgeItem::getIndex()
{
	return index;
}

QLineF EdgeItem::getLine()
{
	return lineC;
}

size_t EdgeItem::getHash()
{
	return hash;
}

size_t EdgeItem::getHashP1()
{
	return hashP1;
}

size_t EdgeItem::getHashP2()
{
	return hashP2;
}

int EdgeItem::getTimeWeight(bool direction)
{
	if (direction) {
		return weights.timeBack;
	}
	else {
		return weights.timeForw;
	}
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsLineItem::mousePressEvent(event);
	emit edgeClicked(this);
}
