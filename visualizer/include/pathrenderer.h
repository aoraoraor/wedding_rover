#ifndef _PATH_RENDERER_H_
#define _PATH_RENDERER_H_

#include "renderer.h"
#include <Qt3DExtras/QSphereMesh>

class PathRenderer : public Renderer
{
private:
	Path m_path;

public:
	PathRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, Path path);
	void render() override;

private:
	void drawSphere(QVector3D loc);
};


#endif // _PATH_RENDERER_H_

