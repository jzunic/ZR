#ifndef PLANET_H
#define PLANET_H
#include <vector>
#include <cmath>

// struct coords 
// {
//     std::vector<double> x;
//     std::vector<double> y;
//     std::vector<double> z;
// };

class Planet
{
    private:
        double m_radius, m_distance_from_earth, m_earth_radius = 6371e3;
        //std::vector<double> m_x_coord, m_y_coord, m_z_coord;
        //coords m_coordinates;

    public:
        Planet(
            double radius, 
            double mass, 
            double distance_from_earth, 
            double x_cm_0,
            double y_cm_0,
            double v_x_cm_0,
            double v_y_cm_0
        );
        std::vector<std::vector<double>> Generate_planet_3D(Planet& planet);
        void Write_to_file(std::string file_name, std::vector<std::vector<double>> planet_3D);
        double degree_to_radians(double degree), _mass;
        std::vector<double> x_cm, y_cm, z_cm, v_x_cm, v_y_cm, a_x_cm, a_y_cm, time, x_3d, y_3d, z_3d;
        std::vector<std::vector<double>> coords_3d;
};

#endif