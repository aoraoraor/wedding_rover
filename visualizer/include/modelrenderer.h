#ifndef _MODEL_RENDERER_H_
#define _MODEL_RENDERER_H_

#include "renderer.h"
#include "moveable.h"

class ModelRenderer : public Renderer, public Moveable
{
private:
	Qt3DRender::QSceneLoader* m_sceneLoader;
	Qt3DCore::QTransform* m_transform;

	string m_model;

public:
	ModelRenderer(Qt3DCore::QEntity* sceneRoot, const vector<uint8_t>& elevation, string model, Pos at, float scale);
	~ModelRenderer();
	void render() override;

private:
	void performMove(Pos to);
};


#endif // _MODEL_RENDERER_H_

