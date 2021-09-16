#include "Shape.h"
#include <iostream>

Shape::Shape()
{
  cmd          = "NULL";
  id           = "NULL";
  name         = "NULL";
  area         = INF;
  color        = ImVec4(100 + rand() % 100, 100 + rand() % 100, 100 + rand() % 100, 255);
  len_in_x     = INF;
  len_in_y     = INF;
  offsetX      = 100.0f;
  offsetY      = 100.0f;
  posX         = INF;
  posY         = INF;
  ref_shape_id = "NULL";
}

void Shape::write_data(std::stringstream &outputss)
{
  outputss << "\t"
           << "cmd " << this->cmd << "\n";
  outputss << "\t"
           << "id " << this->id << "\n";
  outputss << "\t"
           << "name " << this->name << "\n";
  outputss << "\t"
           << "area " << this->area << "\n";
  outputss << "\t"
           << "len_in_x " << this->len_in_x << "\n";
  outputss << "\t"
           << "len_in_y " << this->len_in_y << "\n";
  outputss << "\t"
           << "offsetX " << this->offsetX << "\n";
  outputss << "\t"
           << "offsetY " << this->offsetY << "\n";
  outputss << "\t"
           << "posX " << this->posX << "\n";
  outputss << "\t"
           << "posY " << this->posY << "\n";
  outputss << "\t"
           << "ref_shape_id " << this->ref_shape_id << "\n";
}

void Shape::show_data()
{
  std::cout << "\t"
            << "cmd " << this->cmd << "\n";
  std::cout << "\t"
            << "id " << this->id << "\n";
  std::cout << "\t"
            << "name " << this->name << "\n";
  std::cout << "\t"
            << "area " << this->area << "\n";
  std::cout << "\t"
            << "len_in_x " << this->len_in_x << "\n";
  std::cout << "\t"
            << "len_in_y " << this->len_in_y << "\n";
  std::cout << "\t"
            << "offsetX " << this->offsetX << "\n";
  std::cout << "\t"
            << "offsetY " << this->offsetY << "\n";
  std::cout << "\t"
            << "posX " << this->posX << "\n";
  std::cout << "\t"
            << "posY " << this->posY << "\n";
  std::cout << "\t"
            << "ref_shape_id " << this->ref_shape_id << "\n";
}
