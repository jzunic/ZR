#include <iostream>
#include <string>
#include "planet.h"
#include <fstream>
#include "rocket.h"
#include <functional>
#include <cmath>
#include "systemtest.h"

// double current_mass_function(double current_time)
// {
//     //double exponent = 0.009*current_time;
//     return (131e3 + 5000e3 - 40000*current_time);
// }

double current_mass_function(double current_time)
{
    double mass = (131e3+4500e3) - (25000/0.005)*(1-exp(-0.005*current_time));
    return mass;
}

double current_mass_function_for_TLI(double current_time)
{
    double mass = (329331) - (700/0.0003)*(1-exp(-0.0003*current_time));
    return mass;
}

double current_mass_function_for_lunar_capture(double current_time)
{
    double mass = (131064) - (80/0.00005)*(1-exp(-0.00005*current_time));
    return mass;
}

double current_mass_function_for_lunar_ladning(double current_time)
{
    double mass = (99617.1) - (80/0.0005)*(1-exp(-0.0005*current_time));
    return mass;
}

double find_tilt(
    double lower_bound,
    double upper_bound, 
    double error, 
    Systemtest systemtest
)
{
    double middle;

    while(upper_bound - lower_bound > error)
    {
        //std::cout << "upper: " << upper_bound << std::endl;
        //std::cout << "middle: " << (upper_bound + lower_bound)/2 << std::endl;
        //std::cout << "lower: " << lower_bound << std::endl;
        std::function<double(double)> mass_function = current_mass_function;
        std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
        std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
        std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
        Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
        Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_funtion_for_lunar_ladning);
        middle = (upper_bound + lower_bound)/2;
        systemtest.lift_off(moon, earth, rocket);
        systemtest.tilting(moon, earth, rocket, middle);
        systemtest.gravity_turn(moon, earth, rocket);
        std::cout << "mid: " << middle << std::endl;
        if(systemtest.angle_final - 90 > 0)
        {   
            upper_bound = middle;
        }
        else
        {
            lower_bound = middle;
        }
        systemtest._time.clear();
    }
    return middle;
}

int main()
{
    double rocket_mass = 131e3;
    double fuel_mass = 2286e3;
    std::function<double(double)> mass_function = current_mass_function;
    std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
    std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
    std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;

    Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
    Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_funtion_for_lunar_ladning);

    std::vector<double> time;
    Systemtest systemtest(5*24*3600, time);
    double tilt_angle = find_tilt(0, 90, 0.1, systemtest);
    std::cout << tilt_angle << std::endl;
    systemtest.lift_off(moon, earth, rocket);
    systemtest.tilting(moon, earth, rocket, tilt_angle);
    systemtest.gravity_turn(moon, earth, rocket);
    
    systemtest.orbit(moon, earth, rocket, 10e5);
    //system.translunar_injection_burn(moon, earth, rocket);
    //system.lunar_trajectory(moon, earth, rocket);
    //system.lunar_capture(moon, earth, rocket);
    //system.lunar_orbit(moon, earth, rocket);
    // system.lunar_landing(moon, earth, rocket);

    return 0;
}