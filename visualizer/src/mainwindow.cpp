#include "mainwindow.h"

MainWindow::MainWindow(const vector<uint8_t>& elevation, vector<uint8_t>& overrides,
	vector<uint8_t>& colorMap, PathPlanner planner)
	:m_elevation(elevation),
	m_colorMap(colorMap),
	m_overrides(overrides),
	m_pathPlannerFunc(planner)
{
	m_qmlView = new QQuickView;
	m_qt3dSceneView = new Q3DSceneCreator(m_elevation, m_overrides, m_colorMap);

	init();
}

void MainWindow::init()
{
	// 1- init qml view
	m_qmlView->setResizeMode(QQuickView::SizeRootObjectToView);
	m_qmlView->rootContext()->setContextProperty("pathcalc", this);
	m_qmlView->rootContext()->setContextProperty("animator", m_qt3dSceneView->getAnimator());
	m_qmlView->setSource(QUrl("qrc:/main.qml"));

	// 2- init 3d scene
	m_qt3dSceneView->createScene();
	m_qt3dSceneView->setupTextUpdater(m_qmlView->rootObject());

	// 3- init mainwindow
	setStyleSheet("background-color:black;");
	QHBoxLayout* layout = new QHBoxLayout(this);
	QWidget* quickViewWidget = QWidget::createWindowContainer(m_qmlView, this);
	quickViewWidget->setMaximumWidth(200);
	quickViewWidget->setMinimumWidth(200);
	QWidget* qt3dWidget = QWidget::createWindowContainer(m_qt3dSceneView->getView(), this);
	qt3dWidget->setMinimumWidth(800);
	qt3dWidget->setMinimumHeight(600);
	layout->addWidget(quickViewWidget);
	layout->addWidget(qt3dWidget);
	setLayout(layout);
}

void MainWindow::calculate()
{
	Path roverToBachelor = m_pathPlannerFunc({ ROVER_X, ROVER_Y }, { BACHELOR_X, BACHELOR_Y }, m_elevation, m_overrides);
	Path bachelorToWedding = m_pathPlannerFunc({ BACHELOR_X, BACHELOR_Y }, { WEDDING_X, WEDDING_Y }, m_elevation, m_overrides);

	m_qt3dSceneView->createPaths(roverToBachelor, bachelorToWedding);
}
