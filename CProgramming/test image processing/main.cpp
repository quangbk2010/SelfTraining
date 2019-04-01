#include "CImg.h"
#include  <iostream>
 
// Use the library namespace to ease the declarations afterward.
using namespace cimg_library;
using namespace std;
 
int main() {
 
  /*
  * VARIABLES ---------------------
  * Creation of two instances of images of unsigned char pixels.
  * The first image _image_ is initialized by reading an image file from the disk.
  * Here, lena.jpg must be in the same directory than the current program.
  * Note that you must also have installed the ImageMagick package
  * in order to be able to read JPG images.
  * var(size_x, size_y, size_z, dv, default_fill)
  */
  CImg<unsigned char> image("img/logo.bmp"),
          gray(image.width(), image.height(), 1, 1, 0),
          grayWeight(image.width(), image.height(), 1, 1, 0),
          imgR(image.width(), image.height(), 1, 3, 0),
          imgG(image.width(), image.height(), 1, 3, 0),
          imgB(image.width(), image.height(), 1, 3, 0);
 
  // for all pixels x,y in image
  cimg_forXY(image,x,y) {
    imgR(x,y,0,0) = image(x,y,0,0),    // Red component of image sent to imgR
    imgG(x,y,0,1) = image(x,y,0,1),    // Green component of image sent to imgG
    imgB(x,y,0,2) = image(x,y,0,2);    // Blue component of image sent to imgB
 
    // want to print out the RGB value of each pixel? Uncomment the following:
    /*
    * cout << image.width() << "x" << image.height() << endl;
    * cout << "(" << x << "," << y << ") ="
    *             << " R:" << (int)image(x,y,0,0)
    *             << " G:" << (int)image(x,y,0,1)
    *             << " B:" << (int)image(x,y,0,2) << endl;
    */
 
    // Separation of channels
    int R = (int)image(x,y,0,0);
    int G = (int)image(x,y,0,1);
    int B = (int)image(x,y,0,2);
    // Arithmetic addition of channels for gray
    int grayValue = (int)(0.33*R + 0.33*G + 0.33*B);
    // Real weighted addition of channels for gray
    int grayValueWeight = (int)(0.299*R + 0.587*G + 0.114*B);
    // saving píxel values into image information
    gray(x,y,0,0) = grayValue;
    grayWeight(x,y,0,0) = grayValueWeight;
}
 
  // 4 display windows, one for each image
  CImgDisplay main_disp(image,"Original"),
      //draw_dispR(imgR,"Red"),
      //draw_dispG(imgG,"Green"),
      //draw_dispB(imgB,"Blue"),
      draw_dispGr(gray,"Gray");
      //draw_dispGrWeight(grayPond,"Gray (Weighted)");
 
  // wait until main window is closed
  while (!main_disp.is_closed()){
      main_disp.wait();
  }
 
  return 0;
}
