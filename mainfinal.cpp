#include <iostream>
#include <string>
#include "planet.h"
#include <fstream>
#include "rocket.h"
#include <functional>
#include <cmath>
#include "systemfinal.h"

// double current_mass_function(double current_time)
// {
//     //double exponent = 0.009*current_time;
//     return (131e3 + 5000e3 - 40000*current_time);
// }

using Vec = std::vector<double>;
using Func = std::function<Vec(double)>;
using VecVec = std::vector<std::vector<double>>;

double current_mass_function(double current_time)
{
    double mass = (131e3+4500e3) - (25000/0.005)*(1-exp(-0.005*current_time));
    return mass;
}

double current_mass_function_for_TLI(double current_time)
{
    double mass = (299030) - (700/0.0003)*(1-exp(-0.0003*current_time));
    return mass;
}

double current_mass_function_for_lunar_capture(double current_time)
{
    double mass = (121050) - 5000/0.0008*(1-exp(-0.0008*current_time));
    return mass;
}

double current_mass_function_for_DOI(double current_time)
{
    double mass = 94262.6 - 500/0.008*(1-exp(-0.008*current_time));
    return mass;
}

double current_mass_function_for_lunar_ladning(double current_time)
{
    double mass =  std::max(50000.0,93361.2-128.35*current_time);
    return mass;

}

double find_tilt(
    double lower_bound,
    double upper_bound, 
    double error
)
{
    double middle;
    std::vector<std::vector<double>> vector_of_success;
    double fuel;
    std::vector<double> time;
    while(upper_bound - lower_bound > error)
    {
        Systemtest systemtest(5*24*3600, time);
        std::function<double(double)> mass_function = current_mass_function;
        std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
        std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
        std::function<double(double)> mass_function_for_DOI = current_mass_function_for_DOI;
        std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
        Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
        Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_function_for_DOI, mass_funtion_for_lunar_ladning);
        middle = (upper_bound + lower_bound)/2;
        systemtest.lift_off(moon, earth, rocket, 610);
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
        fuel = rocket._mass_function(systemtest._time.back());
        systemtest._time.clear();
        time.clear();
    }
        vector_of_success.push_back({double(610), middle, fuel});
    std::cout << "altitude: " << vector_of_success.at(0).at(0) 
    << "angle: " << vector_of_success.at(0).at(1)
    << "fuel: " << vector_of_success.at(0).at(2) << std::endl;
    return middle;
}


double find_dv(double upper_bound, double lower_bound, double error)
{
    double middle;
    std::vector<double> time;
    while(upper_bound - lower_bound > error)
    {
        Systemtest systemtest(5*24*3600, time);
        std::function<double(double)> mass_function = current_mass_function;
        std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
        std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
        std::function<double(double)> mass_function_for_DOI = current_mass_function_for_DOI;
        std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
        Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
        Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_function_for_DOI, mass_funtion_for_lunar_ladning);
        middle = (upper_bound + lower_bound)/2;
        systemtest.lift_off(moon, earth, rocket, 610);
        systemtest.tilting(moon, earth, rocket, 60.0293);
        systemtest.gravity_turn(moon, earth, rocket);
        systemtest.orbit(moon, earth, rocket);
        systemtest.translunar_injection_burn(moon, earth, rocket, 10895.47455);
        systemtest.lunar_trajectory(moon, earth, rocket, "capture");
        systemtest.lunar_capture(moon, earth, rocket);
        systemtest.lunar_orbit(moon, earth, rocket);
        double velocity = sqrt(pow(rocket._v_x.back() - moon.v_x_cm.back(), 2) + pow(rocket._v_y.back() - moon.v_y_cm.back(), 2) + pow(rocket._v_z.back() - moon.v_z_cm.back(), 2));
        systemtest.lunar_landing(moon, earth, rocket, velocity - middle, "test");
        std::cout << "peri: " << systemtest.perilune << ", " << middle <<  std::endl;
        if(systemtest.perilune - 15.5e3 > 0)
        {   
            lower_bound = middle;
        }
        else
        {
            upper_bound = middle;
        }
        systemtest._time.clear();
        time.clear();
    }

    return middle;
}

Vec f(double v)
{
    Vec time;
    Systemtest systemtest(5*24*3600, time);
    std::function<double(double)> mass_function = current_mass_function;
    std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
    std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
    std::function<double(double)> mass_function_for_DOI = current_mass_function_for_DOI;
    std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
    Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
    Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, 
                mass_function_for_TLI, mass_funtion_for_lunar_capture, 
                mass_function_for_DOI, mass_funtion_for_lunar_ladning);
    systemtest.lift_off(moon, earth, rocket, 610);
    systemtest.tilting(moon, earth, rocket, 60.0293);
    systemtest.gravity_turn(moon, earth, rocket);
    systemtest.orbit(moon, earth, rocket);
    systemtest.translunar_injection_burn(moon, earth, rocket, v);
    systemtest.lunar_trajectory(moon, earth, rocket, "");
    return {systemtest.perilune, systemtest.is_return};
}

Vec bracket_v(double initial)
{
    double low = 0, up = 0;
    for(double v = initial; v > 10850; v-=5)
    {
        Vec f0 = f(v);
        double perilune = f0[0];
        double is_return = f0[1];

        double error = perilune - 150e3;
        if(error > 0 && is_return == 1.) low = v;
        if(error < 0 && is_return == 1.) up = v;
    }
    return {low, up};
}

double tli_v(double up, double low)
{
    while(up - low > 0.001)
    {
        double middle = (up + low)/2;
        Vec f0 = f(middle);

        if(f0[0] - 150e3 > 0) up = middle;
        else low = middle;

        std::cout << f0[0] << std::endl;
    }
    return (up+low)/2;
}

int main()
{
    double rocket_mass = 131e3;
    double fuel_mass = 2286e3;
    std::function<double(double)> mass_function = current_mass_function;
    std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
    std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
    std::function<double(double)> mass_function_for_DOI = current_mass_function_for_DOI;
    std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;

    Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
    Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_function_for_DOI, mass_funtion_for_lunar_ladning);

    std::vector<double> time;
    Systemtest systemtest(5*24*3600, time);

    
    //double tilt_angle = find_tilt(0, 90, 0.1);
    //std::cout << tilt_angle << std::endl;

    // Vec bounds = bracket_v(10940);
    // std::cout << bounds[0] << ", " << bounds[1] << std::endl;

    // double v = tli_v(10900, 10895);
    // std::cout << std::setprecision(10) << v << std::endl;

    systemtest.lift_off(moon, earth, rocket, 610);
    systemtest.tilting(moon, earth, rocket, 60.0293);
    systemtest.gravity_turn(moon, earth, rocket);
    
    systemtest.orbit(moon, earth, rocket);
    systemtest.translunar_injection_burn(moon, earth, rocket, 10895.47455);
    systemtest.lunar_trajectory(moon, earth, rocket, "capture");
    systemtest.lunar_capture(moon, earth, rocket);
    systemtest.lunar_orbit(moon, earth, rocket);

    double velocity = sqrt(pow(rocket._v_x.back() - moon.v_x_cm.back(), 2) + pow(rocket._v_y.back() - moon.v_y_cm.back(), 2) + pow(rocket._v_z.back() - moon.v_z_cm.back(), 2));

    systemtest.lunar_landing(moon, earth, rocket, velocity - 33.6168, "");

    //std::cout << find_dv(60, 10, 0.001) << std::endl;

    return 0;
}