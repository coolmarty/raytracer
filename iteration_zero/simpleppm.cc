#include <iostream>
#include <fstream>
#include <string>

struct Color{
  int r;
  int g;
  int b;
};

int main(int argc, char** argv){
  std::ifstream infile;
  std::ofstream outfile;
  infile.open(argv[1]);
  std::string line;
  std::getline(infile, line, ' '); //throw out the imsize leading string
  std::getline(infile, line, ' ');
  int img_height = stoi(line);
  std::getline(infile, line);
  int img_width = stoi(line);
  infile.close();
  
  outfile.open("output.ppm");
  outfile << "P3"<< std::endl << img_width << " " << img_height << std::endl << 255 << std::endl; //end header
  
  Color color_left, color_right;
  color_left.r = 172;
  color_left.g = 82; //this is a shade of lavender
  color_left.b = 199;
  
  color_right.r = 116;
  color_right.g = 242; //this is a shade of teal
  color_right.b = 236;
  
  Color current;
  current = color_left;
  for(int x = 0; x < img_width; x++){
    for(int y = 0; y < img_height; y++){
      outfile << current.r + (color_right.r - color_left.r)/img_width << " ";
      current.r += (color_right.r - color_left.r)/img_width;
      outfile << current.g + (color_right.g - color_left.g)/img_width << " ";
      current.g += (color_right.g - color_left.g)/img_width;
      outfile << current.b + (color_right.b - color_left.b)/img_width << " ";
      current.b += (color_right.b - color_left.b)/img_width;
      outfile << std::endl;

    }
    current = color_left;
  }
  
  outfile.close();
  return 0;

}
