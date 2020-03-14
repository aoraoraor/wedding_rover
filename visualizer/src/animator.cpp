#include "animator.h""

Animator::Animator()
	: 
	m_isPlayActive(false), 
	m_isGroomActive(false),
	m_roverMover(0),
	m_bachelorMover(0),
	m_uiTextRef(0),
	m_intersectionIndex(0)
{
	m_updateTimer = new QTimer(this);
	m_updateTimer->setInterval(0);
	connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateLoop()));
}

void Animator::setupUITextRef(QObject* textRef)
{
	m_uiTextRef = textRef;
}

void Animator::setupAnimation(Moveable* roverModel, Path roverPath, Moveable* bachelorModel, Path bachelorPath)
{
	Path totalPath;
	totalPath.insert(totalPath.end(), roverPath.begin(), roverPath.end());
	totalPath.insert(totalPath.end(), bachelorPath.begin(), bachelorPath.end());

	m_intersectionIndex = roverPath.size() - 1;

	m_roverMover = new ModelMover(roverModel, totalPath);
	m_bachelorMover = new ModelMover(bachelorModel, bachelorPath);
	m_updateTimer->start();
}

void Animator::initUIText()
{
	if (!m_uiTextRef) return;
	m_uiTextRef->setProperty("roverXYText", QVariant(QString("%1,%2").arg(QString::number(ROVER_X), QString::number(ROVER_Y))));
	m_uiTextRef->setProperty("bachelorXYText", QVariant(QString("%1,%2").arg(QString::number(BACHELOR_X), QString::number(BACHELOR_Y))));
	m_uiTextRef->setProperty("weddingXYText", QVariant(QString("%1,%2").arg(QString::number(WEDDING_X), QString::number(WEDDING_Y))));
	m_uiTextRef->setProperty("roverStepsText", 0);
	m_uiTextRef->setProperty("bachelorStepsText", 0);
}

void Animator::updateUIText()
{
	if (!m_uiTextRef || !m_bachelorMover || !m_roverMover) return;

	m_uiTextRef->setProperty("roverXYText",
		QVariant(QString("%1,%2")
			.arg(
				QString::number(m_roverMover->currentPos().x), 
				QString::number(m_roverMover->currentPos().y))));

	m_uiTextRef->setProperty("bachelorXYText",
		QVariant(QString("%1,%2")
			.arg(
				QString::number(m_bachelorMover->currentPos().x),
				QString::number(m_bachelorMover->currentPos().y))));

	m_uiTextRef->setProperty("roverStepsText", m_roverMover->currentStep());
	m_uiTextRef->setProperty("bachelorStepsText", m_bachelorMover->currentStep());
}

void Animator::updateLoop()
{
	if (!m_isPlayActive || !m_bachelorMover || !m_roverMover) return;

	if (m_isGroomActive)
		m_bachelorMover->moveFwd();

	if (m_roverMover->currentPos().x == m_bachelorMover->currentPos().x
		&& m_roverMover->currentPos().y == m_bachelorMover->currentPos().y)
	{
		m_isGroomActive = true;
	}

	m_roverMover->moveFwd();

	updateUIText();
}

void Animator::togglePlayPause()
{
	m_isPlayActive = !m_isPlayActive;
}

void Animator::stop()
{
	m_isPlayActive = false;
	m_isGroomActive = false;
	if (m_roverMover) m_roverMover->reset();
	if(m_bachelorMover) m_bachelorMover->reset();

	updateUIText();
}

void Animator::changeSpeed(int val)
{
	m_updateTimer->setInterval(500 - val);
}

void Animator::fastForward()
{
	if (!m_bachelorMover || !m_roverMover) return;

	m_roverMover->fastFWd(FF_STEPS);
	if(m_isGroomActive)
		m_bachelorMover->fastFWd(FF_STEPS);
	else
	{
		int stepDiff = m_roverMover->currentStep() - m_intersectionIndex;
		if (stepDiff > 0)
		{
			if(stepDiff > m_bachelorMover->currentStep())
				m_bachelorMover->fastFWd(stepDiff);
			else
				m_bachelorMover->fastFWd(FF_STEPS);
			m_isGroomActive = true;
		}
	}
}

Animator::~Animator()
{
	disconnect(this);

	if (m_updateTimer)
	{
		m_updateTimer->stop();
		m_updateTimer->deleteLater();
	}

	if (m_roverMover)
		delete m_roverMover;

	if (m_bachelorMover)
		delete m_bachelorMover;
}