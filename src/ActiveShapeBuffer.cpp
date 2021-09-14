#include "ActiveShapeBuffer.h"

ActiveShapeBuffer& ActiveShapeBuffer::get()
{
	static ActiveShapeBuffer instance;
	return instance;
}

std::unordered_map<std::string, Shape> ActiveShapeBuffer::shapePlacementMap;
std::unordered_map<std::string, int> ActiveShapeBuffer::shapeAreaMap;

rapidcsv::Document ActiveShapeBuffer::areaFile = rapidcsv::Document("E:/Dev/C++ Projects/iHack-CAD-2021/iHack-CAD-2021/src/shapes.csv", rapidcsv::LabelParams(-1, -1)); // , rapidcsv::SeparatorParams(' '));
rapidcsv::Document ActiveShapeBuffer::placementFile = rapidcsv::Document("E:/Dev/C++ Projects/iHack-CAD-2021/iHack-CAD-2021/src/placement.csv", rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(' '));


ActiveShapeBuffer::ActiveShapeBuffer() {}