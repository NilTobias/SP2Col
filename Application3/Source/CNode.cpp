#include "CNode.h"


CNode::CNode(void)
{
	next = NULL;
	prev = NULL;
}

CNode::CNode(Object NewObject)
{
	Data = NewObject;
	next = NULL;
	prev = NULL;
}

CNode::~CNode(void)
{
}