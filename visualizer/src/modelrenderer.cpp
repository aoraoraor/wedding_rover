#include "modelrenderer.h""

ModelRenderer::ModelRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, string model, Pos at, float scale)
	: 
	Renderer(sceneRoot, elevation), m_model(model)
{
	m_sceneLoader = new Qt3DRender::QSceneLoader(m_entity);
	m_transform = new Qt3DCore::QTransform;
	m_transform->setScale(scale);
	m_transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 0.0f), 0.0f));
	m_transform->setTranslation(QVector3D(at.x, m_elevation[mkIdx(at.x, at.y)] + 2, -at.y));
}

void ModelRenderer::render()
{
	m_sceneLoader->setSource(QUrl::fromLocalFile(QString::fromStdString(m_model)));

	m_entity->addComponent(m_sceneLoader);
	m_entity->addComponent(m_transform);
}

void ModelRenderer::performMove(Pos to)
{
	m_transform->setTranslation(QVector3D(to.x, m_elevation[mkIdx(to.x, to.y)] + 1, -to.y));
}

ModelRenderer::~ModelRenderer()
{
	m_sceneLoader->deleteLater();
	m_transform->deleteLater();
}