#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <ctime>

#include "bitmap.h"

using namespace std;

const int imageWidth = 1920, imageHeight = 1080;
Bitmap bitmap(imageWidth, imageHeight);
    

//Unbalanced, darkness favouring void cloud creator
int voidSeed(int luminosity)
{
    if (rand()%17<7){
        luminosity = rand()%luminosity;
        double power, darkness;
        power = (double)rand()/(double)RAND_MAX;
        darkness = pow((double)luminosity*17, power);	//Modify luminosity*number to amplify color range to suit the way the image looks. It should be darkness with faint purple.
        darkness = floor(darkness);
        return (int)darkness;
    }
    return rand()%1;
}

//Unbalanced, luminosity favouring star brightness creator
int starSeed(int luminosity)
{
    if (rand()%17>7){
        luminosity = rand()%luminosity;
        double power, brightness;
        power = (double)rand()/(double)RAND_MAX;
        brightness = pow((double) luminosity, power);
        brightness = floor(brightness);
        return (int)brightness;
    }
    return rand()%200+55;
}

//Dragon curve drawer
int breathX=imageWidth/2, breathY=imageHeight/2; 
char spark, way;

char Starwalk (int Shepard){
    Shepard=rand()%Shepard;
    switch (Shepard) {
        case 0:
        return 'u';
        break;

        case 1:
        return 'd';
        break;

        case 2:
        return 'l';
        break;

        case 3:
        return 'r';
        break;
    }
}


void turn(char spark){
    if (spark=='R'){
        switch (way){
            case 'u':
            way='r';
            break;

            case 'd':
            way='l';
            break;

            case 'r':
            way='d';
            break;

            case 'l':
            way='u';
            break;
        }
    }
    else{
        switch (way){
            case 'u':
            way='l';
            break;

            case 'd':
            way='r';
            break;

            case 'r':
            way='u';
            break;

            case 'l':
            way='d';
            break;   
        }
    }
}

void breathe(Bitmap& bitmap, int x, int y){
    x=breathX;
    y=breathY;
    switch (way){
        case 'u':
        while (y<=breathY+5){
            if(x<imageWidth && y<imageHeight && x>0 && y>0){
                bitmap.setPixel (x,y,255,255,255);
            }
            y++;
        }
        breathY=y; 
        break;

        case 'd':
        while (y>=breathY-5){
            if(x<imageWidth && y<imageHeight && x>0 && y>0){
                bitmap.setPixel (x,y,255,255,255);
            }
            y--;
        }
        breathY=y;
        break;

        case 'l':
        while (x<=breathX+5){
            if(x<imageWidth && y<imageHeight && x>0 && y>0){
                bitmap.setPixel (x,y,255,255,255);
            }
            x++;
        }
        breathX=x;
        break;

        case 'r':
        while (x>=breathX-5){
            if(x<imageWidth && y<imageHeight && x>0 && y>0){
                bitmap.setPixel (x,y,255,255,255);
            }
            x--;
        }
        breathX=x;
        break;

        default:
            bitmap.setPixel (x,y,255,0,0);

    }

}

void F(void){
    breathe (bitmap, breathX, breathY);
}

void R(void){
    turn('R');
}

void L(void){
    turn ('L');
}

void dragon(int n, bool nograd){
    if (nograd==0){
        if (n==0)
            F();
        else{
            dragon(n-1, 0);
            L();
            dragon(n-1, 1);
        }
    }
    else {
        if (n==0)
            F();
        else {
            dragon (n-1, 0);
            R();
            dragon (n-1, 1);
        }
    }
}

int main()
{
    int x, y, x0, y0, R, G, B;

    //Voidbirth
    srand(time(NULL));
    for(y0=0; y0<imageHeight; y0=y){
                B=voidSeed(3);
                G=0;
                R=B+voidSeed(5);
        for(x0=0; x0<imageWidth; x0=x){
                B=voidSeed(3);
                G=0;
                R=B+voidSeed(5);
            for(x=x0; x<x0+30; x++){
                for (y=y0; y<y0+30; y++){
                    bitmap.setPixel(x,y,R,G,B);
                }
            }
        }
    }

    //Starburst
    int star=0;
    int starDensity=17;

    do{
        R=starSeed(155);
        G=R;
        B=G;
        int constellationSpan = 5;
        for(int starPopulation=0; starPopulation<=rand()%5+1; starPopulation++){
            int constellationHeartX = (rand()%imageWidth-100) + (rand()%imageWidth/4-imageWidth/8);
            int constellationHeartY = (rand()%imageHeight-100)+ (rand()%imageHeight/4-imageHeight/8);
            for(x = 0; x < imageWidth; x++){
                for(y = 0; y < imageHeight; y++) {
                    int starSpan = sqrt((x-constellationHeartX) * (x-constellationHeartX) + (y-constellationHeartY) * (y-constellationHeartY));
                    if(starSpan<constellationSpan) bitmap.setPixel(x,y,R,G,B);
                }
            }
        }
    star++;
    }
    while (star< starDensity);

    //Flamebreathe
    way = Starwalk(4);  
    dragon (rand()%14+7, 0);

    bitmap.save((char*)"Starwalk.bmp");

    return 0;
}