#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include <qstring.h>
#include <qtimer.h>
#include <qvariant.h>

#include "common.h"
#include "moveable.h"
#include "modelmover.h"

#define FF_STEPS 100

class Animator: public QObject
{
	Q_OBJECT

public:
	Animator();
	~Animator();
	void setupUITextRef(QObject* textRef);
	void setupAnimation(Moveable* roverModel, Path roverPath, Moveable* bachelorModel, Path bachelorPath);
	void initUIText();
	void updateUIText();
	Q_INVOKABLE void togglePlayPause();
	Q_INVOKABLE void stop();
	Q_INVOKABLE void changeSpeed(int val);
	Q_INVOKABLE void fastForward();

public Q_SLOTS:
	void updateLoop();

private:
	bool m_isPlayActive;
	bool m_isGroomActive;
	int m_intersectionIndex;
	QTimer* m_updateTimer;
	ModelMover* m_roverMover;
	ModelMover* m_bachelorMover;
	QObject* m_uiTextRef;
};

#endif // _ANIMATOR_H_