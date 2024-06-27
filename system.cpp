#include <vector>
#include <cmath>
#include "system.h"
#include "planet.h"
#include <string>
#include <fstream>
#include <iostream>

System::System(double max_time, std::vector<double> time)
{
    m_max_time = max_time;
    _time = time;
    _time.push_back(0);
}

double System::m_x_acceleration(double mass, Planet& object_1, Planet& object_2)
{
    double numerator = -m_grav_const*mass*(object_1.x_cm.back() - object_2.x_cm.back());
    double denominator = pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2)), 3);
    //std::cout << "x: " << numerator << "/" << denominator << "=" << numerator/denominator << std::endl;
    return(numerator/denominator);
}

double System::m_y_acceleration(double mass, Planet& object_1, Planet& object_2)
{
    double numerator = -m_grav_const*mass*(object_1.y_cm.back() - object_2.y_cm.back());
    double denominator = pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2)), 3);
    //std::cout << "y: " << numerator << "/" << denominator << "=" << numerator/denominator << std::endl;
    return(numerator/denominator);
}

void  System::move(Planet& object_1, Planet& object_2)
{
    while (_time.back() < m_max_time)
    {    
        //std::cout << object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt <<std::endl;
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt);        
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);

        // vector_of_3D_coordinates.push_back(object_1.Generate_planet_3D(object_1));
        // vector_of_3D_coordinates.push_back(object_2.Generate_planet_3D(object_2));
        
        _time.push_back(_time.back() + m_dt);
    }

    //return vector_of_3D_coordinates;

}

void System::WriteToFile(std::string filename, Planet object)
{
    std::ofstream file;
    
    file.open(filename);

    for(int i = 0; i < object.x_cm.size(); i++)
    {
        file << object.x_cm.at(i) << std::setw(15) << object.y_cm.at(i) <<std::endl;
    }

    file.close();
}

std::vector<std::vector<double>> System::Move_3d(std::vector<std::vector<double>>& object_3d, std::vector<double> object_cm_x, std::vector<double> object_cm_y)
{
    vector_of_evolved_3d_coords.clear();

    for(int j = 0; j < object_cm_x.size(); j+=10)
    {
        temp = object_3d;
        for(int i = 0; i < object_3d.at(0).size(); i++)
        {
            object_3d.at(0).at(i) = object_3d.at(0).at(i) + object_cm_x.at(j);
            object_3d.at(1).at(i) = object_3d.at(1).at(i) + object_cm_y.at(j);
        }
        vector_of_evolved_3d_coords.push_back(object_3d.at(0));
        vector_of_evolved_3d_coords.push_back(object_3d.at(1));
        object_3d.at(0) = temp.at(0);
        object_3d.at(1) = temp.at(1);
    }
    return vector_of_evolved_3d_coords;
}

void System::Write_evolved_3d_to_file(std::string filename, std::vector<std::vector<double>> coords_3d, int size, std::vector<double> z_3d)
{
    std::ofstream file;
    file.open(filename);

    for(int i = 0; i < size; i+=2)
    {
        //std::cout << coords_3d.size() << std::endl;
        for(int j = 0; j < coords_3d.at(i).size(); j++)
        {
            file << coords_3d.at(i).at(j) << std::setw(15) << coords_3d.at(i+1).at(j) << std::setw(15)
            << z_3d.at(j) << std::endl;
        }
    }
    file.close();
}