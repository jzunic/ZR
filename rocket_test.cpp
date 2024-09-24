#include <iostream>
#include <vector>
#include "rocket.h"
#include <fstream>


double mass_function(double t)
{
    return (131e3+2286e3 - 13178*t);
}

void write(Rocket& rocket)
{
    std::ofstream file;
    
    file.open("filename.txt");

    for(int i = 0; i < rocket._x.size(); i++)
    {
        file << rocket._x.at(i) << std::setw(15) << rocket._y.at(i) << std::setw(15) << rocket._z.at(i) <<std::endl;
    }

    file.close();
}

double numerical_derivative(std::function<double(double)> function, std::vector<double> t, double dt)
{
    double derviative = (function(t.back()+dt) - function(t.back()-dt))/(2*dt);
    return derviative;
}

double acceleration_x(double current_time, Rocket& rocket)
{
    double drag_factor = -rocket._v_x.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2));
    double gravity_factor = -6.6743e-11*5.972e24*rocket._x.back()/pow(sqrt(pow(rocket._x.back(), 2) + pow(rocket._y.back(), 2) + pow(rocket._z.back(), 2)), 3);
    //std::cout << rocket._v_x.back() << std::endl;
    return(drag_factor + gravity_factor);
}

double acceleration_y(double current_time, Rocket& rocket)
{
    double drag_factor = -rocket._v_y.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2));
    double gravity_factor = -6.6743e-11*5.972e24*rocket._y.back()/pow(sqrt(pow(rocket._x.back(), 2) + pow(rocket._y.back(), 2) + pow(rocket._z.back(), 2)), 3);
    return(drag_factor + gravity_factor);
}

double acceleration_z(double current_time, Rocket& rocket, double derivative)
{
    double drag_factor = -rocket._v_z.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2));
    double gravity_factor = -6.6743e-11*5.972e24*rocket._z.back()/pow(sqrt(pow(rocket._x.back(), 2) + pow(rocket._y.back(), 2) + pow(rocket._z.back(), 2)), 3);
    double thrust_factor = -2580*derivative/mass_function(current_time);
    std::cout << drag_factor <<", "<< gravity_factor << ", " << thrust_factor<< std::endl;
    return(drag_factor + gravity_factor + thrust_factor);
}

int main()
{
    Rocket rocket = Rocket(1000, 0, 0, 6371e3, 0, 0, 0, 500, mass_function);
    double t_max = 200, dt = 1;
    std::vector<double> t;
    t.push_back(0);

    while(t.back() < t_max)
    {
        rocket._a_x.push_back(acceleration_x(t.back(), rocket));
        rocket._a_y.push_back(acceleration_y(t.back(), rocket));
        rocket._a_z.push_back(acceleration_z(t.back(), rocket, numerical_derivative(mass_function, t, dt)));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*dt);

        t.push_back(t.back() + dt);

        if(mass_function(t.back())<=0)
        {
            break;
        }
    }

    write(rocket);
}