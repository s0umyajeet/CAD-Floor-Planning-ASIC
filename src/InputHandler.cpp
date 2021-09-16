#include "InputHandler.h"
#include "ActiveShapeBuffer.h"
#include "GraphicEngine.h"
#include "Shape.h"
#include "rapidcsv.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using ShapeDataMatrix = std::vector<std::vector<std::string>>;

InputHandler& InputHandler::get()
{
  static InputHandler instance;
  return instance;
}

int InputHandler::parseShapeCSV(rapidcsv::Document doc)
{
  size_t row_count = doc.GetRowCount();

  for (int i = 0; i < row_count; i++)
  {
    std::vector<std::string> this_row = doc.GetRow<std::string>(i);
    ActiveShapeBuffer::get().shapeAreaMap.insert({ this_row[0], stoi(this_row[1]) });
  }
  return row_count;
}

int InputHandler::parsePlacementCSV(rapidcsv::Document doc)
{
  //get all rows in file one by one
  size_t row_count = doc.GetRowCount();

  for (int i = 0; i < row_count; i++)
  {
    std::vector<std::string> this_row = doc.GetRow<std::string>(i);

    Shape* temp = new Shape;

    temp->setcmd(this_row[0]);
    temp->setName(GraphicEngine::get().constructShapeIDFromShapeName(this_row[1]));
    temp->setID(this_row[1]);
    temp->set_ref_shape_id(this_row[2]);
    temp->set_offset_x(stoi(this_row[3]));
    temp->set_offset_y(stoi(this_row[4]));

    //set absolute coordinates
    if (temp->get_ref_id() == "top")
    {
      temp->setposX(temp->getoffsetX());
      temp->setposY(temp->getoffsetY());
    }
    else
    {
      //get coordinates of reference shape
      int ref_x = ActiveShapeBuffer::get().shapePlacementMap[temp->get_ref_id()].getposX();
      int ref_y = ActiveShapeBuffer::get().shapePlacementMap[temp->get_ref_id()].getposY();
      temp->setposX(temp->getoffsetX() + ref_x);
      temp->setposY(temp->getoffsetY() + ref_y);
    }

    //get & set the shape's area from the id-vs-area map
    int area = ActiveShapeBuffer::get().shapeAreaMap[temp->getID()];
    temp->setArea(area);

    //calculate shape's side length from its area
    temp->set_len_in_x(sqrt(temp->getArea()));
    temp->set_len_in_y(sqrt(temp->getArea()));

    ActiveShapeBuffer::get().shapePlacementMap.insert({ temp->getID(), *temp });
    GraphicEngine::get()._current_draw_list.push_back(temp->getID());
  }
  return row_count;
}
