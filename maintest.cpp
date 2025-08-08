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
    double mass = (132987) - (80/0.00005)*(1-exp(-0.00005*current_time));
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
    double error
)
{
    double middle;
    std::vector<int> altitudes;
    for(int i = 10; i < 2000; i+=10)
        altitudes.push_back(i);

    std::vector<std::vector<double>> vector_of_success;
    double memory_lower_bound = lower_bound;
    double memory_upper_bound = upper_bound;
    double fuel;
    std::vector<double> time;
    Systemtest systemtest(5*24*3600, time);
    for(int altitude : altitudes)
    {
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
            systemtest.lift_off(moon, earth, rocket, altitude);
            systemtest.tilting(moon, earth, rocket, middle);
            systemtest.gravity_turn(moon, earth, rocket);
            //std::cout << "mid: " << middle << std::endl;
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
        }
        vector_of_success.push_back({double(altitude), middle, fuel});
        upper_bound = memory_upper_bound;
        lower_bound = memory_lower_bound;
    }

    double max_fuel = 0;
    int max_index;

    for(int i = 0; i < vector_of_success.size(); i++)
    {
        if(vector_of_success.at(i).at(2) > max_fuel)
        {
            max_fuel = vector_of_success.at(i).at(2);   
            max_index = i;
        }
        //middle = vector_of_success.at(max_index).at(1);
    }
    // std::cout << "altitude: " << vector_of_success.at(max_index).at(0) 
    // << "angle: " << vector_of_success.at(max_index).at(1)
    // << "fuel: " << vector_of_success.at(max_index).at(2) << std::endl;
    return middle;
}

void TLI_timing(double altitude, double angle, double upper_bound, double lower_bound, double error)
{
    std::function<double(double)> mass_function = current_mass_function;
    std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
    std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
    std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
    std::vector<int> success;
    std::vector<double> time;
    double middle;

    while(upper_bound - lower_bound > error)
    {
        //std::cout << "kjkladsjgkldsjgkldsjlkgs     " << i << std::endl;
        Systemtest systemtest(5*24*3600, time);
        Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
        Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_funtion_for_lunar_ladning); 
        middle = (upper_bound + lower_bound) / 2; 
        std::cout << middle << std::endl; 
        systemtest.lift_off(moon, earth, rocket, altitude);
        systemtest.tilting(moon, earth, rocket, angle);
        systemtest.gravity_turn(moon, earth, rocket);
        systemtest.orbit(moon, earth, rocket, middle);
        systemtest.translunar_injection_burn(moon, earth, rocket);
        systemtest.lunar_trajectory(moon, earth, rocket, 310e3);
        time.clear();
        double distance_from_moon_center = sqrt(pow(rocket._x.back() - moon.x_cm.back(),2) + pow(rocket._y.back() - moon.y_cm.back(),2) + pow(rocket._z.back() - moon.z_cm.back(),2)) - 1737e3;

        if(distance_from_moon_center - 170e3 > 0)
        {   
            upper_bound = middle;
        }
        else
        {
            lower_bound = middle;
        }
        time.clear();
    }
    std::cout << middle << std::endl;
}

// void lunar_capture(int alt, double inital_tilt, double orbit_fraction, double lower_bound, double upper_bound, double error)
// {
//     double middle;
//     std::vector<int> altitudes;
//     for(int i = 2000e3; i > 0; i-=100000)
//         altitudes.push_back(i);

//     std::vector<std::vector<double>> vector_of_success;
//     double memory_lower_bound = lower_bound;
//     double memory_upper_bound = upper_bound;
//     double fuel;
//     bool success;
//     std::vector<double> time;
//     for(int altitude : altitudes)
//     {   
//         while(upper_bound - lower_bound > error)
//         {
//             //std::cout << "upper: " << upper_bound << std::endl;
//             //std::cout << "middle: " << (upper_bound + lower_bound)/2 << std::endl;
//             //std::cout << "lower: " << lower_bound << std::endl;
//             Systemtest systemtest(5*24*3600, time);
//             std::function<double(double)> mass_function = current_mass_function;
//             std::function<double(double)> mass_function_for_TLI = current_mass_function_for_TLI;
//             std::function<double(double)> mass_funtion_for_lunar_capture = current_mass_function_for_lunar_capture;
//             std::function<double(double)> mass_funtion_for_lunar_ladning = current_mass_function_for_lunar_ladning;
//             Planet earth(6371e3, 5.972e24, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//             Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0.0, 0.0, 0.0, 1022.0, 0.0);
//             Rocket rocket(4581000, 6371e3, 0, 0, 0.0, 0.0, 0.0, 50e3, mass_function, mass_function_for_TLI, mass_funtion_for_lunar_capture, mass_funtion_for_lunar_ladning);
//             middle = (upper_bound + lower_bound)/2;
//             std::cout << middle << std::endl;
//             systemtest.lift_off(moon, earth, rocket, alt);
//             systemtest.tilting(moon, earth, rocket, inital_tilt);
//             systemtest.gravity_turn(moon, earth, rocket);
//             systemtest.orbit(moon, earth, rocket, orbit_fraction);
//             systemtest.translunar_injection_burn(moon, earth, rocket);
//             systemtest.lunar_trajectory(moon, earth, rocket, altitude);
//             systemtest.lunar_capture(moon, earth, rocket, middle);
//             double distance_from_moon_center = sqrt(pow(rocket._x.back() - moon.x_cm.back(),2) + pow(rocket._y.back() - moon.y_cm.back(),2) + pow(rocket._z.back() - moon.z_cm.back(),2)) - 1737e3;

//             //std::cout << "mid: " << middle << std::endl;
//             if(distance_from_moon_center - altitude > 0)
//             {   
//                 lower_bound = middle;
//             }
//             else
//             {
//                 upper_bound = middle;
//             }
//             //fuel = rocket._mass_function(systemtest._time.back());
//             time.clear();
//             success = systemtest.success;
//             break;
//         }
//         vector_of_success.push_back({double(altitude), middle, fuel});
//         upper_bound = memory_upper_bound;
//         lower_bound = memory_lower_bound;
//         break;
//     }

//     double max_fuel = 0;
//     int max_index;

//     for(int i = 0; i < vector_of_success.size(); i++)
//     {
//         if(vector_of_success.at(i).at(2) > max_fuel)
//         {
//             max_fuel = vector_of_success.at(i).at(2);   
//             max_index = i;
//         }
//         //middle = vector_of_success.at(max_index).at(1);
//     }
//     // std::cout << "altitude: " << vector_of_success.at(max_index).at(0) 
//     // << "angle: " << vector_of_success.at(max_index).at(1)
//     // << "fuel: " << vector_of_success.at(max_index).at(2) << std::endl;
// }


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
    //double tilt_angle = find_tilt(0, 90, 0.1);
    //std::cout << tilt_angle << std::endl;
    systemtest.lift_off(moon, earth, rocket, 400);
    systemtest.tilting(moon, earth, rocket, 22.4121);
    systemtest.gravity_turn(moon, earth, rocket);
    
    systemtest.orbit(moon, earth, rocket, 0.605078);
    systemtest.translunar_injection_burn(moon, earth, rocket);
    systemtest.lunar_trajectory(moon, earth, rocket, 170e3);
    systemtest.lunar_capture(moon, earth, rocket);
    //system.lunar_orbit(moon, earth, rocket);
    // system.lunar_landing(moon, earth, rocket);
    //TLI_timing(400, 22.4121, 0.61, 0.6, 0.0001);
    //lunar_capture(400, 22.4121, 0.60, 0, 180, 0.1);

    return 0;
}