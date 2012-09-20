#include "Material.h"

/*
====================
Material::Material
	Returns a material with default values
====================
*/
Material::Material() {
	this->color = Color::ColorBlack();
	this->reflection = 0;
	this->transparency = 0;
}

/*
====================
Material::Material
	Returns a material with specified parameters
====================
*/

Material::Material(Color color, double reflection, double transparency) {
	this->color = color;
	this->reflection = reflection;
	this->transparency = transparency;
}