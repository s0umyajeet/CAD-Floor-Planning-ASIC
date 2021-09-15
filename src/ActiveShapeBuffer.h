#pragma once
#include "Shape.h"
#include "rapidcsv.h"
#include <map>

class ActiveShapeBuffer
{
public:
	//return singleton instance
	static ActiveShapeBuffer& get();
	static bool loadFiles(const std::string areaFilePath, const std::string placementFilePath);

private:
	ActiveShapeBuffer();
	~ActiveShapeBuffer() {};
public:
	static std::map<std::string, Shape> shapePlacementMap;
	static std::map<std::string, int>	 shapeAreaMap;

	static rapidcsv::Document areaFile;  
	static rapidcsv::Document placementFile;  
};


