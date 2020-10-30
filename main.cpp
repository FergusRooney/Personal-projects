#include "mbed.h"
#include "C12832.h"
#include <math.h>

/*
   This basic example just shows how to read the ADC internal channels raw values.
   Please look in the corresponding device reference manual for a complete
   description of how to make a temperature sensor, VBat or Vref measurement.
*/
//128*32 paddle
C12832 lcd(D11, D13, D12, D7, D10); 
class paddle{
    private:
    AnalogIn Pot;
    public:
        int x,y,width, height,score; 
        paddle(int px, int py,int w,int h, PinName pot) : x(px), y(py), width(w),height(h),Pot(pot) {}

        void start(){
                score =0;
            }
        void draw(){
                lcd.fillrect(x,y,x+width,y+height,1);        
            }         
        void update(){
                y =floor(Pot*31);
                draw();
            }
    };
    
class ball{
    private:
        int x,y,width,height,dx,dy;
    public:
        
        ball(int px, int py,int w,int h) : x(px), y(py),  width(w),height(h){start();}
        void start(){
                x = 64;
                y = 16;
                dx = -1;
                dy = 1;
            }
        void draw(){
                lcd.fillrect(x,y,x+width,y+height,1);          
            }   
        int getx(){
            return x;
            }
        void flipdx(){
            dx = dx*-1;
                
            }
        bool collides(paddle Paddle){
            if (x > Paddle.x + Paddle.width or Paddle.x > x + width){
                return false;
            }
            if (y > Paddle.y + Paddle.height or Paddle.y > y + height ){
                return false;
            }
            return true;
            
            }    
        void update(){
                if(y ==1 || y ==31){
                    dy = -dy;
                    }
                x = x+ dx;
                y=y+dy;
                draw();
                
            }
    };
    
void playerScore(paddle paddle1, paddle paddle2){
        lcd.locate(10,0);
        lcd.printf("P1: %d", paddle1.score ); 
        lcd.locate(80,0);
        lcd.printf("P2: %d", paddle2.score ); 
        
    }
    
int main()
{
    int framecounter=0;
    paddle paddle1(2,0,4,3,A0);
    paddle paddle2(123,0,4,3,A1);
    ball ball1(64,16,2,2);
    ball1.start();
    lcd.cls();  
    paddle1.score=0;
    paddle2.score=0;
    
    
    while(1) {
      framecounter++;
      paddle1.update();
      paddle2.update();
      ball1.update();
      playerScore(paddle1,paddle2);
      if(ball1.collides(paddle1) || ball1.collides(paddle2)){
          ball1.flipdx();
          }
      if(ball1.getx() ==1){
            paddle2.score++;
            ball1.start();
            if(paddle2.score%2==0){
                ball1.flipdx();
            }
        }
       if(ball1.getx() ==126){
            paddle1.score++;
            ball1.start();
             if(paddle1.score%2==0){
                ball1.flipdx();
            }
        }   
      if(framecounter>10000){
          framecounter=0;
          }
      if(paddle1.score>=5){
          lcd.locate(60,16);
          lcd.printf("P1 wins!" ); 
          wait(5);
          paddle1.score=0;
          paddle2.score=0;
          ball1.start();
      }
      if(paddle2.score>=5){
          lcd.locate(60,16);
          lcd.printf("P2 wins!" );
          wait(5);
          paddle1.score=0;
          paddle2.score=0;
          ball1.start();
      }
      wait(0.03);
      lcd.cls();
    }
}
