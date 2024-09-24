#ifndef ROCKET_H
#define ROCKET_H
#include <vector>
#include <cmath>
#include <functional>

class Rocket
{
    public: 
        Rocket(    
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
        );
        double _fuel_mass, _rocket_mass;
        std::vector<double> _x, _y, _z, _v_x, _v_y, _v_z, _a_x, _a_y, _a_z;
        std::function<double(double)> _mass_function;
        std::function<double(double)> _mass_function_for_TLI;
        std::function<double(double)> _mass_function_for_lunar_capture;
        std::function<double(double)> _mass_function_for_lunar_landing;

};
#endif