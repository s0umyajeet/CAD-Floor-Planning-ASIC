#include "Shape.h"
#include <iostream>


Shape::Shape() {
	cmd = "NULL";
	id = "NULL";
	area = INF;
	color = ImVec4(100 + rand() % 100, 100 + rand() % 100, 100 + rand() % 100, 255);
	len_in_x = INF;
	len_in_y = INF;
	offsetX = INF;
	offsetY = INF;
	posX = INF;
	posY = INF;
	ref_shape_id = "NULL";
}


void Shape::show_data(std::stringstream &outputss) {

	outputss << "\t" << "cmd " << this->cmd << std::endl;
	outputss << "\t" << "id " << this->id << std::endl;
	outputss << "\t" << "area " << this->area << std::endl;
	outputss << "\t" << "len_in_x " << this->len_in_x << std::endl;
	outputss << "\t" << "len_in_y " << this->len_in_y << std::endl;
	outputss << "\t" << "offsetX " << this->offsetX << std::endl;
	outputss << "\t" << "offsetY " << this->offsetY<< std::endl;
	outputss << "\t" << "posX " << this->posX << std::endl;
	outputss << "\t" << "posY " << this->posY << std::endl;
	outputss << "\t" << "ref_shape_id " << this->ref_shape_id << std::endl;
}
