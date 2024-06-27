#include "planet.h"
#include <vector>
#include <fstream>
#include <cmath>

Planet::Planet(
    double radius, 
    double mass, 
    double distance_from_earth,
    double x_cm_0,
    double y_cm_0,
    double v_x_cm_0,
    double v_y_cm_0
)
{
    m_radius = radius;
    m_distance_from_earth = distance_from_earth;
    _mass = mass;
    x_cm.push_back(x_cm_0);
    y_cm.push_back(y_cm_0);
    v_x_cm.push_back(v_x_cm_0);
    v_y_cm.push_back(v_y_cm_0);

}

std::vector<std::vector<double>> Planet::Generate_planet_3D(Planet& planet)
{
    x_3d.push_back(m_radius*sin(0)*cos(0));
    y_3d.push_back(m_radius*sin(0)*sin(0));
    z_3d.push_back(m_radius*cos(0));


    for(int longitude = 1; longitude <= 179; longitude+=20)
    {
        for(int latitude = 0; latitude <= 359; latitude+=25)
        {
            x_3d.push_back(m_radius*sin(degree_to_radians(longitude))*cos(degree_to_radians(latitude)));
            y_3d.push_back(m_radius*sin(degree_to_radians(longitude))*sin(degree_to_radians(latitude)));
            z_3d.push_back(m_radius*cos(degree_to_radians(longitude)));
        }
    }

    // m_coordinates.x = m_x_coord;
    // m_coordinates.y = m_y_coord;
    // m_coordinates.z = m_z_coord;

    // return m_coordinates;
    coords_3d.push_back(x_3d);
    coords_3d.push_back(y_3d);
    coords_3d.push_back(z_3d);
    return coords_3d;
}

void Planet::Write_to_file(std::string file_name, std::vector<std::vector<double>> planet_3D)
{
    std::ofstream file;
    file.open(file_name);
    
    for(int i = 0; i < x_3d.size(); i++)
    {   //PROMIJENI OVO IZ STRUCT U VECTOR OPET NE ZNAN NI SAM STA SE DOGADA A JA SAN PISA KOD
        file << x_3d.at(i) << std::setw(15) << y_3d.at(i) << std::setw(15) << z_3d.at(i) << "\n";
    }

    file.close();
}

double Planet::degree_to_radians(double degree)
{
    return(degree*(M_PI/180));
}