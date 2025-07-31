#include <vector>
#include <cmath>
#include "planet.h"
#include "rocket.h"
#include <fstream>
#include <iomanip>
#include <string>


class Systemtest
{
    private:
        double m_max_time, m_grav_const = 6.6743e-11, m_dt = 0.1, m_remember_thrust_influence_x, m_remember_thrust_influence_y, m_remember_thrust_influence_z;
        double m_remember_gravity_influence_after_turn_x, m_remember_gravity_influence_after_turn_y, m_remember_gravity_influence_after_turn_z;
        double m_remember_gravity_turn_thrust_x, m_remember_gravity_turn_thrust_y, m_remember_gravity_turn_thrust_z;
        double theta = M_PI/2;
        double phi = 0;
        double vx = 0;
        double vy = 0;
        double vz = 1;
        //std::vector<coords> vector_of_3D_coordinates;
        double m_x_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time); 
        double m_y_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time); 
        double m_z_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time);
        double m_rocket_x_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_rocket_y_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_rocket_z_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_numerical_derivative(std::function<double(double)> function, double time);
        double m_rocket_x_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_rocket_y_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_rocket_z_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time);
        double m_rocket_x_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_y_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_z_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_x_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_y_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_z_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_x_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_y_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_z_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_x_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_y_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_rocket_z_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle);
        double m_orbital_velocity(double distance, double mass, double radius);
        double m_one_orbit(double a);
        void move_planets(Planet& object_1, Planet& object_2, Rocket& rocket, double current_time);
        void move_rocket(Rocket& rocket, double accelerationx, double accelerationy, double accelerationz);
        std::vector<double> m_cross_product(std::vector<double> v1, std::vector<double> v2);
        std::ofstream tempfile;
        //double phi(double old_phi, double angle);



    public:
        Systemtest(double max_time, std::vector<double> time);
        void lift_off(Planet& object_1, Planet& object_2, Rocket& rocket, int altitude);
        void tilting(Planet& object_1, Planet& object_2, Rocket& rocket, double trial_angle);
        void gravity_turn(Planet& object_1, Planet& object_2, Rocket& rocket);
        void orbit(Planet& object_1, Planet& object_2, Rocket& Rocket, double orbit_fraction);
        void translunar_injection_burn(Planet& object_1, Planet& object_2, Rocket& rocket);
        void lunar_trajectory(Planet& object_1, Planet& object_2, Rocket& rocket);
        void lunar_capture(Planet& object_1, Planet& object_2, Rocket& rocket);
        void lunar_orbit(Planet& object_1, Planet& object_2, Rocket& rocket);
        void lunar_landing(Planet& object_1, Planet& object_2, Rocket& rocket);
        std::vector<double> _time;
        void WriteToFile(std::string filename, Planet object);
        void WriteToFile2(std::string filename, Rocket rocket);
        void WriteToFile3(std::string filename, Rocket rocket);
        std::vector<std::vector<double>> Move_3d(
            std::vector<std::vector<double>>& object_3d, 
            std::vector<double> object_cm_x, 
            std::vector<double> object_cm_y
        );
        std::vector<std::vector<double>> vector_of_evolved_3d_coords, temp;
        void Write_evolved_3d_to_file(
            std::string filename,
            std::vector<std::vector<double>> coords_3d,
            int size, std::vector<double> z_3d
        );
        double angle(Rocket rocket);
        double angle2(double x1, double x2, double y1, double y2, double z1, double z2);
        double angle3(double x1, double x2, double y1, double y2, double z1, double z2);
        void WriteToFile4(std::string filename, Rocket rocket);
        double a_x_cm_1 = 0, a_y_cm_1 = 0, a_z_cm_1 = 0;
        double a_x_cm_2 = 0, a_y_cm_2 = 0, a_z_cm_2 = 0;
        double a_x_cm = 0, a_y_cm = 0, a_z_cm = 0;
        double tilt_angle, angle_final;
        double a_x_0, a_y_0, a_z_0;
        double time_for_one_orbit;
        bool success;
};

