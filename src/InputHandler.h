#pragma once

#include "rapidcsv.h"
#include "shape.h"
#include <vector>

class InputHandler
{
public:
  static InputHandler& get();
  int parsePlacementCSV(rapidcsv::Document doc);
  int parseShapeCSV(rapidcsv::Document doc);

private:
  InputHandler()  = default;
  ~InputHandler() = default;
};
