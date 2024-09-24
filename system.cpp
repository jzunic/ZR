#include <vector>
#include <cmath>
#include "system.h"
#include "planet.h"
#include <string>
#include <fstream>
#include <iostream>
#include "rocket.h"
#include <iomanip>
#include <algorithm>

System::System(double max_time, std::vector<double> time)
{
    m_max_time = max_time;
    _time = time;
    _time.push_back(0);
}

/*funkcija koja računa numeričke derivacije*/
double System::m_numerical_derivative(std::function<double(double)> function, double time)
{
    double derviative = (function(time+m_dt) - function(time-m_dt))/(2*m_dt);
    return derviative;
}

double System::m_orbital_velocity(double distance)
{
    double orbital_velocity = sqrt(m_grav_const*7.35e22/(1737e3 + distance));
    return orbital_velocity;
}

/*funkcija koja računa x komponentnu akceleracije planeta*/
double System::m_x_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.x_cm.back() - object_2.x_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    //double grav_influence_rocket_on_planet = 0;
    double grav_influence_rocket_on_planet = -m_grav_const*rocket._mass_function(current_time)*(object_1.x_cm.back() - rocket._x.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout << "x: " << numerator << "/" << denominator << "=" << numerator/denominator << std::endl;
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/*funkcija koja računa y komponentnu akceleracije planeta*/
double System::m_y_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.y_cm.back() - object_2.y_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    double grav_influence_rocket_on_planet = -m_grav_const*rocket._mass_function(current_time)*(object_1.y_cm.back() - rocket._y.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    //double grav_influence_rocket_on_planet = 0;
    //std::cout << "y: " << numerator << "/" << denominator << "=" << numerator/denominator << std::endl;
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/*funkcija koja računa z komponentnu akceleracije planeta*/
double System::m_z_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.z_cm.back() - object_2.z_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    double grav_influence_rocket_on_planet = -m_grav_const*rocket._mass_function(current_time)*(object_1.z_cm.back() - rocket._z.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    //double grav_influence_rocket_on_planet = 0;
    //std::cout << "y: " << numerator << "/" << denominator << "=" << numerator/denominator << std::endl;
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/**
 * Funkcija koja računa x komponentu akceleracije rakete
 * Početna točka na Zemlji određuje kutove theta i fi u sfernim koordinata,
 * definirani su u system.h
 * Kutovi se koriste za izračunati x komponentnu potrebnu za radijalno lansiranje
 * Analogno za sljedeće dvije funkcije
*/
double System::m_rocket_x_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout <<"x: " << grav_influence_planet1_on_rocket << ", " << grav_influence_planet2_on_rocket << std::endl;
    // if (rocket._x.back() < 6371e3 + 100e3) // zemljin radijus + gornja granica atmosfere
    // {
    //     drag_influence = -rocket._v_x.back()*(0.47/(rocket._mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2)); //0.47 je koef trenja za kuglu, lako mijenjat kasnije
    // }
    if (rocket._mass_function(current_time) > 100e3)
    {
        //std::cout << m_numerical_derivative(rocket._mass_function, _time.back()) << ", " << rocket._mass_function(_time.back()) << std::endl;
        thrust_influence = thrust*cos(phi)*sin(theta);
    }
    //m_remember_thrust_influence_x = thrust_influence;
    //std::cout << "x: " << thrust_influence << ", " << grav_influence_planet2_on_rocket << ", " << rocket._mass_function(current_time) << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double System::m_rocket_y_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
    //std::cout<<"y: " << grav_influence_planet1_on_rocket << ", " << grav_influence_planet2_on_rocket << std::endl;
    // if (rocket._y.back() < 6371e3 + 100e3) // zemljin radijus + gornja granica atmosfere
    // {
    //     drag_influence = -rocket._v_y.back()*(0.47/(rocket._mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2)); //0.47 je koef trenja za kuglu, lako mijenjat kasnije
    // }
    if (rocket._mass_function(current_time) > 100e3 /*&& distance_from_earth_center - 6371e3 < 100e3*/)
    {
        thrust_influence = thrust*sin(phi)*sin(theta);
    }

    //m_remember_thrust_influence_y = thrust_influence;
    // else if(rocket._mass_function(current_time) > 0 && distance_from_earth_center - 6371e3 >= 100e3)
    // {
    //     std::cout << "bla" << std::endl;
    //     thrust_influence = 20000*m_numerical_derivative(rocket._mass_function, _time.back())/rocket._mass_function(_time.back());
    // }
    //std::cout << "y: " << thrust_influence << ", " << grav_influence_planet2_on_rocket << ", " << rocket._mass_function(current_time) << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double System::m_rocket_z_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout<<"z: " << grav_influence_planet1_on_rocket << ", " << grav_influence_planet2_on_rocket << std::endl;
    // if (rocket._z.back() < 6371e3 + 100e3) // zemljin radijus + gornja granica atmosfere
    // {
    //     drag_influence = -rocket._v_z.back()*(0.47/(rocket._mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2)); //0.47 je koef trenja za kuglu, lako mijenjat kasnije
    // }
    //std::cout << drag_influence << ", ";
    if (rocket._mass_function(current_time) > 100e3) // ako ima goriva i ako je udaljenost od centra ta i ta
    {
        //std::cout<<rocket._z.back()<<std::endl;
        thrust_influence =  thrust*cos(theta);/*-(grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);*/
        //std::cout << rocket._mass_function(current_time) << std::endl;
    }
    //m_remember_thrust_influence_z = thrust_influence;
    // else if(rocket._mass_function(current_time) > 0 && distance_from_earth_center - 6371e3 >= 100e3)
    // {
    //     thrust_influence = -2000*m_numerical_derivative(rocket._mass_function, _time.back())/rocket._mass_function(_time.back());
    // }
    //std::cout << thrust_influence << std::endl;
    //std::cout<<"pogon: "<<thrust_influence<< "\n" << "otpor zraka: " << drag_influence << "\n" << "grav1: " << grav_influence_planet1_on_rocket << "\n" << "grav2: " << grav_influence_planet2_on_rocket << "\n" << "---" << std::endl;
    //std::cout << "z: " << thrust_influence << ", " << grav_influence_planet2_on_rocket << ", " << rocket._mass_function(current_time) << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

/**
 * Samo gravitacijski utjecaji na raketu
 * Soriste se kada raketa uđe u orbitu
*/
double System::m_rocket_x_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout << "x: " << thrust_influence << ", " << grav_influence_planet1_on_rocket << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double System::m_rocket_y_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout << "y: " << thrust_influence << ", " << grav_influence_planet1_on_rocket << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double System::m_rocket_z_acceleration_engine_off(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time)
{
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    //std::cout << "z: " << thrust_influence << ", " << grav_influence_planet2_on_rocket + grav_influence_planet1_on_rocket << std::endl;
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

/*mislim da je ostalo od stare verzije koda i da se ne koristi nigdje*/
double System::angle(Rocket& rocket)
{
    double scalar_product = 6.19696e6*rocket._v_x.back() + (-1.09269e6)*rocket._v_y.back() + 996644.0*rocket._v_z.back();
    double scalar_length = 6371e3*sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));
    double angle_in_radians = acos(scalar_product / scalar_length);
    double angle_in_degrees = angle_in_radians*180/M_PI;
    return (angle_in_degrees);
}

/*računa kut između 2 vektora i vraća ga u stupnjevima*/
double System::angle2(double x1, double x2, double y1, double y2, double z1, double z2)
{
    /* skalarni produkt s nul vektorom daje 0, ali kut nije definiran pa cu ja rec da je 0*/
    //std::cout << x1 << ", " << y1 << ", " << z1 << ", " << x2 << ", " << y2 << ", " << z2 << std::endl;
    double scalar_product = x1*x2 + y1*y2 + z1*z2;
    double scalar_length = sqrt(x1*x1 + y1*y1 + z1*z1)*sqrt(x2*x2 + y2*y2 + z2*z2);
    double angle_in_degrees = 0;
    if ((x1 == 0 && y1 == 0 && z1 == 0) || (x2 == 0 && y2 == 0 && z2 == 0))
    {
        angle_in_degrees = 0;
    }
    else
    {
        //std::cout << "ha" << std::endl;
        double angle_in_radians = acos(scalar_product / scalar_length);
        angle_in_degrees = angle_in_radians*180/M_PI;
        //std::cout << angle_in_degrees << std::endl;
    }
    return (angle_in_degrees);
}

/*računa kut između 2 vektora i vraća ga u radijanima*/
double System::angle3(double x1, double x2, double y1, double y2, double z1, double z2)
{
    /*skalarni produkt s nul vektorom daje 0, ali kut nije definiran pa cu ja reci da je 0*/
    //std::cout << x1 << ", " << y1 << ", " << z1 << ", " << x2 << ", " << y2 << ", " << z2 << std::endl;
    double scalar_product = x1*x2 + y1*y2 + z1*z2;
    double scalar_length = sqrt(x1*x1 + y1*y1 + z1*z1)*sqrt(x2*x2 + y2*y2 + z2*z2);
    double angle_in_radians = 0;
    if ((x1 == 0 && y1 == 0 && z1 == 0) || (x2 == 0 && y2 == 0 && z2 == 0))
    {
        angle_in_radians = 0;
    }
    else
    {
        //std::cout << "ha" << std::endl;
        angle_in_radians = acos(scalar_product / scalar_length);
        //std::cout << angle_in_degrees << std::endl;
    }
    return (angle_in_radians);
}

/**
 * Računa x komponentnu akceleracije, ali rotirane oko određene osi za određeni kut
 * Prvo se računa akceleracija koju bi raketa imala u slučaju da nema rotacije
 * Potom taj vektor rotira oko proizvoljne osi za proizvoljni kut
 * Komponente novog vektora računaju se prema Rodriguesovoj formuli za rotaciju
 * Vektor oko kojeg se rotira definiran je u system.h
*/
double System::m_rocket_x_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    //std::cout << thrust << std::endl;
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function(current_time) > 100e3)
    {
        thrust_influence = thrust_x*cos(tilt_angle) + (vy*thrust_z - vz*thrust_y)*sin(tilt_angle) + vx*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //std::cout <<"x: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << ", " << tilt_angle << ", "<< thrust << std::endl;
    //m_remember_gravity_turn_thrust_x = thrust_influence;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_y_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function(current_time) > 100e3)
    {
        thrust_influence = thrust_y*cos(tilt_angle) + (vz*thrust_x - vx*thrust_z)*sin(tilt_angle) + vy*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //m_remember_gravity_turn_thrust_y = thrust_influence;
    //std::cout <<"y: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_z_acceleration_tilted(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function, current_time)/rocket._mass_function(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function(current_time) > 100e3)
    {
        thrust_influence = thrust_z*cos(tilt_angle) + (vx*thrust_y-vy*thrust_x)*sin(tilt_angle) + vz*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"z: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

/**
 * Računa thrust vektor za Trans-lunar injection
 * radi na istom principu kao i tilted funkcije prije, samo što koristi drugačiju mass funkciju
 */
double System::m_rocket_x_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_TLI, current_time)/rocket._mass_function_for_TLI(current_time);
    //std::cout << thrust << std::endl;
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_TLI(current_time) > 100e3)
    {
        thrust_influence = thrust_x*cos(tilt_angle) + (vy*thrust_z - vz*thrust_y)*sin(tilt_angle) + vx*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"x: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_y_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_TLI, current_time)/rocket._mass_function_for_TLI(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_TLI(current_time) > 100e3)
    {
        thrust_influence = thrust_y*cos(tilt_angle) + (vz*thrust_x - vx*thrust_z)*sin(tilt_angle) + vy*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"y: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_z_acceleration_TLI(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_TLI, current_time)/rocket._mass_function_for_TLI(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_TLI(current_time) > 100e3)
    {
        thrust_influence = thrust_z*cos(tilt_angle) + (vx*thrust_y-vy*thrust_x)*sin(tilt_angle) + vz*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
    }
    //m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"z: " <<thrust_influence << ", " <<  grav_influence_planet2_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}
/**
* analogno
*bilo je potrebno povećati brzinu kočenja
*/
double System::m_rocket_x_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -4300*m_numerical_derivative(rocket._mass_function_for_lunar_capture, current_time)/rocket._mass_function_for_lunar_capture(current_time);
    //std::cout << thrust << std::endl;
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_capture(current_time) > 85e3)
    {
        thrust_influence = (thrust_x*cos(tilt_angle) + (vy*thrust_z - vz*thrust_y)*sin(tilt_angle) + vx*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"x: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_y_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -4300*m_numerical_derivative(rocket._mass_function_for_lunar_capture, current_time)/rocket._mass_function_for_lunar_capture(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_capture(current_time) > 85e3)
    {
        thrust_influence = (thrust_y*cos(tilt_angle) + (vz*thrust_x - vx*thrust_z)*sin(tilt_angle) + vy*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"y: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_z_acceleration_lunar_capture(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -4300*m_numerical_derivative(rocket._mass_function_for_lunar_capture, current_time)/rocket._mass_function_for_lunar_capture(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_capture(current_time) > 85e3)
    {
        thrust_influence = (thrust_z*cos(tilt_angle) + (vx*thrust_y-vy*thrust_x)*sin(tilt_angle) + vz*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"z: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);
}
double System::m_rocket_x_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_lunar_landing, current_time)/rocket._mass_function_for_lunar_landing(current_time);
    //std::cout << thrust << std::endl;
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_landing(current_time) > 50e3)
    {
        thrust_influence = (thrust_x*cos(tilt_angle) + (vy*thrust_z - vz*thrust_y)*sin(tilt_angle) + vx*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"x: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_y_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_lunar_landing, current_time)/rocket._mass_function_for_lunar_landing(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_landing(current_time) > 50e3)
    {
        thrust_influence = (thrust_y*cos(tilt_angle) + (vz*thrust_x - vx*thrust_z)*sin(tilt_angle) + vy*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"y: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}

double System::m_rocket_z_acceleration_lunar_landing(Planet planet1, Planet planet2, Rocket rocket, double current_time, double tilt_angle)
{
    double thrust = -3500*m_numerical_derivative(rocket._mass_function_for_lunar_landing, current_time)/rocket._mass_function_for_lunar_landing(current_time);
    double drag_influence = 0;
    double thrust_influence = 0;
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    if(rocket._mass_function_for_lunar_landing(current_time) > 50e3)
    {
        thrust_influence = (thrust_z*cos(tilt_angle) + (vx*thrust_y-vy*thrust_x)*sin(tilt_angle) + vz*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle)));
    }
    m_remember_gravity_turn_thrust_z = thrust_influence;
    //std::cout <<"z: " <<thrust_influence << ", " <<  grav_influence_planet1_on_rocket << std::endl;
    return(thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket + drag_influence);

}
/*računa vektorski produkt 2 vektora, ne koristi se nigdje u trenutačnoj verziji koda */
std::vector<double> System::m_cross_product(std::vector<double> v1, std::vector<double> v2)
{
    std::vector<double> v3;
    double v3x = v1.at(1)*v2.at(2) - v1.at(2)*v2.at(1);
    double v3y = v1.at(2)*v2.at(0) - v1.at(0)*v2.at(2);
    double v3z = v1.at(0)*v2.at(1) - v1.at(1)*v2.at(0);
    v3 = {v3x, v3y, v3z};
    return v3;
}

/**
 * Funckija koja ažurira radijalne akceleracije, brzine i položaje
 * Daje prvu fazu lansiranja koja se sastoji od vertikalnog ispaljivanja u odnosu na promatrača na Zemlji
 * i radijalno za promatrače iz svemira
 * Funkcija se zaustavlja kada se postignu željeni uvjeti za početak gravity turna.
 * Ne mogu shvatiti koji su to uvjeti
 */
void System::lift_off(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    while (_time.back() < m_max_time)
    {    
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt); 

        //double velocity = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));


        rocket._a_x.push_back(m_rocket_x_acceleration(object_1, object_2, rocket, _time.back()));
        rocket._a_y.push_back(m_rocket_y_acceleration(object_1, object_2, rocket, _time.back()));
        rocket._a_z.push_back(m_rocket_z_acceleration(object_1, object_2, rocket, _time.back()));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        // vector_of_3D_coordinates.push_back(object_1.Generate_planet_3D(object_1));
        // vector_of_3D_coordinates.push_back(object_2.Generate_planet_3D(object_2));
        
        _time.push_back(_time.back() + m_dt);

        double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
        //std::cout << distance_from_earth_center - 6371e3 << ", " << rocket._mass_function(_time.back()) << std::endl;
        if (distance_from_earth_center - 6371e3 >= 1e3 || distance_from_earth_center - 6370e3 <= 0)
        {
            //std::cout << angle(rocket) << std::endl;
            std::cout << "Lift off ends: " <<_time.back() << std::endl;
            std::cout << distance_from_earth_center - 6370e3 << std::endl;
            break;
        }
    }
    //return vector_of_3D_coordinates;
}

/**
 * Funkcija koja vrši početni korak gravity turna
 * Vektor pogona rakete rotira se oko proizvoljne osi za proizvoljni kut
 * Sada vektor pogona i vektor gravitacije nisu kolinerani te gravitacija nastoji rotirati 
 * putanju rakete
 * Funkcija staje kada se postigne željeni kut ili neki kut njemu blizak, ovisno o koraku
 * koji se koristi za promjenu kuta
 */
void System::tilting(Planet& object_1, Planet& object_2, Rocket& rocket)
{

    double acceleration_before_turn_x = rocket._a_x.back();
    double acceleration_before_turn_y = rocket._a_y.back();
    double acceleration_before_turn_z = rocket._a_z.back();

    /*postavljam na 0 zbog uvjeta u angle2 i angle3 funkcijama, s ovim će vratiti u prvom pozivanju 0*/
    double acceleration_during_turn_x = 0, acceleration_during_turn_y = 0, acceleration_during_turn_z = 0;

    double tilt_angle;
    double counter = 1;


    //std::cout << velocity_before_turn_x << ", " << velocity_before_turn_y << ", " << velocity_before_turn_z << std::endl;

    while(_time.back() <= m_max_time)
    {

        tilt_angle = angle2(acceleration_before_turn_x, acceleration_during_turn_x, acceleration_before_turn_y, acceleration_during_turn_y, acceleration_before_turn_z, acceleration_during_turn_z);
        //std::cout << "kut izmedu prvotne radijalne akceleracije i nove rotirane: " << tilt_angle << "\n"<< std::endl;
        if(tilt_angle <= 55 || tilt_angle == 0) // ako je kut otklona manji od 10 stupnjeva, koristim matricu rotacije oko z osi za rotirati vektor akceleracije za kut 0.1, counter je tu samo osiguranje da se ne izvrsi if blok previse puta 
        {
            object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
            object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
            object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

            object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
            object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
            object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

            object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
            object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
            object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
            
            
            object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
            object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
            object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

            object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
            object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
            object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

            object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
            object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
            object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt); 

            rocket._a_x.push_back(m_rocket_x_acceleration_tilted(object_1, object_2, rocket, _time.back(), object_1.degree_to_radians(counter*0.2)));
            rocket._a_y.push_back(m_rocket_y_acceleration_tilted(object_1, object_2, rocket, _time.back(), object_1.degree_to_radians(counter*0.2)));
            rocket._a_z.push_back(m_rocket_z_acceleration_tilted(object_1, object_2, rocket, _time.back(), object_1.degree_to_radians(counter*0.2)));

            rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
            rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
            rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

            rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
            rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
            rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

            acceleration_during_turn_x = rocket._a_x.back();
            acceleration_during_turn_y = rocket._a_y.back();
            acceleration_during_turn_z = rocket._a_z.back();

            _time.push_back(_time.back() + m_dt);
            counter++;

        }
        else
        {
            double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
            //std::cout << m_remember_thrust_influence_after_turn_x;
            std::cout << distance_from_earth_center - 6370e3 << std::endl;
            std::cout << "Tilting process ends: " <<_time.back() << std::endl;
            std::cout << tilt_angle << std::endl;
            break;
        }
    }
}

/**
 * Funkcija koja bi trebala dovršiti uspon do orbite oko Zemlje
 * Svakim korakom vektor pogona rakete se namješta da bude u istom smjeru kao akceleracija 
 * iz prethodnog koraka
 */
void System::gravity_turn(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double counter = 0;

    double tilt_angle = angle3(rocket._a_x.at(0), rocket._a_x.back(), rocket._a_y.at(0), rocket._a_y.back(), rocket._a_z.at(0), rocket._a_z.back());
    double angle = angle2(rocket._x.back(), rocket._v_x.back(), rocket._y.back(), rocket._v_y.back(), rocket._z.back(), rocket._v_z.back());
    
    
    while(_time.back() < m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);     
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        
        rocket._a_x.push_back(m_rocket_x_acceleration_tilted(object_1, object_2, rocket, _time.back(), tilt_angle)); 
        rocket._a_y.push_back(m_rocket_y_acceleration_tilted(object_1, object_2, rocket, _time.back(), tilt_angle));
        rocket._a_z.push_back(m_rocket_z_acceleration_tilted(object_1, object_2, rocket, _time.back(), tilt_angle));
        

        //std::cout << velocity_after_turn_x << ", " << velocity_after_turn_y << ", " << velocity_after_turn_z << std::endl;
        
        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt); 

        _time.push_back(_time.back() + m_dt);
        tilt_angle = angle3(rocket._a_x.at(0), rocket._v_x.back(), rocket._a_y.at(0), rocket._v_y.back(), rocket._a_z.at(0), rocket._v_z.back());
        angle = angle2(rocket._x.back(), rocket._v_x.back(), rocket._y.back(), rocket._v_y.back(), rocket._z.back(), rocket._v_z.back());
        counter++;
        
        double acceleration_value = sqrt(pow(rocket._a_x.back(), 2) + pow(rocket._a_y.back(), 2) + pow(rocket._a_z.back(), 2));
        double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
        double velocity = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));
        /*orbitalna brzina i kut*/
        if(rocket._mass_function(_time.back()) <= 100e3 || (angle >= 90 && velocity >=7.9e3))
        {
            std::cout << rocket._mass_function(_time.back()) << std::endl;
            std::cout << "angle: "<< angle << std::endl;
            std::cout << distance_from_earth_center - 6370e3 << std::endl;
            std::cout << "gravity turn ends: " <<_time.back() << std::endl;
            break;
        }
        //break;
    }
}

void System::orbit(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    while(_time.back() <= m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        rocket._a_x.push_back(m_rocket_x_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_y.push_back(m_rocket_y_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_z.push_back(m_rocket_z_acceleration_engine_off(object_1, object_2, rocket, _time.back()));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        _time.push_back(_time.back() + m_dt);
        double distance_from_earth_center = sqrt(pow(rocket._x.back(),2) + pow(rocket._y.back(),2) + pow(rocket._z.back(),2));
        double angle_between_rocket_and_moon = angle3(rocket._x.back(), object_1.x_cm.back(), rocket._y.back(), object_1.y_cm.back(), rocket._z.back(), object_1.z_cm.back());
        /*trenutak izbacivanja rakete iz Zemljine orbite znatno utječe na putanju*/
        if(distance_from_earth_center - 6370e3 <=0 || (_time.back() >= 147.63*60))
        {
            m_dt = 1;
            //std::cout << distance_from_earth_center-6370e3 << std::endl;
            std::cout << "TLI burn begins: " <<_time.back() << std::endl;
            break;
        }
    }
}
    
void System::translunar_injection_burn(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double time_for_new_mass_function = 0;
    double velocity = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));
    double tilt_angle = angle3(rocket._a_x.at(0), rocket._v_x.back(), rocket._a_y.at(0), rocket._v_y.back(), rocket._a_z.at(0), rocket._v_z.back());

    while(_time.back() < m_max_time)
    {
        /*veća brzina previše otvara putanju dok manja brzina prebrzo zatvara elipsu*/
        if(velocity <= 10.66e3)
        {
            //std::cout << velocity << ", " << rocket._mass_function_for_TLI(time_for_new_mass_function) <<std::endl;
            //std::cout << tilt_angle << std::endl;

            object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
            object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
            object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

            object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
            object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
            object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

            object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
            object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
            object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
            
            
            object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
            object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
            object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

            object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
            object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
            object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

            object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
            object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
            object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

            //koristim novu varijablu time_of_last_thrust jer u orbiti raketa ne trosi gorivo ali vrijeme tece pa ispada da nece imati goriva za translunarnu injekciju
            //zbog nacina na koji racunam thrust faktor
            //mora bit minus za y komponentu jer je rotacija u drugom smjeru
            rocket._a_x.push_back(m_rocket_x_acceleration_TLI(object_1, object_2, rocket, time_for_new_mass_function, tilt_angle)); 
            rocket._a_y.push_back(m_rocket_y_acceleration_TLI(object_1, object_2, rocket, time_for_new_mass_function, -tilt_angle));
            rocket._a_z.push_back(m_rocket_z_acceleration_TLI(object_1, object_2, rocket, time_for_new_mass_function, tilt_angle));

            rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
            rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
            rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

            rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
            rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
            rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

            tilt_angle = angle3(rocket._a_x.at(0), rocket._v_x.back(), rocket._a_y.at(0), rocket._v_y.back(), rocket._a_z.at(0), rocket._v_z.back());
            velocity = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(),2) + pow(rocket._v_z.back(),2));
            _time.push_back(_time.back() + m_dt);
            time_for_new_mass_function += m_dt;
            //break;
        }
        else
        {
            std::cout << "TLI burn ends: " << _time.back() << std::endl;
            std::cout << "Remaining mass: " << rocket._mass_function_for_TLI(time_for_new_mass_function) << std::endl;
            m_dt = 10;
            break;
        }
    }
}

void System::lunar_trajectory(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    /*transformacija vektora položaja i brzine u Mjesečev referentni sustav*/
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double angle = angle2(rx, vx, ry, vy, rz, vz);

    while(_time.back() < m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        rocket._a_x.push_back(m_rocket_x_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_y.push_back(m_rocket_y_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_z.push_back(m_rocket_z_acceleration_engine_off(object_1, object_2, rocket, _time.back()));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        _time.push_back(_time.back() + m_dt);

        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();
        angle = angle2(rx, rocket._v_x.back(), ry, rocket._v_y.back(), rz, rocket._v_z.back());
        //std::cout << distance_from_moon_center << std::endl;
        if(distance_from_moon_center <= 500e3)
        {
            m_dt = 1;
            std::cout << rx << ", " << ry << ", " << rz << std::endl;
            std::cout << vx << ", " << vy << ", " << vz << std::endl;
            std::cout << angle2(rx, vx, ry, vy, rz, vz) << std::endl;
            break;
        }
    }
    //std::cout << rx << ", " << ry << ", " << rz << std::endl;
    std::cout << distance_from_moon_center << std::endl;
    std::cout << "End time: " << _time.back() << std::endl;

}

void System::lunar_capture(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    
    //std::cout << angle2(rocket._a_x.at(0), rocket._v_x.back(), rocket._a_y.at(0), rocket._v_y.back(), rocket._a_z.at(0), rocket._v_z.back()) << std::endl;
    double time_for_lunar_capture = 0; 
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double tilt_angle = angle3(rocket._a_x.at(0), vx, rocket._a_y.at(0), vy, rocket._a_z.at(0), vz);
    double angle = angle2(rx, vx, ry, vy, rz, vz);
    double counter = 1;
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    double velocity;

    while(_time.back() < m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        
        
            //std::cout << angle << ", " << rocket._mass_function_for_lunar_capture(time_for_lunar_capture) <<std::endl;
        rocket._a_x.push_back(m_rocket_x_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -M_PI + tilt_angle + object_1.degree_to_radians(35)));
        rocket._a_y.push_back(m_rocket_y_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -M_PI + tilt_angle + object_1.degree_to_radians(35)));
        rocket._a_z.push_back(m_rocket_z_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -M_PI + tilt_angle + object_1.degree_to_radians(35)));
        
        // else 
        // {
        //     break;
        //     //std::cout << angle << ", " << rocket._mass_function_for_lunar_capture(time_for_lunar_capture) <<std::endl;
        //     rocket._a_x.push_back(m_rocket_x_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -tilt_angle));
        //     rocket._a_y.push_back(m_rocket_y_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -tilt_angle));
        //     rocket._a_z.push_back(m_rocket_z_acceleration_lunar_capture(object_1, object_2, rocket, time_for_lunar_capture, -tilt_angle));
        // }
        
        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        _time.push_back(_time.back() + m_dt);
        time_for_lunar_capture += m_dt;
        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();
        counter+=0.1;
        velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));
        tilt_angle = angle3(rocket._a_x.at(0), vx, rocket._a_y.at(0), vy, rocket._a_z.at(0), vz);
        angle = angle2(rx, vx, ry, vy, rz, vz);
        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        //std::cout << angle << ", " << rocket._mass_function_for_lunar_capture(time_for_lunar_capture) <<std::endl;
        //std::cout << velocity << ", " << rocket._mass_function_for_lunar_capture(time_for_lunar_capture) << ", " << distance_from_moon_center << ", " << angle << ", " << m_orbital_velocity(distance_from_moon_center) << std::endl;
        //std::cout << rx << ", " << ry << ", " << rz << std::endl;
        //std::cout << vx << ", " << vy << ", " << vz << std::endl;
        if(rocket._mass_function_for_lunar_capture(time_for_lunar_capture) < 85e3 || distance_from_moon_center < 155e3 || angle <= 90.5)
        {
            double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
            std::cout << distance_from_moon_center << std::endl;
            std::cout << velocity << std::endl;
            std::cout << angle << std::endl;
            std::cout << _time.back() <<std::endl;
            m_dt = 1;
            break;
        }
        //break;
    }
}

void System::lunar_orbit(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    int counter = 0;
    while(_time.back() < m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        rocket._a_x.push_back(m_rocket_x_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_y.push_back(m_rocket_y_acceleration_engine_off(object_1, object_2, rocket, _time.back()));
        rocket._a_z.push_back(m_rocket_z_acceleration_engine_off(object_1, object_2, rocket, _time.back()));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        _time.push_back(_time.back() + m_dt);
        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        //std::cout << distance_from_moon_center << std::endl;
        //counter++;
        if(_time.back() > /*236771 je vrijeme u kojem bi započeo silazak*/ 400000)
        {
            break;
        }
    }
}
/*ne radi još*/
void System::lunar_landing(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    double tilt_angle = angle3(rocket._a_x.at(0), vx, rocket._a_y.at(0), vy, rocket._a_z.at(0), vz);
    double angle = angle2(rx, vx, ry, vy, rz, vz);
    double velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));
    double time_for_lunar_landing = 0;

    while(_time.back() < m_max_time)
    {
        object_1.a_x_cm.push_back(m_x_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_y_cm.push_back(m_y_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));
        object_1.a_z_cm.push_back(m_z_acceleration(object_2._mass, object_1, object_2, rocket, _time.back()));

        object_1.v_x_cm.push_back(object_1.v_x_cm.back() + object_1.a_x_cm.back()*m_dt);
        object_1.v_y_cm.push_back(object_1.v_y_cm.back() + object_1.a_y_cm.back()*m_dt);
        object_1.v_z_cm.push_back(object_1.v_z_cm.back() + object_1.a_z_cm.back()*m_dt);

        object_1.x_cm.push_back(object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
        object_1.y_cm.push_back(object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
        object_1.z_cm.push_back(object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);       
        
        
        object_2.a_x_cm.push_back(m_x_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_y_cm.push_back(m_y_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));
        object_2.a_z_cm.push_back(m_z_acceleration(object_1._mass, object_2, object_1, rocket, _time.back()));

        object_2.v_x_cm.push_back(object_2.v_x_cm.back() + object_2.a_x_cm.back()*m_dt);
        object_2.v_y_cm.push_back(object_2.v_y_cm.back() + object_2.a_y_cm.back()*m_dt);
        object_2.v_z_cm.push_back(object_2.v_z_cm.back() + object_2.a_z_cm.back()*m_dt);

        object_2.x_cm.push_back(object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
        object_2.y_cm.push_back(object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
        object_2.z_cm.push_back(object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);

        rocket._a_x.push_back(m_rocket_x_acceleration_lunar_landing(object_1, object_2, rocket, time_for_lunar_landing, M_PI - tilt_angle)); 
        rocket._a_y.push_back(m_rocket_y_acceleration_lunar_landing(object_1, object_2, rocket, time_for_lunar_landing, M_PI - tilt_angle));
        rocket._a_z.push_back(m_rocket_z_acceleration_lunar_landing(object_1, object_2, rocket, time_for_lunar_landing, M_PI - tilt_angle));

        rocket._v_x.push_back(rocket._v_x.back() + rocket._a_x.back()*m_dt);
        rocket._v_y.push_back(rocket._v_y.back() + rocket._a_y.back()*m_dt);
        rocket._v_z.push_back(rocket._v_z.back() + rocket._a_z.back()*m_dt);

        rocket._x.push_back(rocket._x.back() + rocket._v_x.back()*m_dt);
        rocket._y.push_back(rocket._y.back() + rocket._v_y.back()*m_dt);
        rocket._z.push_back(rocket._z.back() + rocket._v_z.back()*m_dt);

        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();

        tilt_angle = angle3(rocket._a_x.at(0), vx, rocket._a_y.at(0), vy, rocket._a_z.at(0), vz);
        angle = angle2(rx, vx, ry, vy, rz, vz);
        velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));

        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        std::cout << distance_from_moon_center << ", " << rocket._mass_function_for_lunar_landing(time_for_lunar_landing) << ", " << angle << ", " << velocity << std::endl;
        _time.push_back(_time.back()+m_dt);
        time_for_lunar_landing += m_dt;

        if(distance_from_moon_center <= 0)
        {
            std::cout << angle << std::endl;
            break;
        }

    }
}
/*slijede funkcije koje zapisuju vrijednosti pojedinih vektora u txt s namjerom plotanja*/
void System::WriteToFile(std::string filename, Planet object)
{
    std::ofstream file;
    
    file.open(filename);

    for(int i = 0; i < object.x_cm.size(); i++)
    {
        file << object.x_cm.at(i) << std::setw(15) << object.y_cm.at(i) << std::setw(15) << object.z_cm.at(i) << std::endl;
    }

    file.close();
}

void System::WriteToFile2(std::string filename, Rocket rocket)
{
    std::ofstream file;
    
    file.open(filename);

    for(int i = 0; i < rocket._x.size(); i++)
    {
        file << std::fixed << std::setprecision(7) <<rocket._x.at(i) << std::setw(20) << std::fixed << std::setprecision(7) << rocket._y.at(i) << std::setw(20) << std::fixed << std::setprecision(7) << rocket._z.at(i) << std::endl;
    }

    file.close();
}

void System::WriteToFile3(std::string filename, Rocket rocket)
{
    std::ofstream file;
    
    file.open(filename);

    for(int i = 0; i < rocket._v_x.size(); i++)
    {
        file << rocket._v_x.at(i) << std::setw(15) << rocket._v_y.at(i) << std::setw(15) << rocket._v_z.at(i) << std::endl;
    }

    file.close();
}

void System::WriteToFile4(std::string filename, Rocket rocket)
{
    std::ofstream file;
    
    file.open(filename);

    for(int i = 0; i < rocket._a_x.size(); i++)
    {
        file << std::fixed << std::setprecision(7) <<rocket._a_x.at(i) << std::setw(15) << std::fixed << std::setprecision(7) << rocket._a_y.at(i) << std::setw(15) << std::fixed << std::setprecision(7) << rocket._a_z.at(i) << std::endl;
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