#pragma once
#include "Shape.h"
#include "rapidcsv.h"
#include <unordered_map>

class ActiveShapeBuffer
{
public:
	//return singleton instance
	static ActiveShapeBuffer& get();

private:
	ActiveShapeBuffer();
	~ActiveShapeBuffer() {};
public:
	static std::unordered_map<std::string, Shape> shapePlacementMap;
	static std::unordered_map<std::string, int>	 shapeAreaMap;

	static rapidcsv::Document areaFile;  
	static rapidcsv::Document placementFile;  
};


