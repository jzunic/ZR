#include <vector>
#include <cmath>
#include "planet.h"


class System
{
    private:
        double m_max_time, m_grav_const = 6.6743e-11, m_dt = 3600;
        //std::vector<coords> vector_of_3D_coordinates;
        double m_x_acceleration(double mass, Planet& object_1, Planet& object_2); 
        double m_y_acceleration(double mass, Planet& object_1, Planet& object_2); 

    public:
        System(double max_time, std::vector<double> time);
        void move(Planet& object_1, Planet& object_2);
        std::vector<double> _time;
        void WriteToFile(std::string filename, Planet object);
        std::vector<std::vector<double>> Move_3d
        (
            std::vector<std::vector<double>>& object_3d, 
            std::vector<double> object_cm_x, 
            std::vector<double> object_cm_y
        );
        std::vector<std::vector<double>> vector_of_evolved_3d_coords, temp;
        void Write_evolved_3d_to_file
        (
        std::string filename,
        std::vector<std::vector<double>> coords_3d,
        int size, std::vector<double> z_3d
        );
        
};

