#include "Shape.h"
#include <iostream>


Shape::Shape() {
	cmd = "NULL";
	id = "NULL";
	area = INF;
	color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
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



/*
shape::shape(shape&& other)
	: cmd(std::move(other.cmd)), id(std::move(other.id)), area(std::move(other.area)), len_in_x(std::move(other.len_in_x)), len_in_y(std::move(other.len_in_y)),
	offsetx(std::move(other.offsetx)), offsety(std::move(other.offsety)), posx(std::move(other.posx)), posy(std::move(other.posy)), ref_shape_id(std::move(other.ref_shape_id))
{

}
*/
