#ifndef _SCENE_CREATOR_H_
#define _SCENE_CREATOR_H_

#include "common.h"
#include "heightmaprenderer.h"
#include "pathrenderer.h"
#include "modelrenderer.h"
#include "moveable.h"
#include <vector>
#include "modelmover.h"
#include "animator.h"

#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <QOrbitCameraController>
#include <Qt3DExtras/qt3dwindow.h>
#include <qforwardrenderer.h>
#include <qpointlight.h>

class Q3DSceneCreator
{
private:
	Qt3DExtras::Qt3DWindow* m_view;
	Qt3DCore::QEntity* m_sceneRoot;
	Qt3DRender::QCamera* m_camera;

	HeightMapRenderer* m_heightMap;
	ModelRenderer* m_roverModel;
	ModelRenderer* m_bachelorModel;
	ModelRenderer* m_weddingModel;
	Animator* m_animator;

	const vector<uint8_t>& m_elevation;
	const vector<uint8_t>& m_overrides;
	const vector<uint8_t>& m_colorMap;

public:
	Q3DSceneCreator(const vector<uint8_t>& elevation, const vector<uint8_t>& overrides, const vector<uint8_t>& colorMap);
	~Q3DSceneCreator();
	void createScene();
	void createPaths(Path roverToBachelor, Path bachelorToWedding);
	Animator* getAnimator();
	Qt3DExtras::Qt3DWindow* getView();
	void setupTextUpdater(QObject* textRef);

private:
	void initCamera();
	void createModels();
	void createHeightMap();
};

#endif // _SCENE_CREATOR_H_

