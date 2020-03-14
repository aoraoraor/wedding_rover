#include "modelmover.h""

ModelMover::ModelMover(Moveable* moveable, Path path)
	: m_moveable(moveable), m_path(path), m_curPathIndex(0)
{
}

Pos ModelMover::currentPos()
{
	return m_path[m_curPathIndex];
}

int ModelMover::currentStep()
{
	return m_curPathIndex;
}

void ModelMover::moveFwd()
{
	m_mutex.lock();
	if (m_curPathIndex >= m_path.size() - 1)
	{
		m_mutex.unlock();
		return;
	}

	m_curPathIndex++;
	m_mutex.unlock();

	m_moveable->move(m_path[m_curPathIndex]);
}

void ModelMover::fastFWd(int steps)
{
	m_mutex.lock();
	if ((m_curPathIndex + steps) < m_path.size())
		m_curPathIndex += steps;
	else
		m_curPathIndex = m_path.size() - 1;
	m_mutex.unlock();

	m_moveable->move(m_path[m_curPathIndex]);
}

void ModelMover::reset()
{
	m_curPathIndex = 0;
	m_moveable->move(m_path[m_curPathIndex]);
}
