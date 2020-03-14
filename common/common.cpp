#include <common.h>

int mkIdx(int x, int y)
{
	return IMAGE_DIM * y + x;
}

uint8_t getelevetion(uint8_t overrides, uint8_t elevation, size_t x, size_t y)
{
	const int idx = x + y * IMAGE_DIM;

	// Signifies water
	if ((overrides & (OF_WATER_BASIN | OF_RIVER_MARSH)) ||
		elevation == 0)
	{
		return uint8_t(IPV_WATER);
	}

	// Signifies normal ground color
	if (elevation < IPV_ELEVATION_BEGIN)
	{
		elevation = IPV_ELEVATION_BEGIN;
	}
	return elevation;
}