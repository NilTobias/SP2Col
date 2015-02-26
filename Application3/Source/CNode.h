#include "Object.h"
#pragma once

class CNode
{
public:
	CNode(void);
	CNode(Object NewObject);
	~CNode(void);

	Object Data;
	CNode* next;
	CNode* prev;
};