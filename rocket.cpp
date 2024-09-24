#include "rocket.h"
#include <vector>
#include <fstream>
#include <cmath>

Rocket::Rocket(
    double fuel_mass, 
    double x_0, 
    double y_0, 
    double z_0, 
    double v_x_0,
    double v_y_0,
    double v_z_0,
    double rocket_mass,
    std::function<double(double)> mass_function,
    std::function<double(double)> mass_function_for_TLI,
    std::function<double(double)> mass_function_for_lunar_capture,
    std::function<double(double)> mass_function_for_lunar_landing
)
{
    _fuel_mass = fuel_mass;
    _x.push_back(x_0);
    _y.push_back(y_0);
    _z.push_back(z_0);
    _v_x.push_back(0);
    _v_y.push_back(0);
    _v_z.push_back(0);
    _rocket_mass = rocket_mass;
    _mass_function = mass_function;
    _mass_function_for_TLI = mass_function_for_TLI;
    _mass_function_for_lunar_capture = mass_function_for_lunar_capture;
    _mass_function_for_lunar_landing = mass_function_for_lunar_landing;
}




