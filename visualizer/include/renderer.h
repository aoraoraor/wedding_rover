#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "common.h"
#include <stdint.h>
#include <vector>

#include <QtCore/QObject>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DExtras/QPhongMaterial>
#include <QPerVertexColorMaterial>
#include <Qt3DRender/qbuffer.h>
#include <QPerVertexColorMaterial>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender\qgeometryrenderer.h>
#include <Qt3DRender/qattribute.h>

using namespace std;

class Renderer
{
protected:
	/** Qt3D elements
	*/
	Qt3DCore::QEntity* m_sceneRoot;
	Qt3DCore::QEntity* m_entity;

	/** Scene specific elements
	*/
	const vector<uint8_t>& m_elevation;

public:
	Renderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation);
	virtual ~Renderer();
	virtual void render() = 0;
};

#endif // _RENDERER_H_

