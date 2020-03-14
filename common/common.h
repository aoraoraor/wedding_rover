#ifndef _COMMON__H
#define _COMMON__H

#include <cstdint>
#include <vector>

struct Pos
{
	int x;
	int y;
};

using Path = std::vector<Pos>;

// Bits used in the overrides image bytes
enum OverrideFlags
{
	OF_PATH = 0x01,
	OF_RIVER_MARSH = 0x10,
	OF_INLAND = 0x20,
	OF_WATER_BASIN = 0x40,
};

// Some constants
enum
{
	IMAGE_DIM = 2048, // Width and height of the elevation and overrides image

	ROVER_X = 159,
	ROVER_Y = 1520,
	BACHELOR_X = 1303,
	BACHELOR_Y = 85,
	WEDDING_X = 1577,
	WEDDING_Y = 1294
};

enum ImagePixelValues
{
	IPV_PATH = 0,               // Results in red in the BMP
	IPV_WATER = 1,              // Results in the water color in the BMP
	IPV_ELEVATION_BEGIN = 2     // 2-255
};

int mkIdx(int x, int y);

uint8_t getelevetion(uint8_t overrides, uint8_t elevation, size_t x, size_t y);

#endif // #define _COMMON__H
