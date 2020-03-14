#ifndef _MODEL_MOVER_H_
#define _MODEL_MOVER_H_

#include "common.h"
#include "moveable.h"

#include <qmutex.h>

using namespace std;

class ModelMover
{
private:
	Moveable* m_moveable;
	Path m_path;
	int m_curPathIndex;
	QMutex m_mutex;

public:
	ModelMover(Moveable* moveable, Path path);
	void moveFwd();
	void fastFWd(int steps);
	void reset();
	Pos currentPos();
	int currentStep();
};


#endif // _MODEL_MOVER_H_