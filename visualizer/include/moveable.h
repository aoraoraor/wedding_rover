#ifndef _MOVEABLE_H_
#define _MOVEABLE_H_

#include <common.h>

class Moveable
{
public:
	Moveable();

	void move(Pos to);

protected:
	virtual void performMove(Pos to) = 0;
};


#endif // _MOVEABLE_H_

