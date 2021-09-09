#include "ActiveShapeBuffer.h"

ActiveShapeBuffer& ActiveShapeBuffer::get()
{
	static ActiveShapeBuffer instance;
	return instance;
}

ActiveShapeBuffer::ActiveShapeBuffer() {
	areaFile = rapidcsv::Document("E:/Dev/C++ Projects/iHack-CAD-2021/iHack-CAD-2021/src/shapes.csv", rapidcsv::LabelParams(-1, -1)); // , rapidcsv::SeparatorParams(' '));
	placementFile = rapidcsv::Document("E:/Dev/C++ Projects/iHack-CAD-2021/iHack-CAD-2021/src/placement.csv", rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(' '));
}