/*-------------------------------------------------------------------------------

  File        : jawbreaker.cpp

  Description : A funny game featuring small colored balls.

  Copyright  : David Tschumperle - http://www.greyc.ensicaen.fr/~dtschump/

  This software is governed by the CeCILL-C license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL-C
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL-C license and that you accept its terms.

  ------------------------------------------------------------------------------*/

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
    if (redraw) {
      (img=background).draw_text(2,2,green,0,24,0.7f,"Score : %u\n",score);
      if (selected_board) {
        cimg_forXY(selected_board,x,y) if (selected_board(x,y))
          img.draw_image(balls[selected_board(x,y)],mask,x*balls[0].dimx(),y*balls[0].dimy());
      } else cimg_forXY(board,x,y) if (board(x,y)) img.draw_image(balls[board(x,y)],mask,x*balls[0].dimx(),y*balls[0].dimy());
    }
    if ((opac-=0.02f)>0) disp.display((+img).draw_text(disp.mouse_x-8,disp.mouse_y-54+(int)(30*opac),white,0,32,opac,"+%u",shape_score)).wait(20);
    else { if (redraw) { disp.display(img); redraw = false; } else disp.wait(); }

    // Handle key and window events
    if (disp.is_resized) disp.resize(disp);
    if (disp.key==cimg::keyBACKSPACE || disp.key==cimg::keySPACE) {
      board = previous_board; score = previous_score; selected_board.assign(); redraw = true; disp.key = 0;
    }
    if (disp.key==cimg::keyQ || disp.key==cimg::keyESC) { gameover = true; disp.key = 0; }

    // Handle ball selection and removal
    const int x = disp.mouse_x*board.dimx()/disp.dimx(), y = disp.mouse_y*board.dimy()/disp.dimy();
    if (disp.button&1 && x>=0 && y>=0) {
      disp.button=0;
      if (!board(x,y)) { selected_board.assign(); redraw = true; }
      else {
        if (!selected_board || selected_board(x,y)!=6) {
          (selected_board=board).draw_fill(x,y,0,&six,shape);
          if ((shape_score=(unsigned int)shape.sum())<2) selected_board.assign();
          else { (shape_score-=1)*=shape_score; opac = 1.0f; redraw = true; }
        } else {
          selected_board.assign();
          previous_board = board;
          previous_score = score;
          score += shape_score;
          board&=--shape;
          redraw = true;

          // Handle board modification due to ball removal
          for (int pmax = board.dimx(), p=0; p<pmax; ++p) {
            for (int q = board.dimy()-1, qs = q; q>=0; --q) {
              while (!board(p,qs)) --qs;
              board(p,q) = (qs>=0?board(p,qs--):0);
            }
            if (!board(p,board.dimy()-1)) {
              board.draw_image(board.get_crop(p,0,board.dimx()-1,board.dimy()-1).translate(1),p,0);
              if (p<pmax) { p--; pmax--; }
            }
          }

          // Test possible end of the game
          gameover = true;
          cimg_forXY(board,x,y)
            if (board(x,y) && ((y && board(x,y)==board(x,y-1)) || (x && board(x,y)==board(x-1,y)))) gameover = false;
        }
      }
      disp.button = 0;
    }

    if (gameover && opac<=0) {
      const CImg<unsigned char> text = CImg<unsigned char>().draw_text("Game Over !",0,0,white,0,48);
      img.draw_image(text,text,(img.dimx()-text.dimx())/2,(img.dimy()-text.dimy())/2,0,0,255,0.7f).display(disp);
      for (disp.button = disp.key = 0; !disp.is_closed && !disp.key && !disp.button; disp.wait())
        if (disp.is_resized) disp.resize();
      disp.button = disp.key = 0;
      board.assign();
    }
  }
  return 0;
}
