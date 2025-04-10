#include "edgeItem.h"

edgeItem::edgeItem(const QPointF& outNode, const QPointF& inNode)
	:QLineF(outNode,inNode)
{
	outNd = outNode;
	inNd = inNode;

}

void edgeItem::setWeight(int& newValue)
{
	weight = newValue;
}

void edgeItem::setIndex(QString& idx)
{
	index = idx;
}
