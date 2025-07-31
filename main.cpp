#include <iostream>
#include <string>
#include "planet.h"
#include "system.h"
#include <fstream>
#include "rocket.h"
#include <functional>
#include <cmath>

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

    std::vector<std::vector<double>> earth_3D = earth.Generate_planet_3D(earth);
    std::vector<std::vector<double>> moon_3D = moon.Generate_planet_3D(moon);

    double calculated_angle = 9.75586;
    std::vector<double> time;
    System system(5*24*3600, time);
    system.lift_off(moon, earth, rocket);
    system.tilting(moon, earth, rocket, calculated_angle);
    system.gravity_turn(moon, earth, rocket, calculated_angle);
    system.orbit(moon, earth, rocket);
    system.translunar_injection_burn(moon, earth, rocket);
    system.lunar_trajectory(moon, earth, rocket);
    //system.lunar_capture(moon, earth, rocket);
    //system.lunar_orbit(moon, earth, rocket);
    // system.lunar_landing(moon, earth, rocket);

    // for(int i = 1400; i < max_fuel_velocity; i+=10)
    // {
    //     system.gravity_turn(moon, earth, rocket, i);
    //     double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
    //     double velocity_at_altitude = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));
    //     if(system.angle(rocket) >= 89.9 && system.angle(rocket) <= 91 && velocity_at_altitude >= 7797 && velocity_at_altitude <= 7800)
    //     {
    //         optimal_fuel_velocity = i;
    //     }
    // }


    system.WriteToFile("earth_cm.txt", earth);
    system.WriteToFile("moon_cm.txt", moon);
    system.WriteToFile2("rocket_cm.txt", rocket);
    system.WriteToFile3("rocket_v.txt", rocket);
    system.WriteToFile4("akce.txt", rocket);

    // for(int i = 0; i < earth_3D.at(0).size(); i++)
    // {
    //     //std::cout << earth_3D.at(0).at(i) <<std::endl;
    //     earth_3D.at(0).at(i) = earth_3D.at(0).at(i) + earth.x_cm.back();
    //     earth_3D.at(1).at(i) = earth_3D.at(1).at(i) + earth.y_cm.back();
    // }
    
    // for(int i = 0; i < moon_3D.at(0).size(); i++)
    // {
    //     moon_3D.at(0).at(i) = moon_3D.at(0).at(i) + moon.x_cm.back();
    //     moon_3D.at(1).at(i) = moon_3D.at(1).at(i) + moon.y_cm.back();
    // }

    std::vector<std::vector<double>> earth_3d_evolved = system.Move_3d(earth_3D, earth.x_cm, earth.y_cm);
    std::vector<std::vector<double>> moon_3d_evolved = system.Move_3d(moon_3D, moon.x_cm, moon.y_cm);

    std::ofstream file;
    file.open("test.txt");

    // for(int i = 0; i < vector_3D_cordinates.size(); i++)
    // {
    //     for(int j = 0; j < vector_3D_cordinates.at(i).x.size(); j++)
    //     {
    //         file << vector_3D_cordinates.at(i).x.at(j) << std::setw(15)
    //         << vector_3D_cordinates.at(i).y.at(j) << std::setw(15)
    //         << vector_3D_cordinates.at(i).z.at(j) << std::endl;
    //     }
    // }
    // for(int i = 0; i < vector_3D_cordinates.back().x.size(); i++)
    // {
    //     file << vector_3D_cordinates.back().x.at(i) << std::setw(15)
    //     << vector_3D_cordinates.back().y.at(i) << std::setw(15)
    //     << vector_3D_cordinates.back().z.at(i) << std::endl;
    // }
    // file.close();
    // for(int i = 0; i < earth_3D.at(0).size(); i++)
    // {
    //     file << earth_3D.at(0).at(i) << std::setw(15) << earth_3D.at(1).at(i) << 
    //     std::setw(15) << earth_3D.at(2).at(i) << std::endl;
    // }
    // file.close();

    // file.open("test1.txt");
    // for(int i = 0; i < moon_3D.at(0).size(); i++)
    // {
    //     file << moon_3D.at(0).at(i) << std::setw(15) << moon_3D.at(1).at(i) << 
    //     std::setw(15) << moon_3D.at(2).at(i) << std::endl;
    // }
    // file.close();

    system.Write_evolved_3d_to_file("test.txt", earth_3d_evolved, earth_3d_evolved.size(), earth_3D.at(2));
    system.Write_evolved_3d_to_file("test1.txt", moon_3d_evolved, moon_3d_evolved.size(), moon_3D.at(2));

    return 0;
}