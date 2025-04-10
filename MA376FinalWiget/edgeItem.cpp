#include "edgeItem.h"

edgeItem::edgeItem(const QPointF& outNode, const QPointF& inNode)
	:QLineF(outNode,inNode)
{
	index;
	weight;
}
