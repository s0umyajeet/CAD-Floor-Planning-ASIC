#pragma once
#include "Shape.h"
#include "rapidcsv.h"
#include <map>

class ActiveShapeBuffer
{
public:
	//return singleton instance
	static ActiveShapeBuffer& get();

private:
	ActiveShapeBuffer();
	~ActiveShapeBuffer() {};
public:
	std::map<std::string, Shape> shapePlacementMap;
	std::map<std::string, int>	 shapeAreaMap;

	rapidcsv::Document areaFile;  
	rapidcsv::Document placementFile;  
};


