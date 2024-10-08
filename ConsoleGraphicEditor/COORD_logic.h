#pragma once

#include <windows.h>
#include <memory>

struct COORDHash
{
	size_t operator()(const COORD& coord) const
	{
		return std::hash<SHORT>()(coord.X) ^ (std::hash<SHORT>()(coord.Y) << 1);
	}
};

struct COORDEqual
{
	bool operator()(const COORD& coord1, const COORD& coord2) const
	{
		return coord1.X == coord2.X && coord1.Y == coord2.Y;
	}
};