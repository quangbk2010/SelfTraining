#include "CImg.h"
using namespace cimg_library;

#ifdef cimg_use_visualcpp6
#define std
#endif
#define COLOR_DEFIN(index, r,g,b) "my_color[#index][] = {#r, #g, #b}"

int main (int argc, char **argv) {
    cimg_usage ("MY game demo");
    CImgList<> ball (1,32,32,1,1,0);
    unsigned int W, H, score = 0;
    std::sscanf (cimg_option ("-g", "12x14", "Board dimension"), "%u%*c%u", &(W=12), &(H=14));
    CImg<> colors = CImg<>(3,7,1,1,194).fill(255,255,255, 255,0,255, 0,255,0, 255,255,0, 255,0,0, 0,128,255, 450,350,300);
    
    CImg<>&drawer = ball[0];
    drawer.draw_circle (16, 16, 14.0f, colors.get_shared_line(0).ptr() );

    const unsigned char green[] = {128, 255, 128}, white[] = {255, 255, 255}, red[] = {100,100,500};
    unsigned char my_color[256][256];
    CImgList<unsigned char> object(ball);
    CImg<unsigned char> background = CImg<unsigned char> (W * object[0].dimx(), H * object[0].dimy(), 1, 3, 0).noise(255,1).blur(255,0,50);
    bool redraw = true;

    // Begin user-interaction 
    CImg<unsigned char> board, selected_board, img(background);
    CImgDisplay disp (img, "NNQuang demo", 0, 2);
    while (!disp.is_closed) {
        // Init board

        if (!board) {
            (++((board.assign(W,H,1,1,5).noise(5,1))%=5)).get_shared_line(0).fill(0);
        }
        
        (img=background).draw_text (2,2,green,0,20,0.7f,"Score: %u\n", score);
		int i, j, k;
        for (i = 0; i < 256; i++) {
        	for (j = 0; j < 256; j++) 
        		for (k = 0; k < 256; k++) 
        			COLOR_DEFIN (i,i,j,k);
        }
        for (i = 0; i < 256; i++) 
        	img.draw_triangle (20 * (i + 1),20 * (i + 1),20 * (i + 1),20 * (i + 2),20 * (i + 2),20 * (i + 2),my_color[i+20],1,1);
        
        disp.display(img);
    }
}





