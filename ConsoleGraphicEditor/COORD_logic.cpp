#include "COORD_logic.h"

bool AreCOORDSetsEqual(const std::unordered_set <COORD, COORDHash, COORDEqual> inSet1, const std::unordered_set<COORD, COORDHash, COORDEqual> inSet2)
{
	if (inSet1.size() != inSet2.size())
	{
		return false;
	}
	for (auto& coord : inSet1)
	{
		if (!(inSet2.contains(coord)))
		{
			return false;
		}
	}
	return true;
}