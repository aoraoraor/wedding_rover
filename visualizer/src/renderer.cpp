#include "renderer.h""

Renderer::Renderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation)
	: m_sceneRoot(sceneRoot), m_elevation(elevation)
{
	m_entity = new Qt3DCore::QEntity(m_sceneRoot);
}

Renderer::~Renderer()
{
	delete m_entity;
}
