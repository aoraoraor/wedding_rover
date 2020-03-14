#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "common.h"
#include "scenecreator.h"

#include <vector>

#include "qrendersettings.h"
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <qquickview.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <qqmlcontext.h>
#include <qquickitem.h>

using namespace std;

class MainWindow : public QWidget
{
	Q_OBJECT

private:
	typedef Path(*PathPlanner)(Pos, Pos, const vector<uint8_t>&, vector<uint8_t>&);
	QQuickView* m_qmlView;
	Q3DSceneCreator* m_qt3dSceneView;
	PathPlanner m_pathPlannerFunc;
	const vector<uint8_t>& m_elevation;
	vector<uint8_t>& m_overrides;
	vector<uint8_t>& m_colorMap;

public:
	MainWindow(const vector<uint8_t>& elevation, vector<uint8_t>& overrides, vector<uint8_t>& colorMap, PathPlanner planner);
	Q_INVOKABLE void calculate();

private:
	void init();
};

#endif // _MAINWINDOW_H_

