#include "heightmaprenderer.h""

HeightMapRenderer::HeightMapRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, const vector<uint8_t>& overrides, const vector<uint8_t>& colorMap)
	: Renderer(sceneRoot, elevation), m_overrides(overrides), m_colorMap(colorMap)
{
	m_geometry = new Qt3DRender::QGeometry(m_entity);
	m_geometryRenderer = new Qt3DRender::QGeometryRenderer;
}

HeightMapRenderer::~HeightMapRenderer()
{
	m_geometry->deleteLater();
	m_geometryRenderer->deleteLater();
}

void HeightMapRenderer::render()
{
	m_geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
	m_geometryRenderer->setGeometry(m_geometry);

	parseVertices();
	populateVertices();
	populateIndexes();
	populateColors();

	m_entity->addComponent(m_geometryRenderer);
	m_entity->addComponent(new Qt3DExtras::QPerVertexColorMaterial());
}

void HeightMapRenderer::parseVertices()
{
	m_vertexPositions.clear();
	for (int row = 0; row < IMAGE_DIM; row++) {
		for (int col = 0; col < IMAGE_DIM; col++) {
			m_vertexPositions.append(QVector3D(row, m_elevation[mkIdx(row, col)], -col));
		}
	}
}

void HeightMapRenderer::populateVertices()
{
	QByteArray vertexBufferData;
	vertexBufferData.resize(IMAGE_DIM * IMAGE_DIM * 3 * sizeof(float));

	float* rawVertexArray = reinterpret_cast<float*>(vertexBufferData.data());
	int idx = 0;
	for each (const QVector3D & v in m_vertexPositions) {
		rawVertexArray[idx++] = v.x();
		rawVertexArray[idx++] = v.y();
		rawVertexArray[idx++] = v.z();
	}

	Qt3DRender::QBuffer* vertexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_geometry);
	vertexDataBuffer->setData(vertexBufferData);

	Qt3DRender::QAttribute* positionAttribute = new  Qt3DRender::QAttribute();
	positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
	positionAttribute->setBuffer(vertexDataBuffer);
	positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
	positionAttribute->setDataSize(3);
	positionAttribute->setByteOffset(0);
	positionAttribute->setByteStride(0);
	positionAttribute->setCount(m_vertexPositions.count());
	positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

	m_geometry->addAttribute(positionAttribute);
}

void HeightMapRenderer::populateColors()
{
	QByteArray colorBufferData;
	colorBufferData.resize(IMAGE_DIM * IMAGE_DIM * 3 * sizeof(float));

	float* rawColorArray = reinterpret_cast<float*>(colorBufferData.data());
	int cidx = 0;
	for (int row = 0; row < IMAGE_DIM; row++) {
		for (int col = 0; col < IMAGE_DIM; col++) {
			uint8_t elev = getelevetion(m_overrides[mkIdx(row, col)], m_elevation[mkIdx(row, col)], row, col);
			rawColorArray[cidx++] = m_colorMap[3 * elev] / 255.0f;
			rawColorArray[cidx++] = m_colorMap[3 * elev + 1] / 255.0f;
			rawColorArray[cidx++] = m_colorMap[3 * elev + 2] / 255.0f;
		}
	}

	Qt3DRender::QBuffer* colorDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_geometry);;
	colorDataBuffer->setData(colorBufferData);

	Qt3DRender::QAttribute* colorAttribute = new  Qt3DRender::QAttribute();
	colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
	colorAttribute->setBuffer(colorDataBuffer);
	colorAttribute->setDataType(Qt3DRender::QAttribute::Float);
	colorAttribute->setDataSize(3);
	colorAttribute->setByteOffset(0);
	colorAttribute->setByteStride(0);
	colorAttribute->setCount(colorDataBuffer->data().count());
	colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());

	m_geometry->addAttribute(colorAttribute);
}

void HeightMapRenderer::populateIndexes()
{
	Qt3DRender::QBuffer* indexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, m_geometry);
	QByteArray indexBufferData;
	int indicesCount = (IMAGE_DIM - 1) * (IMAGE_DIM - 1) * 2 * 3;
	indexBufferData.resize(indicesCount * sizeof(uint));
	uint* rawIndexArray = reinterpret_cast<uint*>(indexBufferData.data());

	int index = 0;
	for (int row = 0; row < IMAGE_DIM - 1; row++) 
	{
		for (int column = 0; column < IMAGE_DIM - 1; column++) 
		{
			int vertexBufferIndex = (row * IMAGE_DIM) + column;
			rawIndexArray[index++] = vertexBufferIndex;
			rawIndexArray[index++] = vertexBufferIndex + IMAGE_DIM;
			rawIndexArray[index++] = vertexBufferIndex + 1;
			rawIndexArray[index++] = vertexBufferIndex + 1;
			rawIndexArray[index++] = vertexBufferIndex + IMAGE_DIM;
			rawIndexArray[index++] = vertexBufferIndex + IMAGE_DIM + 1;
		}
	}

	indexDataBuffer->setData(indexBufferData);

	Qt3DRender::QAttribute* indexAttribute = new  Qt3DRender::QAttribute();
	indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
	indexAttribute->setBuffer(indexDataBuffer);
	indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
	indexAttribute->setDataSize(1);
	indexAttribute->setByteOffset(0);
	indexAttribute->setByteStride(0);
	indexAttribute->setCount(indicesCount);

	m_geometry->addAttribute(indexAttribute);
}

