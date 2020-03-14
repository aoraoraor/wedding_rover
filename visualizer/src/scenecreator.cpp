#include "scenecreator.h""

Q3DSceneCreator::Q3DSceneCreator(
	const vector<uint8_t>& elevation, 
	const vector<uint8_t>& overrides, 
	const vector<uint8_t>& colorMap)
	:m_elevation(elevation), m_overrides(overrides), m_colorMap(colorMap)
{
	m_view = new Qt3DExtras::Qt3DWindow();
	m_sceneRoot = new Qt3DCore::QEntity();
	m_animator = new Animator();

	m_view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x0)));
	m_view->setRootEntity(m_sceneRoot);
}

Q3DSceneCreator::~Q3DSceneCreator()
{
	m_view->deleteLater();
	m_sceneRoot->deleteLater();
	delete m_animator;
}

void Q3DSceneCreator::createScene()
{
	initCamera();
	createHeightMap();
	createModels();
}

void Q3DSceneCreator::createHeightMap()
{
	m_heightMap = new HeightMapRenderer(m_sceneRoot, m_elevation, m_overrides, m_colorMap);
	m_heightMap->render();
}

void Q3DSceneCreator::createModels()
{
	m_roverModel = new ModelRenderer(
		m_sceneRoot, 
		m_elevation, 
		"models/rover/Rover3.obj", 
		{ ROVER_X , ROVER_Y },
		0.3f);

	m_bachelorModel = new ModelRenderer(
		m_sceneRoot,
		m_elevation,
		"models/groom/rp_dennis_posed_004_30k.OBJ",
		{ BACHELOR_X , BACHELOR_Y },
		0.3f);

	m_weddingModel = new ModelRenderer(
		m_sceneRoot,
		m_elevation,
		"models/wedding/Cyprys_House.obj",
		{ WEDDING_X , WEDDING_Y },
		10.0f);

	m_roverModel->render();
	m_bachelorModel->render();
	m_weddingModel->render();
}

void Q3DSceneCreator::createPaths(Path roverToBachelor, Path bachelorToWedding)
{
	PathRenderer* roverToBachelorRenderer = new PathRenderer(m_sceneRoot, m_elevation, roverToBachelor);
	PathRenderer* bachelorToWeddingRenderer = new PathRenderer(m_sceneRoot, m_elevation, bachelorToWedding);

	roverToBachelorRenderer->render();
	bachelorToWeddingRenderer->render();

	m_animator->setupAnimation(m_roverModel, roverToBachelor, m_bachelorModel, bachelorToWedding);
}

void Q3DSceneCreator::initCamera()
{
	m_camera = m_view->camera();

	// light setup
	Qt3DCore::QEntity* lightEntity = new Qt3DCore::QEntity(m_camera);
	Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(lightEntity);
	light->setColor("white");
	light->setIntensity(1);
	lightEntity->addComponent(light);

	// camera setup
	m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
	m_camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
	m_camera->setViewCenter(QVector3D(IMAGE_DIM / 2.0f, 0.0f, -IMAGE_DIM / 2.0f));
	m_camera->setPosition(QVector3D(IMAGE_DIM / 2.0f, 3000.0f, IMAGE_DIM));
	m_camera->setFarPlane(10000);

	// cam controller setup
	Qt3DExtras::QOrbitCameraController* camController = new Qt3DExtras::QOrbitCameraController(m_sceneRoot);
	camController->setCamera(m_camera);
	camController->setLinearSpeed(-3000);
	camController->setLookSpeed(-20);
}

Qt3DExtras::Qt3DWindow* Q3DSceneCreator::getView()
{
	return m_view;
}

void Q3DSceneCreator::setupTextUpdater(QObject* textRef)
{
	m_animator->setupUITextRef(textRef);
	m_animator->initUIText();
}

Animator* Q3DSceneCreator::getAnimator()
{
	return m_animator;
}