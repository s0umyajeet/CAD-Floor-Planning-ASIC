#include "ActiveShapeBuffer.h"

ActiveShapeBuffer& ActiveShapeBuffer::get()
{
  static ActiveShapeBuffer instance;
  return instance;
}

bool ActiveShapeBuffer::loadFiles(const std::string areaFilePath, const std::string placementFilePath)
{
  //todo
  areaFile      = rapidcsv::Document(areaFilePath, rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(' '));
  placementFile = rapidcsv::Document(placementFilePath, rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(' '));
  return true;
}

std::map<std::string, Shape> ActiveShapeBuffer::shapePlacementMap;
std::map<std::string, int> ActiveShapeBuffer::shapeAreaMap;

rapidcsv::Document ActiveShapeBuffer::areaFile;
rapidcsv::Document ActiveShapeBuffer::placementFile;

ActiveShapeBuffer::ActiveShapeBuffer() {}