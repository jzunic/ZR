#include <vector>
#include <cmath>
#include "systemtest.h"
#include "planet.h"
#include <string>
#include <fstream>
#include <iostream>
#include "rocket.h"
#include <iomanip>
#include <algorithm>

Systemtest::Systemtest(double max_time, std::vector<double> time)
{
    m_max_time = max_time;
    _time = time;
    _time.push_back(0);
}
/*gustoća zraka*/
double ro(double altitude)
{
    return 1225*exp(-(altitude - 6370e3)/8500);
}

/*funkcija koja računa numeričke derivacije*/
double Systemtest::m_numerical_derivative(std::function<double(double)> function, double time)
{
    double h = 1e-6;
    double derviative = (function(time+h) - function(time-h))/(2*h);
    return derviative;
}

/*Orbitalne brzine*/
double Systemtest::m_orbital_velocity(double distance, double mass, double radius)
{
    double orbital_velocity = sqrt(m_grav_const*mass/(radius + distance));
    return orbital_velocity;
}

/*signum funkcija*/
double sgn(double x)
{
    if(x > 0)
        return 1;
    else if (x == 0)
        return 0;
    else
        return -1;
}

/*funkcija koja računa x komponentnu akceleracije planeta*/
double Systemtest::m_x_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time, std::function<double(double)> mass_function)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.x_cm.back() - object_2.x_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    double grav_influence_rocket_on_planet = -m_grav_const*mass_function(current_time)*(object_1.x_cm.back() - rocket._x.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/*funkcija koja računa y komponentnu akceleracije planeta*/
double Systemtest::m_y_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time, std::function<double(double)> mass_function)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.y_cm.back() - object_2.y_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    double grav_influence_rocket_on_planet = -m_grav_const*mass_function(current_time)*(object_1.y_cm.back() - rocket._y.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/*funkcija koja računa z komponentnu akceleracije planeta*/
double Systemtest::m_z_acceleration(double mass, Planet& object_1, Planet& object_2, Rocket& rocket, double current_time, std::function<double(double)> mass_function)
{
    double grav_influence_planet_on_planet = -m_grav_const*mass*(object_1.z_cm.back() - object_2.z_cm.back())/pow(sqrt(pow(object_1.x_cm.back() - object_2.x_cm.back(), 2) + pow(object_1.y_cm.back() - object_2.y_cm.back(), 2) + pow(object_1.z_cm.back() - object_2.z_cm.back(), 2)), 3);
    double grav_influence_rocket_on_planet = -m_grav_const*mass_function(current_time)*(object_1.z_cm.back() - rocket._z.back())/pow(sqrt(pow(object_1.x_cm.back() - rocket._x.back(), 2) + pow(object_1.y_cm.back() - rocket._y.back(), 2) + pow(object_1.z_cm.back() - rocket._z.back(), 2)), 3);
    return(grav_influence_planet_on_planet + grav_influence_rocket_on_planet);
}

/**
 * @brief Funkcija koja računa x komponentu akceleracije rakete
 * Početna točka na Zemlji određuje kutove theta i fi u sfernim koordinata,
 * definirani su u system.h
 * Kutovi se koriste za izračunati x komponentnu potrebnu za radijalno lansiranje
 * Analogno za sljedeće dvije funkcije
 * 
 * @param planet1 Mjesec
 * @param planet2 Zemlja
 * @param rocket raketa
 * @param current_time trenutno vrijeme
 * @param tilt_angle kut za koji želimo rotirati akceleraciju
 * @param stage radijalno izgaranje, izgaranje gdje rotiramo akceleraciju, ugaseni motori
 * @param mass_function funkcija m(t) koju koristimo
 * @return double 
 */
double Systemtest::m_rocket_x_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time, double tilt_angle, std::string stage, std::function<double(double)> mass_function)
{
    double thrust = -3500*m_numerical_derivative(mass_function, current_time)/mass_function(current_time);
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._x.back() - planet1.x_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._x.back() - planet2.x_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    double distance_from_earth = sqrt(pow(rocket._x.back() - planet2.x_cm.back(),2) + pow(rocket._y.back() - planet2.y_cm.back(),2) + pow(rocket._z.back() - planet2.z_cm.back(),2));
    if (distance_from_earth - 6371e3 < 100e3) // zemljin radijus + gornja granica atmosfere
    {
        drag_influence = -0.5*ro(distance_from_earth)*1*rocket._v_x.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2));
    }
    if (mass_function(current_time) > 20e3 || stage == "engine-off")
    {
        if(stage == "pre-tilt")
            thrust_influence = thrust_x;
        else if(stage == "post-tilt")
            thrust_influence = thrust_x*cos(tilt_angle) + (vy*thrust_z - vz*thrust_y)*sin(tilt_angle) + vx*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
        else if(stage == "engine-off")
            thrust_influence = 0; 
    }
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double Systemtest::m_rocket_y_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time, double tilt_angle, std::string stage, std::function<double(double)> mass_function)
{
    double thrust = -3500*m_numerical_derivative(mass_function, current_time)/mass_function(current_time);
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._y.back() - planet1.y_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._y.back() - planet2.y_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    double distance_from_earth = sqrt(pow(rocket._x.back() - planet2.x_cm.back(),2) + pow(rocket._y.back() - planet2.y_cm.back(),2) + pow(rocket._z.back() - planet2.z_cm.back(),2));
    if (distance_from_earth - 6371e3 < 100e3)
    {
        drag_influence = -0.5*ro(distance_from_earth)*1*rocket._v_y.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2)); //0.47 je koef trenja za kuglu, lako mijenjat kasnije
    }
    if (mass_function(current_time) > 20e3 || stage == "engine-off")
    {
        if(stage == "pre-tilt")
            thrust_influence = thrust_y;
        else if(stage == "post-tilt")
            thrust_influence = thrust_y*cos(tilt_angle) + (vz*thrust_x - vx*thrust_z)*sin(tilt_angle) + vy*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
        else if(stage == "engine-off")
            thrust_influence = 0; 
    }
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

double Systemtest::m_rocket_z_acceleration(Planet& planet1, Planet& planet2, Rocket& rocket, double current_time, double tilt_angle, std::string stage, std::function<double(double)> mass_function)
{
    double thrust = -3500*m_numerical_derivative(mass_function, current_time)/mass_function(current_time);
    double thrust_x = thrust*cos(phi)*sin(theta);
    double thrust_y = thrust*sin(phi)*sin(theta);
    double thrust_z = thrust*cos(theta);
    double drag_influence = 0;
    double thrust_influence = 0;
    double grav_influence_planet1_on_rocket = -m_grav_const*planet1._mass*(rocket._z.back() - planet1.z_cm.back())/pow(sqrt(pow(planet1.x_cm.back() - rocket._x.back(), 2) + pow(planet1.y_cm.back() - rocket._y.back(), 2) + pow(planet1.z_cm.back() - rocket._z.back(), 2)), 3);
    double grav_influence_planet2_on_rocket = -m_grav_const*planet2._mass*(rocket._z.back() - planet2.z_cm.back())/pow(sqrt(pow(planet2.x_cm.back() - rocket._x.back(), 2) + pow(planet2.y_cm.back() - rocket._y.back(), 2) + pow(planet2.z_cm.back() - rocket._z.back(), 2)), 3);
    double distance_from_earth = sqrt(pow(rocket._x.back() - planet2.x_cm.back(),2) + pow(rocket._y.back() - planet2.y_cm.back(),2) + pow(rocket._z.back() - planet2.z_cm.back(),2));
    if (distance_from_earth - 6371e3 < 100e3)
    {
        drag_influence = -0.5*ro(distance_from_earth)*1*rocket._v_z.back()*(0.47/(mass_function(current_time)))*sqrt(pow(rocket._v_x.back(), 2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(), 2)); //0.47 je koef trenja za kuglu, lako mijenjat kasnije
    }
    if (mass_function(current_time) > 20e3 || stage == "engine-off")
    {
        if(stage == "pre-tilt")
        {
            thrust_influence = thrust_z;
        }
        else if(stage == "post-tilt")
        {
            thrust_influence = thrust_z*cos(tilt_angle) + (vx*thrust_y-vy*thrust_x)*sin(tilt_angle) + vz*(vx*thrust_x+vy*thrust_y+vz*thrust_z)*(1-cos(tilt_angle));
        }
        else if(stage == "engine-off")
        {
            thrust_influence = 0; 
        }
    }
    return (drag_influence + thrust_influence + grav_influence_planet1_on_rocket + grav_influence_planet2_on_rocket);
}

/*Računa iznos skalarnog produkta*/
double Systemtest::dot(std::vector<double> v1, std::vector<double> v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

/*računa kut između 2 vektora i vraća ga u stupnjevima*/
double Systemtest::angle2(double x1, double x2, double y1, double y2, double z1, double z2)
{
    /* skalarni produkt s nul vektorom daje 0, ali kut nije definiran pa cu ja rec da je 0*/
    double scalar_product = x1*x2 + y1*y2 + z1*z2;
    double scalar_length = sqrt(x1*x1 + y1*y1 + z1*z1)*sqrt(x2*x2 + y2*y2 + z2*z2);
    double angle_in_degrees = 0;
    if ((x1 == 0 && y1 == 0 && z1 == 0) || (x2 == 0 && y2 == 0 && z2 == 0))
    {
        angle_in_degrees = 0;
    }
    else
    {
        double argument = scalar_product/scalar_length;
        if (argument > 1.0) argument = 1.0;
        if (argument < -1.0) argument = -1.0;
        double angle_in_radians = acos(argument);
        angle_in_degrees = angle_in_radians*180.0/M_PI;
    }
    return (angle_in_degrees);
}

/*računa kut između 2 vektora i vraća ga u radijanima, postoji za izbjeći previše preračunjavanja iz
stupnjeva u radijane i obratno*/
double Systemtest::angle3(double x1, double x2, double y1, double y2, double z1, double z2)
{
    double scalar_product = x1*x2 + y1*y2 + z1*z2;
    double scalar_length = sqrt(x1*x1 + y1*y1 + z1*z1)*sqrt(x2*x2 + y2*y2 + z2*z2);
    double angle_in_radians = 0;
    if ((x1 == 0 && y1 == 0 && z1 == 0) || (x2 == 0 && y2 == 0 && z2 == 0))
    {
        angle_in_radians = 0;
    }
    else
    {
        double argument = scalar_product/scalar_length;
        if (argument > 1.0) argument = 1.0;
        if (argument < -1.0) argument = -1.0;
        angle_in_radians = acos(argument);
    }
    return (angle_in_radians);
}

/**
 *Koristi Eulerovu metodu za ažurirati brzine i položaje
 */
void Systemtest::move_planets(Planet& object_1, Planet& object_2, Rocket& rocket, double current_time, std::function<double(double)> mass_function)
{
    a_x_cm_1 = (m_x_acceleration(object_2._mass, object_1, object_2, rocket, current_time, mass_function));
    a_y_cm_1 = (m_y_acceleration(object_2._mass, object_1, object_2, rocket, current_time, mass_function));
    a_z_cm_1 = (m_z_acceleration(object_2._mass, object_1, object_2, rocket, current_time, mass_function));

    object_1.v_x_cm.back() = (object_1.v_x_cm.back() + a_x_cm_1*m_dt);
    object_1.v_y_cm.back() = (object_1.v_y_cm.back() + a_y_cm_1*m_dt);
    object_1.v_z_cm.back() = (object_1.v_z_cm.back() + a_z_cm_1*m_dt);

    object_1.x_cm.back() = (object_1.x_cm.back() + object_1.v_x_cm.back()*m_dt);
    object_1.y_cm.back() = (object_1.y_cm.back() + object_1.v_y_cm.back()*m_dt); 
    object_1.z_cm.back() = (object_1.z_cm.back() + object_1.v_z_cm.back()*m_dt);     
    
    a_x_cm_2 = (m_x_acceleration(object_1._mass, object_2, object_1, rocket, current_time, mass_function));
    a_y_cm_2 = (m_y_acceleration(object_1._mass, object_2, object_1, rocket, current_time, mass_function));
    a_z_cm_2 = (m_z_acceleration(object_1._mass, object_2, object_1, rocket, current_time, mass_function));

    object_2.v_x_cm.back() = (object_2.v_x_cm.back() + a_x_cm_2*m_dt);
    object_2.v_y_cm.back() = (object_2.v_y_cm.back() + a_y_cm_2*m_dt);
    object_2.v_z_cm.back() = (object_2.v_z_cm.back() + a_z_cm_2*m_dt);

    object_2.x_cm.back() = (object_2.x_cm.back() + object_2.v_x_cm.back()*m_dt);
    object_2.y_cm.back() = (object_2.y_cm.back() + object_2.v_y_cm.back()*m_dt);
    object_2.z_cm.back() = (object_2.z_cm.back() + object_2.v_z_cm.back()*m_dt);
}

void Systemtest::move_rocket(Rocket& rocket, double accelerationx, double accelerationy, double accelerationz)
{
    rocket._v_x.back() = (rocket._v_x.back() + accelerationx*m_dt);
    rocket._v_y.back() = (rocket._v_y.back() + accelerationy*m_dt);
    rocket._v_z.back() = (rocket._v_z.back() + accelerationz*m_dt);

    rocket._x.back() = (rocket._x.back() + rocket._v_x.back()*m_dt);
    rocket._y.back() = (rocket._y.back() + rocket._v_y.back()*m_dt);
    rocket._z.back() = (rocket._z.back() + rocket._v_z.back()*m_dt);
}

/*Računa vektorski produkt 2 vektora*/
std::vector<double> Systemtest::m_cross_product(std::vector<double> v1, std::vector<double> v2)
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
 */
void Systemtest::lift_off(Planet& object_1, Planet& object_2, Rocket& rocket, int altitude)
{       

    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    rck.open("rocket_cm.txt", std::ios::out | std::ios::app);
    while (_time.back() < m_max_time)
    {    
        move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function);

        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), 0, "pre-tilt", rocket._mass_function));
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), 0, "pre-tilt", rocket._mass_function));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), 0, "pre-tilt", rocket._mass_function));
        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);
        
        _time.push_back(_time.back() + m_dt);
        a_x_0 = a_x_cm;
        a_y_0 = a_y_cm;
        a_z_0 = a_z_cm;

        double rx = rocket._x.back() - object_2.x_cm.back();
        double ry = rocket._y.back() - object_2.y_cm.back();
        double rz = rocket._z.back() - object_2.z_cm.back();
        double vx = rocket._v_x.back() - object_2.v_x_cm.back();
        double vy = rocket._v_y.back() - object_2.v_y_cm.back();
        double vz = rocket._v_z.back() - object_2.v_z_cm.back();
        double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
        double horizontal_velocity = sqrt(std::max(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2), 0.));
        //rck << _time.back() << ", " << radial_velocity << ", " << horizontal_velocity << std::endl;
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        //rck << rocket._x.back() << ", " << rocket._y.back() << ", " << rocket._z.back() << std::endl;
        double distance_from_earth_center = sqrt(pow(rocket._x.back() - object_2.x_cm.back(),2) + pow(rocket._y.back() - object_2.y_cm.back(),2) + pow(rocket._z.back() - object_2.z_cm.back(),2));
        double velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
        if ((distance_from_earth_center - 6371e3 >= altitude) || distance_from_earth_center - 6370e3 <= 0)
        {
            // std::cout << angle(rocket) << std::endl;
            //std::cout << "Lift off ends: " <<_time.back() << std::endl;
            // std::cout << "altitude: " << altitude << std::endl;
            // std::cout << distance_from_earth_center - 6371e3 << std::endl;
            //std::cout << velocity << std::endl;
            // std::cout << rocket._mass_function(12.1) << std::endl;
            tempfile.close();
            rck.close();
            break;
        }
    }
}

/**
 * Funkcija koja vrši početni korak gravity turna
 * Vektor pogona rakete rotira se oko proizvoljne osi za proizvoljni kut
 * Sada vektor pogona i vektor gravitacije nisu kolinerani te gravitacija nastoji rotirati 
 * putanju rakete
 * Funkcija staje kada se postigne željeni kut ili neki kut njemu blizak, ovisno o koraku
 * koji se koristi za promjenu kuta
 */
void Systemtest::tilting(Planet& object_1, Planet& object_2, Rocket& rocket, double trial_angle)
{

    double acceleration_before_turn_x = a_x_cm;
    double acceleration_before_turn_y = a_y_cm;
    double acceleration_before_turn_z = a_z_cm;

    /*postavljam na 0 zbog uvjeta u angle2 i angle3 funkcijama, s ovim će vratiti u prvom pozivanju 0*/
    double acceleration_during_turn_x = 0, acceleration_during_turn_y = 0, acceleration_during_turn_z = 0;

    double counter = 1;
    double tilt = 0;
    double desired_tilt_rate = 0.5 * M_PI/180;

    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    rck.open("rocket_cm.txt", std::ios::out | std::ios::app);

    while(_time.back() <= m_max_time)
    {

        tilt_angle = angle2(acceleration_before_turn_x, acceleration_during_turn_x, acceleration_before_turn_y, acceleration_during_turn_y, acceleration_before_turn_z, acceleration_during_turn_z);
        
        if(tilt_angle <= trial_angle || tilt_angle == 0)
        {
            move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function);

            a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), tilt, "post-tilt", rocket._mass_function));
            a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), tilt, "post-tilt", rocket._mass_function));
            a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), tilt, "post-tilt", rocket._mass_function));
            move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

            acceleration_during_turn_x = a_x_cm;
            acceleration_during_turn_y = a_y_cm;
            acceleration_during_turn_z = a_z_cm;

            _time.push_back(_time.back() + m_dt);

            tilt += desired_tilt_rate * m_dt;

            double rx = rocket._x.back() - object_2.x_cm.back();
            double ry = rocket._y.back() - object_2.y_cm.back();
            double rz = rocket._z.back() - object_2.z_cm.back();
            double vx = rocket._v_x.back() - object_2.v_x_cm.back();
            double vy = rocket._v_y.back() - object_2.v_y_cm.back();
            double vz = rocket._v_z.back() - object_2.v_z_cm.back();
            double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
            double horizontal_velocity = sqrt(std::max(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2), 0.));
            
            //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
            //rck << rocket._x.back() << ", " << rocket._y.back() << ", " << rocket._z.back() << std::endl;
            //rck << _time.back() << ", " << radial_velocity << ", " << horizontal_velocity << std::endl;
        }
        else
        {
            //std::cout << "fuel: " <<rocket._mass_function(_time.back()) << std::endl;
            //std::cout << distance_from_earth_center - 6371e3 << std::endl;
            // std::cout << "Tilting process ends: " <<_time.back() << std::endl;
            //std::cout << "tilt: "<< tilt_angle << std::endl;
            tempfile.close();
            rck.close();
            break;
        }
    }
}

/**
 * Funkcija koja bi trebala dovršiti uspon do orbite oko Zemlje
 * Svakim korakom vektor pogona rakete se namješta da bude u istom smjeru kao akceleracija 
 * iz prethodnog koraka
 */
void Systemtest::gravity_turn(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double counter = 0;

    tilt_angle = object_1.degree_to_radians(tilt_angle);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    rck.open("rocket_cm.txt", std::ios::out | std::ios::app);
    double rx = rocket._x.back() - object_2.x_cm.back();
    double ry = rocket._y.back() - object_2.y_cm.back();
    double rz = rocket._z.back() - object_2.z_cm.back();
    double vx = rocket._v_x.back() - object_2.v_x_cm.back();
    double vy = rocket._v_y.back() - object_2.v_y_cm.back();
    double vz = rocket._v_z.back() - object_2.v_z_cm.back();
    angle_final = angle2(rx, vx, ry, vy, rz, vz);

    double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
    
    while(_time.back() < m_max_time)
    {
        
        move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function);
        
        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), tilt_angle, "post-tilt", rocket._mass_function)); 
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), tilt_angle, "post-tilt", rocket._mass_function));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), tilt_angle, "post-tilt", rocket._mass_function));
        
        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

        double rx = rocket._x.back() - object_2.x_cm.back();
        double ry = rocket._y.back() - object_2.y_cm.back();
        double rz = rocket._z.back() - object_2.z_cm.back();
        double vx = rocket._v_x.back() - object_2.v_x_cm.back();
        double vy = rocket._v_y.back() - object_2.v_y_cm.back();
        double vz = rocket._v_z.back() - object_2.v_z_cm.back();

        _time.push_back(_time.back() + m_dt);
        tilt_angle = angle3(a_x_0, vx, a_y_0, vy, a_z_0, vz);
        angle_final = angle2(rx, vx, ry, vy, rz, vz);

        
        double distance_from_earth_center = sqrt(pow(rocket._x.back() - object_2.x_cm.back(),2) + pow(rocket._y.back() - object_2.y_cm.back(),2) + pow(rocket._z.back() - object_2.z_cm.back(),2));
        double velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        //rck << rocket._x.back() << ", " << rocket._y.back() << ", " << rocket._z.back() << std::endl;

        double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
        double horizontal_velocity = sqrt(std::max(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2), 0.));
        //rck << _time.back() << ", " << radial_velocity << ", " << horizontal_velocity << std::endl; 
        
        if(rocket._mass_function(_time.back()) <= 140e3 || ((angle_final >= 90 && angle_final < 90.1) && (velocity >=m_orbital_velocity(distance_from_earth_center, object_2._mass, 0) && velocity <=m_orbital_velocity(distance_from_earth_center, object_2._mass, 0) + 1) /*&& (distance_from_earth_center-6371e3 > 150e3*/ /*&& distance_from_earth_center-6371e3 < 150e3*/))
        {
            // std::cout << "fuel: " << rocket._mass_function(_time.back()) << std::endl;
            // std::cout << "angle: "<< angle_final << std::endl;
            // std::cout << "distance: " << (distance_from_earth_center - 6370e3)/1000 << std::endl;
            // std::cout << "gravity turn ends: " <<_time.back() << std::endl;
            // std::cout << "velocity: " << velocity << std::endl;
            //tempfile << object_2.x_cm.back() << std::setw(15) << object_2.y_cm.back() << std::setw(15) << object_2.z_cm.back() << std::endl;
            tempfile.close();
            rck.close();
            break;
        }
    }  
}
/**
 * Računa položaje i brzine u orbiti oko Zemlje
 * Funkcija završava u trenutku kad je kut između Zemlje i Mjeseca 140 stupnjeva
 * Daje dobre rezultate
 */
void Systemtest::orbit(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    std::ofstream file;
    file.open("udaljenosti.txt");
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    rck.open("rocket_cm.txt", std::ios::out | std::ios::app);

    double distance_from_earth_center = sqrt(pow(rocket._x.back() - object_2.x_cm.back(),2) + pow(rocket._y.back() - object_2.y_cm.back(),2) + pow(rocket._z.back() - object_2.z_cm.back(),2));
    double velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
    double angle = angle2(rocket._x.back(), rocket._v_x.back(), rocket._y.back(), rocket._v_y.back(), rocket._z.back(), rocket._v_z.back());
    double angle_moon_rocket = angle2(rocket._x.back(), object_1.x_cm.back(), rocket._y.back(), object_1.y_cm.back(), rocket._z.back(), object_1.z_cm.back());
    double time = _time.back();
    double rmass = rocket_mass;


    while(_time.back() <= m_max_time)
    {
        /*kako se masa ne mijenja dok motori ne rade, moram funkcijama akceleracije slati trenutnu masu*/
        std::function<double(double)> constant_mass = [rmass](double time) {return rmass;};

        move_planets(object_1, object_2, rocket, _time.back(), constant_mass);

        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));

        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

        _time.push_back(_time.back() + m_dt);
        distance_from_earth_center = sqrt(pow(rocket._x.back() - object_2.x_cm.back(),2) + pow(rocket._y.back() - object_2.y_cm.back(),2) + pow(rocket._z.back() - object_2.z_cm.back(),2));
        velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
        angle = angle2(rocket._x.back(), rocket._v_x.back(), rocket._y.back(), rocket._v_y.back(), rocket._z.back(), rocket._v_z.back());
        angle_moon_rocket = angle2(rocket._x.back(), object_1.x_cm.back(), rocket._y.back(), object_1.y_cm.back(), rocket._z.back(), object_1.z_cm.back());
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        //rck << rocket._x.back() << ", " << rocket._y.back() << ", " << rocket._z.back() << std::endl;
        double signed_angle = atan2(m_cross_product({rocket._x.back(), rocket._y.back(), rocket._z.back()}, {object_1.x_cm.back(), object_1.y_cm.back(), object_1.z_cm.back()})[2], dot({rocket._x.back(), rocket._y.back(), rocket._z.back()}, {object_1.x_cm.back(), object_1.y_cm.back(), object_1.z_cm.back()}));
        signed_angle = signed_angle*180/M_PI;
        if(distance_from_earth_center - 6371e3 <=0 || (signed_angle >= 140 && signed_angle <= 140.1))
        {
            //tempfile << object_2.x_cm.back() << std::setw(15) << object_2.y_cm.back() << std::setw(15) << object_2.z_cm.back() << std::endl;
            tempfile.close();
            file.close();
            rck.close();
            break;
        }
    }
}
/**
 * Vrši ubrzanje potrebno za poslati raketu na putanju prema Mjesecu
 * Izvršava se dok brzina ne postigne proslijeđenu vrijednost 
 */
void Systemtest::translunar_injection_burn(Planet& object_1, Planet& object_2, Rocket& rocket, double target_velocity)
{
    double time_for_new_mass_function = 0;
    double velocity = velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
    double tilt_angle = angle3(a_x_0, rocket._v_x.back(), a_y_0, rocket._v_y.back(), a_z_0, rocket._v_z.back());
    std::ofstream file("putanje.txt", std::ios::out | std::ios::app);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);

    double rx = rocket._x.back() - object_2.x_cm.back();
    double ry = rocket._y.back() - object_2.y_cm.back();
    double rz = rocket._z.back() - object_2.z_cm.back();
    double vx = rocket._v_x.back() - object_2.v_x_cm.back();
    double vy = rocket._v_y.back() - object_2.v_y_cm.back();
    double vz = rocket._v_z.back() - object_2.v_z_cm.back();


    double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
    double horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));


    while(_time.back() < m_max_time)
    {
        if(velocity <= target_velocity)
        {
            move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function_for_TLI);

            a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, time_for_new_mass_function, tilt_angle, "post-tilt", rocket._mass_function_for_TLI)); 
            a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, time_for_new_mass_function, sgn(rocket._v_y.back())*tilt_angle, "post-tilt", rocket._mass_function_for_TLI));
            a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, time_for_new_mass_function, tilt_angle, "post-tilt", rocket._mass_function_for_TLI));

            move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

            tilt_angle = angle3(a_x_0, rocket._v_x.back(), a_y_0, rocket._v_y.back(), a_z_0, rocket._v_z.back());
            velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));
            _time.push_back(_time.back() + m_dt);
            time_for_new_mass_function += m_dt;
            double rx = rocket._x.back() - object_2.x_cm.back();
            double ry = rocket._y.back() - object_2.y_cm.back();
            double rz = rocket._z.back() - object_2.z_cm.back();
            double vx = rocket._v_x.back() - object_2.v_x_cm.back();
            double vy = rocket._v_y.back() - object_2.v_y_cm.back();
            double vz = rocket._v_z.back() - object_2.v_z_cm.back();


            double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
            double horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));
            //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
            //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        }
        else
        {
            //std::cout << "Remaining mass: " << rocket._mass_function_for_TLI(time_for_new_mass_function) << std::endl;
            //std::cout << "VELOCITY: " << velocity << std::endl;
            file.close();
            tempfile.close();
            m_dt = 0.1;
            rocket_mass = rocket._mass_function_for_TLI(time_for_new_mass_function);
            break;
        }
    }
}
/**
 * @brief Računa putanju do Mjeseca
 * 
 * @param mode Parametar određuje želimo li pustiti putanju da se integrira iza periapsisa,
 * potrebno za odrediti je li putanja dobra ili želimo stati u periapsisu s izvršavanjem
 */
void Systemtest::lunar_trajectory(Planet& object_1, Planet& object_2, Rocket& rocket, std::string mode)
{
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();

    double rxe = rocket._x.back() - object_2.x_cm.back();
    double rye = rocket._y.back() - object_2.y_cm.back();
    double rze = rocket._z.back() - object_2.z_cm.back();
    double vxe = rocket._v_x.back() - object_2.v_x_cm.back();
    double vye = rocket._v_y.back() - object_2.v_y_cm.back();
    double vze = rocket._v_z.back() - object_2.v_z_cm.back();

    double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
    double angle_temp = angle2(rx, vx, ry, vy, rz, vz);
    angle_final = angle_temp;
    tilt_angle = angle3(a_x_0, rocket._v_x.back(), a_y_0, rocket._v_y.back(), a_z_0, rocket._v_z.back());
    double time_for_burn = 0;
    double velocity_rel;
    double rmass = rocket_mass;
    rad_velocity = radial_velocity;
    double radial_velocity_earth = (rxe*vxe + vye*rye + vze*rze)/sqrt(rxe*rxe + rye*rye + rze*rze);
    bool first = true;
    double horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));
    is_return = 0.;
    
     	

    std::ofstream file("putanje.txt", std::ios::out | std::ios::app);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    std::ofstream ofc("brzine.txt", std::ios::out | std::ios::app);
    std::ofstream bla("udalj.txt");
    double velocity = sqrt(pow(rocket._v_x.back() - object_2.v_x_cm.back(),2) + pow(rocket._v_y.back() - object_2.v_y_cm.back(),2) + pow(rocket._v_z.back() - object_2.v_z_cm.back(),2));

    while(_time.back() < m_max_time)
    {
        success = false;
        std::function<double(double)> constant_mass = [rmass](double time) {return rmass;};
        
        move_planets(object_1, object_2, rocket, _time.back(), constant_mass);

        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));

        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

        _time.push_back(_time.back() + m_dt);

        velocity_rel = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));
        velocity = sqrt(pow(rocket._v_x.back(),2) + pow(rocket._v_y.back(), 2) + pow(rocket._v_z.back(),2));
        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        double distance_from_earth = sqrt(pow(rocket._x.back() - object_2.x_cm.back(),2) + pow(rocket._y.back() - object_2.y_cm.back(),2) + pow(rocket._z.back() - object_2.z_cm.back(),2));
        //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
    

        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();

        rxe = rocket._x.back() - object_2.x_cm.back();
        rye = rocket._y.back() - object_2.y_cm.back();
        rze = rocket._z.back() - object_2.z_cm.back();
        vxe = rocket._v_x.back() - object_2.v_x_cm.back();
        vye = rocket._v_y.back() - object_2.v_y_cm.back();
        vze = rocket._v_z.back() - object_2.v_z_cm.back();

        angle_temp = angle2(rx, vx, ry, vy, rz, vz);
        angle_final = angle_temp;
        radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
        radial_velocity_earth = (rxe*vxe + vye*rye + vze*rze)/sqrt(rxe*rxe + rye*rye + rze*rze);
        horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));

        if(distance_from_moon_center < perilune)
        {
            perilune = distance_from_moon_center;
            //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
            
        }
        else if(mode == "capture") break;
        if(distance_from_moon_center < 10000e3)
        {
            //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;   
            //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        }
        if(_time.back() >= 3.6*24*3600)
        {
            if(radial_velocity_earth < 0)
                is_return = 1.0;
            break;
        }
    }
    file.close();
    tempfile.close();
}
/**
 * @brief Funkcija izvršava usporavanje potrebno za uhvatiti se u Mjesečevu orbitu
 * 
 */
void Systemtest::lunar_capture(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    
    double time_for_lunar_capture = 0; 
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double tilt_angle = angle3(a_x_0, vx, a_y_0, vy, a_z_0, vz);
    double angle = angle2(rx, vx, ry, vy, rz, vz);
    double counter = 1;
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    double velocity;
    double target_altitude = distance_from_moon_center;
    std::ofstream file("putanje.txt", std::ios::out | std::ios::app);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    m_dt = 0.001;

    while(_time.back() < m_max_time)
    {
        success = false;
        move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function_for_lunar_capture);

        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, time_for_lunar_capture, - M_PI + tilt_angle, "post-tilt", rocket._mass_function_for_lunar_capture));
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, time_for_lunar_capture, - M_PI + sgn(vy)*tilt_angle, "post-tilt", rocket._mass_function_for_lunar_capture));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, time_for_lunar_capture, - M_PI + tilt_angle, "post-tilt", rocket._mass_function_for_lunar_capture));

        velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));

        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

        _time.push_back(_time.back() + m_dt);
        time_for_lunar_capture += m_dt;
        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();
        counter+=0.1;
        
        tilt_angle = angle3(a_x_0, vx, a_y_0, vy, a_z_0, vz);
        angle = angle2(rx, vx, ry, vy, rz, vz);
        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        angle_final = angle;
        //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        // file << rx << std::setw(15) << ry << std::setw(15) << rz << std::endl;
        if((rocket._mass_function_for_lunar_capture(time_for_lunar_capture) > 50e3 && velocity <= m_orbital_velocity(distance_from_moon_center, object_1._mass, 1737e3)))
        {
            // double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
            // std::cout << "visina: " << distance_from_moon_center << std::endl;
            // std::cout << velocity << std::endl;
            // std::cout << angle << std::endl;
            // std::cout << _time.back() <<std::endl;
            success = true;
            file.close();
            tempfile.close();
            rocket_mass = rocket._mass_function_for_lunar_capture(time_for_lunar_capture);
            break;
        }
    }
}
void Systemtest::lunar_orbit(Planet& object_1, Planet& object_2, Rocket& rocket)
{
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    std::ofstream file("putanje.txt", std::ios::out | std::ios::app);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    std::ofstream file1("udaljenosti.txt");
    double angle;
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double rmass = rocket_mass;
    m_dt = 0.01;

    while(_time.back() < m_max_time)
    {
        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();

        std::function<double(double)> constant_mass = [rmass](double time) {return rmass;};

        move_planets(object_1, object_2, rocket, _time.back(), constant_mass);

        a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));
        a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, _time.back(), 0, "engine-off", rocket._mass_function));

        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);

        _time.push_back(_time.back() + m_dt);
        distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
        angle = angle2(rx, vx, ry, vy, rz, vz);
        //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl;
        //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        //file << rx << std::setw(15) << ry << std::setw(15) << rz << std::endl;
        
        if(_time.back() > 280e3 || distance_from_moon_center < 0)
        {
            //tempfile << object_1.x_cm.back() << std::setw(15) << object_1.y_cm.back() << std::setw(15) << object_1.z_cm.back() << std::endl; 
            file.close();
            tempfile.close();
            break;
        }
    }
}
/**
 * @brief Funkcija prvo vrši kratko izgaranje za spustiti najmanju udaljenost od Mjeseca
 * na 15.5 km pa onda ovisno o stage parametru ili nastavlja računati novu eliptičnu putanju
 * ili nastavlja sa slijetanjem. Varijabla first postoji jer raketa pri spustanju opet može postići
 * brzinu veću od target velocity, ali ne želimo da se opet pali DOI.
 * 
 * @param target_velocity Brzina koju želimo da raketa ima nakon DOI faze 
 * @param stage Određuje želimo li da raketa nastavi s eliptičnom orbitom (vrijednost test) ili da 
 * krene usporavati za slijetanje (bilo koja druga vrijednost)
 */
void Systemtest::lunar_landing(Planet& object_1, Planet& object_2, Rocket& rocket, double target_velocity, std::string stage)
{
    std::ofstream file("putanje.txt", std::ios::out | std::ios::app);
    tempfile.open("moon_cm.txt", std::ios::out | std::ios::app);
    double rx = rocket._x.back() - object_1.x_cm.back();
    double ry = rocket._y.back() - object_1.y_cm.back();
    double rz = rocket._z.back() - object_1.z_cm.back();
    double vx = rocket._v_x.back() - object_1.v_x_cm.back();
    double vy = rocket._v_y.back() - object_1.v_y_cm.back();
    double vz = rocket._v_z.back() - object_1.v_z_cm.back();
    double distance_from_moon_center = sqrt(pow(rocket._x.back() - object_1.x_cm.back(),2) + pow(rocket._y.back() - object_1.y_cm.back(),2) + pow(rocket._z.back() - object_1.z_cm.back(),2)) - 1737e3;
    double tilt_angle = angle3(a_x_0, vx, a_y_0, vy, a_z_0, vz);
    double angle = angle2(rx, vx, ry, vy, rz, vz);
    double velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));
    double time_for_lunar_landing = 0;
    double time_for_DOI = 0;
    double radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
    double horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));
    bool end = true;
    bool landing_burn = false;
    bool DOI_burn = false;
    bool first_time = true;
    m_dt = 0.001;

    while(_time.back() < m_max_time && end)
    {
        move_planets(object_1, object_2, rocket, _time.back(), rocket._mass_function_for_lunar_landing);

        if(velocity >= target_velocity && first_time)
        {
            a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, time_for_DOI, - M_PI + tilt_angle, "post-tilt", rocket._mass_function_for_DOI)); 
            a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, time_for_DOI, - M_PI + sgn(vy)*tilt_angle, "post-tilt", rocket._mass_function_for_DOI));
            a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, time_for_DOI, - M_PI + tilt_angle, "post-tilt", rocket._mass_function_for_DOI));
            DOI_burn = true;
        }
        else if(distance_from_moon_center <= 15.7e3 && stage != "test")
        {
            a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, time_for_lunar_landing, - M_PI + sgn(vx)*tilt_angle, "post-tilt", rocket._mass_function_for_lunar_landing)); 
            a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, time_for_lunar_landing, - M_PI + sgn(vy)*tilt_angle, "post-tilt", rocket._mass_function_for_lunar_landing));
            a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, time_for_lunar_landing, - M_PI + tilt_angle, "post-tilt", rocket._mass_function_for_lunar_landing));
            //std::cout << "x: " << m_remember_thrust_influence_x << " y: " << m_remember_thrust_influence_y << std::endl;
            landing_burn = true;
            if(abs(radial_velocity) < 1) break;
        }
        else
        {
            a_x_cm = (m_rocket_x_acceleration(object_1, object_2, rocket, time_for_lunar_landing, 0, "engine-off", rocket._mass_function_for_DOI)); 
            a_y_cm = (m_rocket_y_acceleration(object_1, object_2, rocket, time_for_lunar_landing, 0, "engine-off", rocket._mass_function_for_DOI));
            a_z_cm = (m_rocket_z_acceleration(object_1, object_2, rocket, time_for_lunar_landing, 0, "engine-off", rocket._mass_function_for_DOI));
            DOI_burn = false;
            first_time = false;
        }

        move_rocket(rocket, a_x_cm, a_y_cm, a_z_cm);
        
        if(DOI_burn)
            time_for_DOI += m_dt;

        if(landing_burn)
            time_for_lunar_landing += m_dt;

        rx = rocket._x.back() - object_1.x_cm.back();
        ry = rocket._y.back() - object_1.y_cm.back();
        rz = rocket._z.back() - object_1.z_cm.back();
        vx = rocket._v_x.back() - object_1.v_x_cm.back();
        vy = rocket._v_y.back() - object_1.v_y_cm.back();
        vz = rocket._v_z.back() - object_1.v_z_cm.back();

        tilt_angle = angle3(a_x_0, vx, a_y_0, vy, a_z_0, vz);
        angle = angle2(rx, vx, ry, vy, rz, vz);
        velocity = sqrt(pow(vx,2) + pow(vy,2) + pow(vz,2));
        radial_velocity = (rx*vx + vy*ry + vz*rz)/sqrt(rx*rx + ry*ry + rz*rz);
        double horizontal_velocity = sqrt(vx*vx + vy*vy + vz*vz - pow(radial_velocity, 2));

        distance_from_moon_center = sqrt(pow(rx,2) + pow(ry,2) + pow(rz,2)) - 1737e3;
        //std::cout << distance_from_moon_center << ", " << velocity << ", " << radial_velocity << ", " << rocket._mass_function_for_lunar_landing(time_for_lunar_landing) << ", " << horizontal_velocity << std::endl;
        _time.push_back(_time.back()+m_dt);
    
        //file << rocket._x.back() << std::setw(15) << rocket._y.back() << std::setw(15) << rocket._z.back() << std::endl;
        //tempfile << object_1.x_cm.back() << ", " << object_1.y_cm.back() << ", " << object_1.z_cm.back() << std::endl;
        file << rx << ", " << ry << ", " << rz << std::endl;
        if(distance_from_moon_center < perilune)
            perilune = distance_from_moon_center;

        if(stage == "test")
        {
            if(distance_from_moon_center < 15.5e3 && (radial_velocity >= 0 && radial_velocity <= 0.5))
            {
                // std::cout << "VELOCITY: " << velocity << std::endl;
                // std::cout << angle << std::endl;
                // std::cout << time_for_DOI << std::endl;
                // std::cout << distance_from_moon_center << std::endl;
                // std::cout << rocket._mass_function_for_DOI(time_for_DOI) << std::endl;
                end = false;
            }else if(_time.back() > 300e3) end = false;
        }

        if(distance_from_moon_center < 0) 
        {
            break;
        }

    }
}