// Challenge 1 
#include <liblas/liblas.hpp>
#include <fstream>
#include <vector>
#include <array>
#include <iostream>

// Type pour stocker un point : x, y, z, intensité
using PointData = std::array<double, 4>;

static 
std::vector<PointData> readLASPoints(const std::string& lasFilePath) 
{
    std::vector<PointData> points;

    std::ifstream ifs(lasFilePath, std::ios::in | std::ios::binary);
    if (!ifs) {
        std::cerr << "Erreur d'ouverture du fichier LAS : " << lasFilePath << std::endl;
        return points;
    }

    liblas::ReaderFactory readerFactory;
    liblas::Reader reader = readerFactory.CreateWithStream(ifs);

    while (reader.ReadNextPoint()) {
        const liblas::Point& point = reader.GetPoint();
        double x = point.GetX();
        double y = point.GetY();
        double z = point.GetZ();
        double intensity = static_cast<double>(point.GetIntensity());

        points.push_back({x, y, z, intensity});
    }

    return points;
}

int main()
{
  return 0;
}

