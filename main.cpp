#include <iostream>
#include <string>
#include "planet.h"
#include "system.h"
#include <fstream>


int main()
{
    Planet earth(6371e3, 5.972e24, 0, 0, 0, 0, 0);
    Planet moon(1737e3, 7.35e22, 384400e3, 384400e3, 0, 0, 1022);

    std::vector<std::vector<double>> earth_3D = earth.Generate_planet_3D(earth);
    std::vector<std::vector<double>> moon_3D = moon.Generate_planet_3D(moon);

    std::vector<double> time;
    System system(28*3600*24, time);
    system.move(moon, earth);

    system.WriteToFile("earth_cm.txt", earth);
    system.WriteToFile("moon_cm.txt", moon);

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