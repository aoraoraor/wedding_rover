#include "PathRenderer.h""

PathRenderer::PathRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, Path path)
	: Renderer(sceneRoot, elevation), m_path(path)
{
}

void PathRenderer::render()
{
	int i = 0;
	for (; i < m_path.size(); i+=4)
	{
		drawSphere(QVector3D(m_path[i].x, m_elevation[mkIdx(m_path[i].x, m_path[i].y)] + 3, -m_path[i].y));
	}
	if(i != m_path.size())	// Draw last point if it got skipped from the above +=4
	{
		i = m_path.size() - 1;
		drawSphere(QVector3D(m_path[i].x, m_elevation[mkIdx(m_path[i].x, m_path[i].y)] + 3, -m_path[i].y));
	}
}

void PathRenderer::drawSphere(QVector3D loc)
{
	Qt3DExtras::QSphereMesh* pointMesh = new Qt3DExtras::QSphereMesh();
	pointMesh->setRadius(2.0f);

	Qt3DCore::QTransform* transform = new Qt3DCore::QTransform();
	transform->setTranslation(loc);

	Qt3DExtras::QPhongMaterial* pointMaterial = new Qt3DExtras::QPhongMaterial();
	pointMaterial->setDiffuse(QColor(QRgb(0xFF0000)));

	Qt3DCore::QEntity* m_entity = new Qt3DCore::QEntity(m_sceneRoot);
	m_entity->addComponent(pointMesh);
	m_entity->addComponent(pointMaterial);
	m_entity->addComponent(transform);
}
