#ifndef _HEIGHTMAP_RENDERER_H_
#define _HEIGHTMAP_RENDERER_H_

#include "renderer.h"

class HeightMapRenderer : public Renderer
{
private:
	Qt3DRender::QGeometryRenderer* m_geometryRenderer;
	Qt3DRender::QGeometry* m_geometry;
	QVector<QVector3D> m_vertexPositions;

	const vector<uint8_t>& m_overrides;
	const vector<uint8_t>& m_colorMap;

public:
	HeightMapRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, const vector<uint8_t>& overrides, const vector<uint8_t>& colorMap);
	~HeightMapRenderer();
	void render() override;

private:
	void parseVertices();
	void populateVertices();
	void populateColors();
	void populateIndexes();
};


#endif // _HEIGHTMAP_RENDERER_H_

