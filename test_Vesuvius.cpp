#include <liblas/liblas.hpp>
#include <fstream>
#include <iostream> // std::cout
#include <vector>
#include <array>
#include <iostream>

class Point
{
   public:
   Point(double x = 0., double y = 0., double z = 0.){
       m_coord[0] = x;
       m_coord[1] = y;
       m_coord[2] = z;
   }    

    double x() const {return m_coord[0];}
    double y() const {return m_coord[1];}
    double z() const {return m_coord[2];}

    double operator[]( size_t i) const { return m_coord[i];}
    double& operator[]( size_t i){ return m_coord[i];}

    Point operator-(const Point& v) const {
        return {x() - v.x(), y() - v.y(), z() - v.z()};
    }

    Point cross(const Point& v) const {
        return {
            y() * v.z() - z() * v.y(),
            z() * v.x() - x() * v.z(),
            x() * v.y() - y() * v.x()
        };
    }

    double dot(const Point& v) const {
        return x() * v.x() + y() * v.y() + z() * v.z();
    }
    private:
    std::array<double,3> m_coord;
};
using Vector = Point;


static 
std::vector<Point> st_pointsOnReflectorBand(const std::string& lasFilePath) 
{
    std::vector<Point> points;

    std::ifstream ifs(lasFilePath, std::ios::in | std::ios::binary);
    if (!ifs) {
        std::cerr << "Erreur d'ouverture du fichier LAS : " << lasFilePath << std::endl;
        return points;
    }

    liblas::ReaderFactory readerFactory;
    liblas::Reader reader = readerFactory.CreateWithStream(ifs);

    while (reader.ReadNextPoint()) 
    {
        const liblas::Point& point = reader.GetPoint();
        double intensity = static_cast<double>(point.GetIntensity());
        if(intensity >= 65535) // maximal inensity => the point is on the reflector strip band
        {           
            double x = point.GetX();
            double y = point.GetY();
            double z = point.GetZ();
            points.push_back({x, y, z});
        }
    }

    return points;
}

double computeVolume(const std::vector<Point>& triangles)
{
    double volume = 0.0;
    for (size_t i = 0; i < triangles.size(); i += 3)
    {
        const Point& a = triangles[i];
        const Point& b = triangles[i + 1];
        const Point& c = triangles[i + 2];
        volume += (a.cross(b)).dot(c);
    }
    return std::abs(volume) / 6.0;
}

int main()
{
    // Challenge 1 :
    Point centroid; // by default = {0., 0.,0.};
    std::vector<Point> pointsReflector = st_pointsOnReflectorBand("reflector.las");
    size_t nP = pointsReflector.size();

    if(nP > 0)  
    {
        for (const Point& P : pointsReflector) 
            for (size_t i  =0 ; i <= 2 ;++i)
                centroid[i] += P[i];

         std::cout << "Centroid: (" << centroid[0] << ", " << centroid[1] << ", " << centroid[2] << ")\n";
    }
    else std::cout << "No detected points on reflector strip band.\n";

        
    // Challenge 2 :
    std::ifstream file("mesh.stl");
    if (!file)
    {
        std::cerr << "Cannot open mesh.stl\n";
        return 1;
    }

    std::string line;
    std::vector<Point> triangles;
    while (std::getline(file, line))
    {
        if (line.find("vertex") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string dummy;
            double x, y, z;
            iss >> dummy >> x >> y >> z;
            triangles.push_back({x, y, z});
        }
    }

    double volume = computeVolume(triangles);
    std::cout << " Volume of our mesh is : " << volume << "\n";
    
    return 0;
}
