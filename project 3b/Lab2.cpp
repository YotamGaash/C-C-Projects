//
// Created by Yotam Gaash on 23/02/2024.
//
#include <iostream>
class Car
{
public:
	Car(const std::string& color): _color(color){}
	void set_color(const std::string& color){_color = color;}
	std::string& get_color() const {return _color;}
private:
	std::string _color;
};

int main()

{
	std::string color("Black");
	Car c(color);
	color = "Blue";
	c.set_color(color);
	std::cout << c.get_color() << std::endl;
}