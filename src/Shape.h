#pragma once
#include <imgui.h>
#include <sstream>
#include <string>

constexpr auto INF = 99999999999;

class Shape
{
private:
  std::string cmd;
  std::string id;
  std::string name;
  int area;
  ImVec4 color;

  //dimensions
  int len_in_x;
  int len_in_y;

  //offsets for lower left vertex
  int offsetX;
  int offsetY;

  //actual position of lower left vertex;
  int posX;
  int posY;

  //id reference shape
  std::string ref_shape_id;

public:
  Shape();
  void write_data(std::stringstream &outputss);
  void show_data();
  void setArea(int area) { this->area = area; }
  void setName(std::string name) { this->name = name; }
  void setposX(int x) { this->posX = x; }
  void setposY(int y) { this->posY = y; }
  void setID(std::string id) { this->id = id; }
  void setcmd(std::string cmd) { this->cmd = cmd; }
  void set_ref_shape_id(std::string ref) { this->ref_shape_id = ref; }
  void set_offset_x(int offsetX) { this->offsetX = offsetX; }
  void set_offset_y(int offsetY) { this->offsetY = offsetY; }
  void set_len_in_x(int len) { this->len_in_x = len; }
  void set_len_in_y(int len) { this->len_in_y = len; }
  void setColor(ImVec4 c) { this->color = c; }

  const inline int getArea() { return this->area; }
  const inline int getposX() { return this->posX; }
  const inline std::string getName() { return this->name; }
  const inline int getposY() { return this->posY; }
  const inline int getoffsetX() { return this->offsetX; }
  const inline int getoffsetY() { return this->offsetY; }
  const inline int get_len_in_x() { return this->len_in_x * 25; }
  const inline int get_len_in_y() { return this->len_in_y * 25; }
  const inline ImVec4 getColor() { return this->color; }
  inline const std::string getID() { return this->id; }
  inline const std::string get_ref_id() { return this->ref_shape_id; }
};
