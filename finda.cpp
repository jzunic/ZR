#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file;
    file.open("udaljenosti.txt");
    std::string value;
    double min, max, a;

    getline(file, value);
    min = std::stod(value);
    max = std::stod(value);

    while(getline(file, value))
    {
        if(min > std::stod(value))
            min = std::stod(value);
        if(max < std::stod(value))
            max = std::stod(value);
    }

    std::cout << "apogee: " << max << ", perigee: " << min << "a: " <<  (min+max)/2 << std::endl; 
}