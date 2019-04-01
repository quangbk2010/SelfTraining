#include "CImg.h"
using namespace cimg_library;

// The lines below are necessary when using a non-standard compiler as visualcpp6.
#ifdef cimg_use_visualcpp6
#define std
#endif

// Start main procedure
//----------------------
int main(int argc, char **argv) {

   // Read command line argument
  cimg_usage("A funny game featuring small colored balls.");
  unsigned int W, H, score = 0, previous_score = score, shape_score = 0;
  std::sscanf(cimg_option("-g","12x14","Board dimension"),"%u%*c%u",&(W=12),&(H=14));

  // Create game graphics from scratch
  CImgList<> balls0(7,32,32,1,3,0);
  CImg<> colors = CImg<>(3,7,1,1,194).fill(255,255,255, 255,0,255, 0,255,0,
                                           255,255,0, 255,0,0, 0,128,255, 450,350,300);
  cimglist_for(balls0,l) if (l) {
    CImg<>& ball = balls0[l];
    ball.draw_circle(16,16,14.0f,colors.get_shared_line(l).ptr());
    cimg_forXYV(ball,x,y,k) if (ball(x,y,k)) (ball(x,y,k)*=(32-x+y)/60.0f)+=20;
    ball.draw_circle(16,16,14.0f,colors.get_shared_line(l).ptr(),~0UL,0.5f).
      draw_circle(20,10,5.0f,colors.ptr(),0L,0.2f).draw_circle(22,8,2.0f,colors.ptr(),0L,0.4f).cut(0,255);
  }
  balls0[6].mirror('y').mirror('x');
  CImgList<unsigned char> balls(balls0);
  CImg<unsigned char>
    mask =  balls[1].get_cut(0,1).channel(0).dilate(3),
    background = CImg<unsigned char>(W*balls[0].dimx(),H*balls[0].dimx(),1,3,0).
    noise(255,1).blur(6,20,0).equalize_histogram(100,0,255);
  background.get_shared_channel(0)/=5; background.get_shared_channel(1)/=6; background.get_shared_channel(2)/=2;

  // Begin user-interaction loop.
  CImg<unsigned char> board, previous_board, selected_board, shape, img(background);
  CImgDisplay disp(img,"Jawbreaker",0,2);
  const unsigned char green[] = { 128,255,128 }, white[] = { 255,255,255 }, six = 6;
  bool redraw = true, gameover = false;
  float opac = 0.0f;
  while (!disp.is_closed) {

    
    // Init board
    if (!board) {
      (++((board.assign(W,H,1,1,5).noise(5,1))%=5)).get_shared_line(0).fill(0);
      opac = (float)(score = previous_score = shape_score = 0);
      previous_board = board;
      gameover = false; redraw = true;
    }

    // Draw graphical board
    //if (redraw) {
      (img=background).draw_text(2,2,green,0,24,0.7f,"Score : %u\n",score);
      if (selected_board) {
        cimg_forXY(selected_board,x,y) if (selected_board(x,y))
          img.draw_image(balls[selected_board(x,y)],mask,x*balls[0].dimx(),y*balls[0].dimy());
      } else cimg_forXY(board,x,y) if (board(x,y)) img.draw_image(balls[board(x,y)],mask,x*balls[0].dimx(),y*balls[0].dimy());
   // }
    /*if ((opac-=0.02f)>0) disp.display((+img).draw_text(disp.mouse_x-8,disp.mouse_y-54+(int)(30*opac),white,0,32,opac,"+%u",shape_score)).wait(20);
    else { */
      //if (redraw) { disp.display(img); redraw = false; } //else disp.wait(); //}
      disp.display(img);
  }
  return 0;
}