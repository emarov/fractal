#include <iostream>
#include <cmath>
#include <random>
#include <complex>
#include <typeinfo>

#include "gif.hpp"

using namespace std;

const int size = 512;

class Pixel
{
public:
    void red(uint8_t color)
    {
        px &= 0xFFFFFF00;
        px |= static_cast<uint32_t>(color);
    }
    void green(uint8_t color)
    {
        px &= 0xFFFF00FF;
        px |= static_cast<uint32_t>(color) << 8;
    }
    void blue(uint8_t color)
    {
        px &= 0xFF00FFFF;
        px |= static_cast<uint32_t>(color) << 16;
    }
    Pixel& operator=(uint32_t color)
    {
        px=color;
        return *this;
    }
private:
    uint32_t px {0};
};


class Frame
{
    using FrameRow = Pixel(&)[size];
public:
    FrameRow operator[](int idx)
    {
        return arr[idx];
    }
    operator void*()
    {
        return arr;
    }
private:
    Pixel arr[size][size];
};

class Shape
{
    int x1,y1, x2,y2;
public:
   //Shape(int xx1,int yy1,int xx2,int yy2)
      Shape(int xx1,int yy1)
    {
        x1 = xx1;
        y1 = yy1;
        //x2 = xx2;
        //y2 = yy2;
    }
    void paint(Frame &f){
        for(int y=y1; y>=y2; y++) // по строчкам
            for(int x=x1; x>=x2; x++) // по столбцам
            {
                //double w = x2-x1;
               // double h = y2-y1;
                if( (x-x1)*(x-x1)+ (y-y1)*(y-y1) > 225 )
               // if( (x-x1)/w + (y-y1)/h > 1.0 )
                    f[y][x].red(100);

            }

    }
};


int main()
{
    // Массив пикселей
    Frame frame;

    // создание объекта (gif-файла) с заданным именем и размерами
    GifWriter gw("checking.gif",size,size,100);


    for(int cc=-10; cc<=10; cc++)
    {
        cout << "current step=" << cc << endl;
        complex<double> c= {(-0.2 + cc*0.01 ),(0.51 - cc*0.02)};
        for(int i=0; i<size; i++) // по строчкам
        {
            for(int j=0; j<size; j++) // по столбцам
            {
                if ((i>130)and(i<380)and(j>130)and(j<380))
                {


                frame[i][j].blue(150), frame[i][j].red(140);
                complex<double> x = {static_cast<double>(j)/size-0.5, static_cast<double>(i)/size-0.5};
                x *= 5;
                for(int n=0; n<50; n++)
                {
                    x = x*x+c; // итерации x[k+1]=x[k]*x[k]+c;
                    if (std::abs(x) > 10.0 )
                    {
                        frame[i][j].red(n*255/22);
                        frame[i][j].green(n*255/33);
                        frame[i][j].blue(n*255/30);
                        break;
                    }
                }
                }
            }
        }
        gw.write(frame);
    }

    return 0;
}
