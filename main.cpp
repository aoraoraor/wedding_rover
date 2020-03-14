#include <iostream>
#include <fstream>
#include <stdint.h>

#include <QtWidgets/QApplication>

#include "visualizer.h"
#include "mainwindow.h"
#include "pathplanner.h"

std::ifstream::pos_type fileSize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	if (!in.good())
	{
		throw std::exception();
	}
	return in.tellg();
}

std::vector<uint8_t> loadFile(const char* fileName, size_t expectedFileSize)
{
	size_t fsize = fileSize(fileName);
	if (fsize != expectedFileSize)
	{
		throw std::exception();
	}
	std::vector<uint8_t> data(fsize);
	std::ifstream ifile(fileName, std::ifstream::binary);
	if (!ifile.good())
	{
		throw std::exception();
	}
	ifile.read((char*)& data[0], fsize);
	return data;
}

Path planPath(Pos from, Pos to, const vector<uint8_t>& elevation, vector<uint8_t>& overrides)
{
	const float weight = 1; // with a "weight" of 1 the rover goes half the speed with each positive change in elevation
							// and sqrt(2) times the speed with each negative change; e.g., when going from a cell with elevation 1
							// to a cell with elevation 2, the speed is half, when going from 1 to 3 the speed is 1/4th etc.

	//auto modelConfig = pathplanner::AStarPlanner::RoverModelConfig{weight, /* max speed */ 5.0f};
	auto modelConfig = pathplanner::AStarPlanner::RoverModelConfig{ weight, 5.0f, [](int ax, int ay, int bx, int by) {return std::sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)); } };
	modelConfig.m_baseSpeed = 1.25;
	constexpr float relaxation = 10.f;

	printf("Planning path from bachelor's to wedding's position: ");
	const auto pathAndCosts = pathplanner::AStarPlanner::plan(
		{ from.x, from.y }, { to.x, to.y }, IMAGE_DIM, IMAGE_DIM, elevation, modelConfig, relaxation);
	for (auto p : pathAndCosts.path) overrides[p.x + IMAGE_DIM * p.y] |= OF_PATH;
	if (!pathAndCosts.path.empty())
	{
		printf(" takes %f island seconds\n", pathAndCosts.finalCosts);
	}
	else
	{
		printf(" no path found!\n");
	}

	return pathAndCosts.path;
}

int main(int argc, wchar_t* argv[])
{
	constexpr size_t expectedFileSize = IMAGE_DIM * IMAGE_DIM;
	auto elevation = loadFile("assets/elevation.data", expectedFileSize);
	auto overrides = loadFile("assets/overrides.data", expectedFileSize);

	// impassable fields are marked with elevation == 0
	for (size_t i = 0; i < overrides.size(); ++i) if (overrides[i] & (OF_WATER_BASIN | OF_RIVER_MARSH)) elevation[i] = 0;

	QApplication app(argc, 0);

	auto colormap(visualizer::generateElevationColormap());
	MainWindow mainWindow(elevation, overrides, colormap, planPath);
	mainWindow.show();

	app.exec();
	return 0;
}
