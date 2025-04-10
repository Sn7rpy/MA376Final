#include "EdgeItem.h"
#include <QFont>



EdgeItem::EdgeItem(const QLineF& line, const QString& idx, QGraphicsItem* parent) : QGraphicsLineItem(line,parent)
{
	index = idx;
	weight = 0;
	lineC = line;

	
	setPen(QPen(Qt::blue, 2));
	setFlag(QGraphicsItem::ItemIsSelectable);

	label = new QGraphicsTextItem(this);
	label->setPlainText(index);
	label->setDefaultTextColor(Qt::black);
	QFont font;
	font.setPointSize(10);
	font.setItalic(true);
	label->setFont(font);
	label->setPos(line.center());
}

void EdgeItem::setWeight(int& newValue)
{
	weight = newValue;
}

void EdgeItem::setIndex(QString& idx)
{
	index = idx;
	label->setPlainText(idx);
}

QString EdgeItem::getIndex()
{
	return index;
}

QLineF EdgeItem::getLine()
{
	return lineC;
}

void EdgeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsLineItem::mousePressEvent(event);
	emit edgeClicked(this);
}
