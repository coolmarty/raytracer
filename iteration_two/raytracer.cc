#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <float.h>
using namespace std;

class Vector; //forward declaration of vector for use in Point

class Point{
public:
  float x, y, z;
  Point(){
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
  }
  Point(float x1, float y1, float z1){
    this->x = x1;
    this->y = y1;
    this->z = z1;
  }
  void print(){
    cout << this->x << ", " << this->y << ", " << this->z;
  }
  Point add(Vector* v); //TO BE DEFINED SOON
  // float get_x(){
  //   return this->x;
  // }
  // float get_y(){
  //     return this->y;
  //   }
  // float get_z(){
  //   return this->z;
  //S}
  Point subtract(Vector* v);

  Vector subtract(Point p);

  Point subtract_p(Point p){
    return Point((this->x - p.x), (this->y - p.y), (this->z - p.z));
  }

  Point add_p(Point p){
    return Point((this->x + p.x), (this->y + p.y), (this->z -+p.z));
  }

  Point scale(float s){
    return Point(s*this->x, s*this->y, s*this->z);
  }
};


class Vector{
public:
  float x, y, z;
  Vector(){
    this->x = 1.0;
    this->y = 2.0;
    this->z = 3.0;
  }
  // float get_x(){
  //   return this->x;
  // }
  // float get_y(){
  //     return this->y;
  //   }
  // float get_z(){
  //   return this->x;
  // }
  Vector(float x1, float y1, float z1){
    this->x = x1;
    this->y = y1;
    this->z = z1;
  }
  Vector add(Vector v){
     return Vector((this->x + v.x), (this->y + v.y), (this->z + v.z));
  }
  Vector subtract(Vector v){
    Vector neg_v = v.scale(-1);
    return this->add(neg_v);
  }
  float multiply(Vector v){
    return(this->x*v.x + this->y*v.y + this->z*v.z);
  }
  float magnitude(){
    return sqrt((x*x) + (y*y) + (z*z));
  }
  Vector scale(float s){
    return Vector(s*this->x, s*this->y, s*this->z);
  }
  float dot(Vector v){
    return (this->x*v.x + this->y*v.y + this->z*v.z);
  }
  Vector normalize(){
    return (this->scale(1/this->magnitude()));
  }
  float find_angle_between(Vector v){
    Vector this_n = this->normalize();
    Vector v_n = v.normalize();
     return acos(this_n.dot(v_n));
  }

  Vector orthogonally_project(Vector v){
    return v.scale(this->dot(v)/v.dot(v));
  }
  Vector cross_product(Vector v){
    return Vector((this->y*v.z - this->z*v.y), (this->z*v.x - this->x*v.z), (this->x*v.y - this->y*v.x));
  }
  void print(){
    cout << this->x << ", " << this->y << ", " << this->z << " ";
  }
  Point to_point(){
    return Point(this->x, this->y, this->z);
  }
};

//Point functions that need Vector class
Point Point::add(Vector* v){
   return Point((this->x + v->x), (this->y + v->y), (this->z + v->z));
}
Point Point::subtract(Vector* v){
  return Point((this->x - v->x), (this->y - v->y), (this->z - v->z));
}
Vector Point::subtract(Point p){
  return Vector((this->x - p.x), (this->y - p.y), (this->z - p.z));
}

struct Color{
  float r, g, b;
  void print(){
    cout << this->r << ", " << this->g << ", " << this->b << " ";
  }
};

struct MTLColor{
  Color diffuse;
  float spec_r;
  float spec_g;
  float spec_b;
  float ka;
  float kd;
  float ks;
  int n;
};

struct Ray{
  Point origin;
  Vector direction;
  Point at(float t){
    Vector scaled_v = direction.scale(t);
    return this->origin.add(&scaled_v);
  }
};

class Object{
  public:
    Point center;
    int corresponding_mtl;
    float radius;

};

struct Light{
  Point position;
  Vector direction;
  Color c;
  int w;
};

// class Sphere : public Object{
// //  public:
// //    float radius;
// };
//declarations
vector<Object> obj_array;
vector<MTLColor> mtl_array;
vector<Light> light_array;
MTLColor m;
Light light;
int objcount;
int mtlcount;
int lightcount = 0;
Color bkg;
Point eye;
int img_height, img_width;
float vfov;
Vector viewdir;
Vector updir;
int d;
Vector n;
Point ul, ur, ll, lr;
//setup
void init_defaults(){
  m.diffuse.r = 0;
  m.diffuse.g = 0;
  m.diffuse.b = 1;
  mtl_array.push_back(m);
  bkg.r = 0.53;
  bkg.g = 0.53;
  bkg.b = 0.53;
  img_height = 100;
  img_width = 250;
  d = 5;


  ul.x = 2;
  ul.y = 2;
  ul.z = 2;

  ur.x = 2;
  ur.y = 2;
  ur.z = -2;

  ll.x = 2;
  ll.y = -2;
  ll.z = 2;

  lr.x = 2;
  lr.y = -2;
  lr.z = -2;
  return;
}

Color shade_ray(int o_count, float quadratic_result, Ray r){
  float ambient_r = (mtl_array[obj_array[o_count].corresponding_mtl].ka)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.r);
  float ambient_g = (mtl_array[obj_array[o_count].corresponding_mtl].ka)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.g);
  float ambient_b = (mtl_array[obj_array[o_count].corresponding_mtl].ka)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.b);
  //cout << "HEY! AMBIENT R IS: " << ambient_r << endl;
  Color c;
  c.r = ambient_r;
  c.g = ambient_g;
  c.b = ambient_b;
  Vector N = ((r.at(quadratic_result).subtract(obj_array[o_count].center)));
  N = N.scale(1/obj_array[o_count].radius);
  N = N.normalize();
  //cout << "VECTOR N IS NOW: ";
  //N.print();
  //cout << " EXPECTED: 0 0 1" << endl;
  Vector L;
  for (int l= 0; l < lightcount; l++){
    if(light_array[l].w == 1){ //point light source
    //  cout << "point" << endl;
      L = r.at(quadratic_result).subtract(light_array[l].position);
      L = L.normalize();
      L = L.scale(-1);
    }
    else{ //directional light source
    //  cout << "directional";
      L = light_array[l].direction.scale(-1);
      L = L.normalize();
    }
    float NL = N.dot(L);
    // cout << "N is: ";
    // N.print();
    // cout << endl;
    // cout << "L is: ";
    // L.print();
    // cout << endl;
    //cout << "therefore NL is: " << NL << endl;
    //  cout << mtl_array[obj_array[o_count].corresponding_mtl].diffuse.r << endl;
    int shadow = 1;
    Ray shadow_check;
    shadow_check.origin = r.at(quadratic_result);
    shadow_check.direction = L;
    for(int o = 0; o < objcount; o++){

      Vector oc = shadow_check.origin.subtract(obj_array[o].center);
      float B_coeff = 2.0*(shadow_check.direction.dot(oc));
      float C_coeff = oc.dot(oc)-(obj_array[o].radius*obj_array[o].radius);
      float discriminant = ((B_coeff*B_coeff)-4*C_coeff);
      if(discriminant >= 0 && o != o_count){//intersects with object
        // cout << "SPHERE HIT, TRACING COLOR: ";
        // obj_array[o].color.print();
        // cout << endl;
        shadow = 0;
      }
    }
    float diffuse_r = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].kd)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.r)*NL;
    float diffuse_g = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].kd)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.g)*NL;
    float diffuse_b = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].kd)*(mtl_array[obj_array[o_count].corresponding_mtl].diffuse.b)*NL;
    //  cout << "diffuse_r was: " << diffuse_r << endl;
    Vector H = L.add(r.at(quadratic_result).subtract(eye));
    H = H.normalize();
    //  cout << "H vector is equivalent to: ";
    //  H.print();
    //  cout << endl;
    float NH = N.dot(H);
    float NH_pow = pow(NH, mtl_array[obj_array[o_count].corresponding_mtl].n);
    //  cout << "NH raised to the power of: " <<  mtl_array[obj_array[o_count].corresponding_mtl].n << " is: " << NH_pow << endl;
    float specular_r = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].ks)*(mtl_array[obj_array[o_count].corresponding_mtl].spec_r)*NH_pow;
    float specular_g = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].ks)*(mtl_array[obj_array[o_count].corresponding_mtl].spec_g)*NH_pow;
    float specular_b = shadow*(mtl_array[obj_array[o_count].corresponding_mtl].ks)*(mtl_array[obj_array[o_count].corresponding_mtl].spec_b)*NH_pow;
    //  cout << "just for fun, specular_r is : " << specular_r << endl;
    c.r += max((float) 0,diffuse_r) + max((float) 0,specular_r);
    c.g += max((float) 0,diffuse_g) + max((float) 0,specular_g);
    c.b += max((float) 0,diffuse_b) + max((float) 0,specular_b);
//  cout << "ADDED TOGETHER IS: ";
//  c.print();
//  cout << endl;

  }
  return c;
}

Color trace_ray(Ray r){
  float best_discrim = FLT_MAX;
  float best_B_coeff;
  float best_A_coeff;
  int closest = -1;
  for(int o = 0; o < objcount; o++){
    Vector oc = r.origin.subtract(obj_array[o].center);
    float A_coeff = r.direction.dot(r.direction);
    float B_coeff = 2.0*(oc.dot(r.direction));
    float C_coeff = oc.dot(oc)-(obj_array[o].radius*obj_array[o].radius);
    float discriminant = ((B_coeff*B_coeff)-4*A_coeff*C_coeff);
    if(discriminant >= 0){//intersects with object
      if(discriminant < best_discrim){
        best_discrim = discriminant;
        best_B_coeff = B_coeff;
        best_A_coeff = A_coeff;
        closest = o;
      }
    }
  }
  if(closest == -1){
    return bkg;
  }
  return shade_ray(closest, ((best_B_coeff*-1) - sqrtf(best_discrim))/(2.0*best_A_coeff), r);
}

int main(int argc, char** argv){
  cout << "start" << endl;
  init_defaults();
  ifstream infile;
  ofstream outfile;
  infile.open(argv[1]);
  string line;
 // int debug = 0;
  string keyword;
  cout << "made it through break point: " << "initialization" << endl;
  while(getline(infile, line)){
 //   cout << "line is now : " << line << endl;
    stringstream ss(line);
    getline(ss,keyword,' ');
 //   cout << "keyword is now : " << keyword << endl;
    char first = keyword[0];
    char second = keyword[1]; //a switch statement is used here so the order of the keywords can vary
  //  cout << "made it through break point: " << first << second << endl;
    switch(first){
      case 'e': //eyesize
        getline(ss, line, ' ');
 //       cout << "line is now : " << keyword << endl;
        eye.x = stof(line);
 //       cout << "line is now : " << line << endl;
        getline(ss, line, ' ');
        eye.y = stof(line);
        getline(ss, line);
        eye.z = stof(line);
        cout << "Eye is positioned at: " << eye.x << ", " << eye.y << ", " << eye.z << endl;
        break;

      case 'i': //imsize
        getline(ss, line, ' ');
        img_height = stoi(line);
        getline(ss, line);
        img_width = stoi(line);
        cout << "Image size is now: " << img_height << " by: " << img_width << endl;
        break;
      case 'b': //bkgcolor
        getline(ss, line, ' ');
        bkg.r = stof(line);
        getline(ss, line, ' ');
        bkg.g = stof(line);
        getline(ss, line, ' ');
        bkg.b = stof(line);
        cout << "Background color is " << bkg.r << ", " << bkg.g << ", " << bkg.b << endl;
        break;
      case 's':{//sphere
        Object sph;
        getline(ss, line, ' ');
        sph.center.x = stof(line);
        getline(ss, line, ' ');
        sph.center.y = stof(line);
        getline(ss, line, ' ');
        sph.center.z = stof(line);
        getline(ss, line, ' ');
        sph.radius = stof(line);
      //  cout << "here" << endl;
      //  sph.color = mtl_array.back().diffuse;
      //  cout << "now here" << endl;
        sph.corresponding_mtl = mtlcount;
        obj_array.push_back(sph);
        objcount++;
        cout << "Sphere positioned at: " << sph.center.x << ", " << sph.center.y << ", "  <<  sph.center.z << endl;
        cout << "With radius: " << sph.radius << " units" << endl;
        cout << "And corresponding material color: " << sph.corresponding_mtl << endl;
      }
        break;
      case 'm': //mtlcolor
        MTLColor mtl;
        getline(ss, line, ' ');
        mtl.diffuse.r = stof(line);
        getline(ss, line, ' ');
        mtl.diffuse.g = stof(line);
        getline(ss, line, ' ');
        mtl.diffuse.b = stof(line);
        getline(ss, line, ' ');
        mtl.spec_r = stof(line);
        getline(ss, line, ' ');
        mtl.spec_g = stof(line);
        getline(ss, line, ' ');
        mtl.spec_b = stof(line);
        getline(ss, line, ' ');
        mtl.ka = stof(line);
        getline(ss, line, ' ');
        mtl.kd = stof(line);
        getline(ss, line, ' ');
        mtl.ks = stof(line);
        getline(ss, line, ' ');
        mtl.n = stoi(line);
        mtl_array.push_back(mtl);
        mtlcount++;
        cout << "Material diffuse color has changed to: " << mtl.diffuse.r << ", " << mtl.diffuse.g << ", " << mtl.diffuse.b << endl;
        cout << "With specular coloro: " << mtl.spec_r << ", " << mtl.spec_g << ", " << mtl.spec_b << endl;
        cout << "Ambient coefficient: " << mtl.ka << " diffuse coefficient: " << mtl.kd << " specular coefficient: " << mtl.ks << endl;
        cout << "Specular highlight exponent: " << mtl.n << endl;
        break;
      case 'u': //updir
        getline(ss, line, ' ');
        updir.x = stof(line);
        getline(ss, line, ' ');
        updir.y = stof(line);
        getline(ss, line, ' ');
        updir.z = stof(line);
        cout << "The 'up' vector is: " << updir.x << ", " << updir.y << ", " << updir.z << endl;
        break;
      case 'l': //light input
        getline(ss, line, ' ');
        light.position.x = stof(line);
        getline(ss, line, ' ');
        light.position.y = stof(line);
        getline(ss, line, ' ');
        light.position.z = stof(line);
        getline(ss, line, ' ');
        light.w = stoi(line);
        getline(ss, line, ' ');
        light.c.r = stof(line);
        getline(ss, line, ' ');
        light.c.g = stof(line);
        getline(ss, line, ' ');
        light.c.b = stof(line);
        //cout << "light found!" << endl;
        if(light.w == 1){
          cout << "Point ";
        }
        else{
          cout << "Directional ";
          light.direction.x = light.position.x;
          light.direction.y = light.position.y;
          light.direction.z = light.position.z;
          light.direction.normalize();
        }
        cout << "light created with position: ";
        light.position.print();
        cout << " and color: ";
        light.c.print();
        cout << endl;
        light_array.push_back(light);
        lightcount++;
        break;
      case 'v': //there are two possible keywords that begin with the letter v, so we'll switch again on the second character
        switch(second){
          case 'i': //viewdir
            getline(ss, line, ' ');
            viewdir.x = stof(line);
            getline(ss, line, ' ');
            viewdir.y = stof(line);
            getline(ss, line, ' ');
            viewdir.z = stof(line);
            cout << "The 'viewing' vector is: " << viewdir.x << ", " << viewdir.y << ", " << viewdir.z << endl;
            break;
          case 'f': //vfov
            getline(ss, line, ' ');
            vfov = stof(line);
            cout << "Vertical FOV is : " << vfov << endl;
            break;
          default:
            cout << "huh_v?!" << endl;

        }
        break;
      default:
        cout << "huh?!" << endl;
    }
  }
  infile.close();
  cout << "infile closed" << endl;
  //END READING INPUT

  //BEGIN PIXEL ARRAY
  Color pixels[img_height][img_width];
  for(int x = 0; x < img_width; x++){
    for(int y = 0; y < img_height; y++){
      pixels[x][y].r = 0.8;
        pixels[x][y].g = 0.1;
        pixels[x][y].b = 0.7;
    }
  }
  //DEFINE VIEWING WINDOW
  cout << "VIEW DISTANCE IS: " << d << endl;
  vfov = (vfov / 180.0f)*3.14159f;
  float view_height = 2*d*(tan(vfov * 0.5));
  cout << "VIEWING WINDOW HEIGHT IS NOW: " << view_height << " EXPECTED: 10" << endl;
  float aspect = img_width / img_height;
  cout << "ASPECT RATIO IS NOW: " << aspect << " EXPECTED: 1" << endl;
  float view_width = aspect*view_height;
  cout << "VIEWING WINDOW WIDTH IS NOW: " << view_width << " EXPECTED: 10" << endl;

  Vector u_prime = viewdir.cross_product(updir);
  Vector u = u_prime.normalize();
  Vector v_prime = u.cross_product(viewdir);
  Vector v = v_prime.normalize();


  cout << "u is now: "<< endl;
  u.print();
  cout << endl;
  cout << "v is now: " << endl;
  v.print();
  cout << endl << endl;


  Vector n = viewdir.normalize();
  Vector dn = n.scale(d);
  Vector w_2_u = u.scale(view_width*0.5);
  Vector h_2_v = v.scale(view_height*0.5);

  //
  // cout << "TESTING ADDITION: " << endl;
  // (eye.add(&dn)).print();
  // cout << endl;
  // cout << "w2u is: ";
  // w_2_u.print();
  // cout << endl;
  // cout << "h2v is: ";
  // h_2_v.print();
  // cout << endl;
  Point viewwindowlocation = eye.add(&dn);
  cout << "VIEW WINDOW CENTER RESIDES AT: ";
  viewwindowlocation.print();
  cout << endl;

  ul = (((viewwindowlocation).subtract(&w_2_u)).add(&h_2_v));
  ur = (((viewwindowlocation).add(&w_2_u)).add(&h_2_v));
  ll = (((viewwindowlocation).subtract(&w_2_u)).subtract(&h_2_v));
  lr = (((viewwindowlocation).add(&w_2_u)).subtract(&h_2_v));

  cout << "VIEWING WINDOW COORDINATES: " << endl;
  cout << "UPPER LEFT CORNER: ";
  ul.print();
  cout << endl;
  cout << "UPPER RIGHT CORNER: ";
  ur.print();
  cout << endl;
  cout << "LOWER LEFT CORNER: ";
  ll.print();
  cout << endl;
  cout << "LOWER RIGHT CORNER: ";
  lr.print();
  cout << endl;

 Vector delta_h = ur.subtract(ul).scale(1.0/(img_width));
 Vector delta_v = ll.subtract(ul).scale(1.0/(img_height));

 Vector center_h = (ur.subtract(ul).scale(1.0/(2*img_width)));
 Vector center_v = (ll.subtract(ul).scale(1.0/(2*img_height)));
 Point pix_centers_arr[img_height][img_width];
 for(int i = 0; i < img_width; i++){
   for(int j = 0; j < img_height; j++){
     Vector i_delta_h = delta_h.scale(i);
     Vector j_delta_v = delta_v.scale(j);
     Point center_of_pixel = (((ul.add(&i_delta_h)).add(&j_delta_v)).add(&center_h)).add(&center_v);
     // cout << "CENTER OF VIEW WINDOW PIXEL [" << i << "][" << j << "] ";
     // center_of_pixel.print();
     // cout << endl;
     pix_centers_arr[i][j] = center_of_pixel;
   }
 }

 for(int x = 0; x < img_width; x++){
   for(int y = 0; y < img_height; y++){
     Ray r;
     r.origin.x = eye.x;
     r.origin.y = eye.y;
     r.origin.z = eye.z;
     r.direction = (pix_centers_arr[x][y].subtract(r.origin));
     r.direction = r.direction.normalize();
     cout << "shooting through pixel at: ";
     pix_centers_arr[x][y].print();
     cout << endl;
     // cout << "ray from : " << x << " , " << y << " normalized direction is: ";
    // r.direction.print();
    // cout << endl;
     pixels[y][x] = trace_ray(r);
   }
 }

  outfile.open(argv[2]);
  outfile << "P3"<< endl << img_width << " " << img_height << endl << 255 << endl; //end header
  cout << "made it through break point: " << "opening output file" << endl;

  for(int x = 0; x < img_width; x++){
    for(int y = 0; y < img_height; y++){

      outfile << (int) (pixels[x][y].r * 255) << " ";

      outfile << (int) (pixels[x][y].g * 255) << " ";

      outfile << (int) (pixels[x][y].b * 255) << endl;

    }
  }
  // cout << endl << endl << "TESTING VECTOR FUNCTIONS" << endl;
  //
  // Vector norman = Vector(1,1,1);
  // cout << "MY NORMALIZATION OUTPUTS: " << norman.normalize().x << ", " << norman.normalize().y << ", " << norman.normalize().z << " EXPECTED: 0.57, 0.57, 0.57" << endl;
  //
  // Vector maggie = Vector(3,7,1);
  // cout << "MY MAGNITUDE OUTPUTS: " << maggie.magnitude() << " EXPECTED: 7.68 " << endl;
  //
  // Vector alpha = Vector(1,2,3);
  // Vector beta = Vector(4,-5,6);
  // cout << "MY DOT PRODUCT OUTPUTS: " << alpha.dot(beta) << " EXPECTED: 12" << endl;
  //
  // Vector a = Vector(1,2,2);
  // Vector b = Vector(0,3,4);
  // cout << "MY ANGLE BETWEEN OUTPUTS: " << a.find_angle_between(b) << " EXPECTED : 0.37" << endl;
  //
  // Vector marty = Vector(1,2,2);
  // Vector prita = Vector(0,3,4);
  // cout << "MY ORTHOGONAL PROJECTION OUTPUTS: ";
  // marty.orthogonally_project(prita).print();
  // cout << " EXPECTED: 0, 1.68, 2.24" << endl;
  //
  // Vector tom = Vector(1,2,3);
  // Vector colette = Vector(4,5,6);
  // cout << "MY CROSS PRODUCT OUTPUTS : ";
  // tom.cross_product(colette).print();
  // cout << " EXPECTED: -3, 6, -3" << endl;
  //
  // Point mine = Point(8,23,99);
  // Vector alexs = Vector(2,12,0);
  // Point please = mine.subtract(&alexs);
  // cout << "I AM NOW TESTING SUBTRACTING A VECTOR FROM A POINT. EXPECTED: 6 11 99 ACTUAL: ";
  // please.print();
  // cout << endl;

  outfile.close();
  cout << "made it through final break point." << endl;
  return 0;
}
