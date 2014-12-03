/*****************************************************
 * Program: line.c   Coded by: Harry Li              *
 * Version: x1.0;    status: tested;                 *
 * Compile and build:                                *
 * gcc -lGL -lGLU -lglut line.c -o main.o            *
 * Date: Jun 5, 2014                                 *
 * Purpose: Graphics program to prepare visulization *
 *          of Bresenham circle.                     *
 *****************************************************/

#include<stdio.h>

FILE* wrp;

int getArcPoint(int x, int y_previous, int r)
{
//#include <math.h>
//#include <stdio.h>

int dist1, dist2;
int func1, func2;

//-------compute functions----------------------//
//printf( "sub: x = %4d y = %4d \n", x, y_previous);

func1 = r * r - (x * x + y_previous * y_previous);
func2 = r * r - (x * x + (y_previous - 1) * (y_previous - 1));

dist1 = func1 * func1;
dist2 = func2 * func2;

//printf( "sub: x = %4d y = %4d \n", x, y_previous);

//------compute next y-------------------------//
if (dist1 == 0)
   {
   y_previous = y_previous;
   }
else if (dist2 == 0)
   {
   y_previous = y_previous - 1;
   }
else if (dist1 < dist2)
   {
   y_previous = y_previous;
   }
else if (dist2 < dist1)
   {
   y_previous = y_previous - 1;
   }

return (y_previous);

}

void mydisplay()
{

//------------Graphics---------------------------//
/* define virtex for the outter big square
   (p4x,p4y) ---------- (p1x,p1y)
  (0.0f,1.0f)          (1.0f,1.0f)
       |                    |
       |                    |
       |                    |
   (p3x,p3y) ----------  (p2x,p2y)
  (0.0f,0.0f)           (1.0f,0.0f)
*/

float p1x=1.0f,p1y=1.0f;
float p2x=1.0f,p2y=0.0f;
float p3x=0.0f,p3y=0.0f;
float p4x=0.0f,p4y=1.0f;

//int r;
float grid, float_r;
int i=0, j=0;

/*define the x-y axis

             (px4x,px4y)
             (0.5f,1.0f)
                  |
                  |
                  |
   (px1x,px1y) -------- (px2x,px2y)
   (0.0f,0.5f)    |     (1.0f,0.5f)
                  |
                  |
             (px3x,px3y)
             (0.5f,0.0f)
*/

float px1x=0.0f,px1y=0.5f, px2x=1.0f, px2y=0.5f;
float px3x=0.5f,px3y=0.0f, px4x=0.5f, px4y=1.0f;

//--------------Bresenham circle module------------

//#include <stdio.h>
//#include <math.h>

//---------------------------------------------------//
// Parameter passing                                 //
//---------------------------------------------------//


//--------- Bresenham circle -----------------//
int r, x[1024], y[1024], x_buf[1024], y_buf[1024]; //stored in x_buf, y_buf
int i_x, i_y, i_temp, i_accumulator;        // No. of pts of circle array
int upperBound, i_offset;
int i_shift1, i_shift2;                     // value of i for points on the axis

   printf("Please Enter radius :\n");
   scanf("%d", &r);
   printf("Radius = %4d \n",r);

// find 1/8 Bresenham circle upper bound, e.g., pi/4 angle pt
   upperBound = r * 0.707106;
   y[1] = r;

// on +y axis point
   x[0] = 0; x_buf[0] = 0;
   y[0] = r; y_buf[0] = r;
//   printf("on +y axis x = %4d y = %4d \n", x[0], y[0]);
   i_accumulator = 0;
//-------------------------------------------//
// Bresenham 1/8 of the circle pts           //
//-------------------------------------------//
   for(i = 1; i <= upperBound; i++) {
   i_accumulator++;
   x[i] = i;
   x_buf[i] = x[i];
   i_y = getArcPoint(x[i], y[i], r);
   y[i] = i_y;
   y_buf[i] = y[i];
   y[i+1] = y[i];
   printf( "I0 i = %4d | x = %4d y = %4d \n", i_accumulator, x_buf[i], y_buf[i]);
}

//----------------------------------------------//
// Symmetric pts in I (the other 1/8 of the cirlce
//----------------------------------------------//
//*******footnote on the symmetric **************
/* (1) x(Delta + 1) = y (Delta)               ***
 *     y(Delta + 1) = x (Delta)               ***
 * (2) x(Delta + 2) = y (Delta - 2 + 1)       ***
 *     y(Delta + 2) = x (Delta - 2 + 1)       ***
 * (3) x(Delta + i) = y (Delta - i + 1)       ***
 *     y(Delta + i) = x (Delta - i + 1)       ***
 * (4) x(Delta + Delta)=y (Delta - Delta + 1) ***
 *     y(Delta + Delta)=x (Delta - Delta + 1) ***
 * where Delta is the upperBound              ***
 ************************************************/
//   printf( "Symmetric pts in I \n");
   for(i = 1; i <= upperBound; i++) {
   i_accumulator++;
   i_temp = x[upperBound - i + 1];
   x_buf[upperBound + i] = y[upperBound - i + 1]; //step through known 1/8 pts
   y_buf[upperBound + i] = i_temp;                //update the other 1/8 pts
   printf( "I1 i = %4d | x = %4d y = %4d \n", i_accumulator, x_buf[upperBound + i], y_buf[upperBound + i]);
}

//accumulator update and take care of the point on +x axis
   i_accumulator = i_accumulator + 1;
   x_buf[i_accumulator] = r;
   y_buf[i_accumulator] = 0;
   printf("+x Ax i= %2d x = %4d y = %4d \n", i_accumulator, x_buf[i_accumulator], y_buf[i_accumulator]);
   i_shift1 = i_accumulator;

//------------------------------------------//
// Symmetric pts in II (region -x +y axis)  //
// (x, y) = (-x, y)                         //
//------------------------------------------//
   i_offset = i_accumulator;
//   printf("\ni_offset = %d\n", i_offset);
   for(i = 1; i <= 2*upperBound + 1; i++) {  // +1 due to a pt on the axis
   i_accumulator++;
   i_x = - x_buf[i_offset - i + 1]; //step through known 1/4 circle
   x_buf[i_accumulator] = i_x;
   i_y = y_buf[i_offset - i + 1];
   y_buf[i_accumulator] = i_y;
   printf( "II i = %4d | x = %4d y = %4d \n", i_accumulator, x_buf[i_accumulator], y_buf[i_accumulator]);
}
   i_shift2 = i_accumulator;

//-----------------------------------------//
// Symmetric pts in III (-x -y)            //
// (x, y) = (-x, -y)                       //
// and pts in IV (-x, y)                   //
//-----------------------------------------//

   i_offset = i_accumulator;

   //accumulator defines total number of circle pts

   for(i = 1; i <= 4*upperBound + 2; i++) {  // +1 due to a pt on the axis
   i_accumulator++;
   i_x = x_buf[i_offset - i + 1]; //step through known 1/4 circle
   x_buf[i_accumulator] = i_x;
   i_y = - y_buf[i_offset - i + 1];
   y_buf[i_accumulator] = i_y;
   printf( "Half i = %4d | x = %4d y = %4d \n", i_accumulator, x_buf[i_accumulator], y_buf[i_accumulator]);

   //Bresenham cirlec pts are in x_buf[] and y_buf[]
}

i_accumulator++;
printf("i_accumulator = %d\n", i_accumulator);
x_buf[i_accumulator] = 0;
y_buf[i_accumulator] = -1 * r;

int l_temp;
for (l_temp = 0; l_temp <= i_accumulator; l_temp++)
{
  printf("x_temp = %d ", x_buf[l_temp]);
  printf("y_temp = %d\n", y_buf[l_temp]);
}

//fprintf(wrp, "%d\n", r);
int l;
char x1='x', y1='y';
fprintf(wrp, "%c, %c\n", x1, y1);
for (l = 0; l <= i_accumulator; l++)
{
  fprintf(wrp, "%d, %d\n", x_buf[l], y_buf[l]);
}
//fclose(wrp);



/* Getting rid of the extra points on the axis and array values adjustment */
for (j = 1; j < i_shift1; j++) {
	x_buf[j-1] = x_buf[j];
	y_buf[j-1] = y_buf[j];
	printf("I. x_buf[%d] = %d, y_buf[%d] = %d\n", j-1, x_buf[j-1], j-1, y_buf[j-1]);
}

for (j = i_shift1+2; j <= i_shift2; j++) {
	x_buf[j-3] = x_buf[j];
	y_buf[j-3] = y_buf[j];
	printf("II. x_buf[%d] = %d, y_buf[%d] = %d\n", j-3, x_buf[j-3], j-3, y_buf[j-3]);
}

for (j = i_shift2+1; j < i_shift2 + i_shift1; j++) {
	x_buf[j-3] = x_buf[j];
	y_buf[j-3] = y_buf[j];
	printf("III. x_buf[%d] = %d, y_buf[%d] = %d\n", j-3, x_buf[j-3], j-3, y_buf[j-3]);
}

for (j = i_shift2 + i_shift1 + 2; j <= i_shift2 * 2; j++) {
	x_buf[j-5] = x_buf[j];
	y_buf[j-5] = y_buf[j];
	printf("IV. x_buf[%d] = %d, y_buf[%d] = %d\n", j-5, x_buf[j-5], j-5, y_buf[j-5]);
}


//-------grids size-------------------//
float_r = r;
grid = 1.0/(2*float_r);
//printf("\ngrid = %f\n", grid);
}

//------------------main--------------------------//
int main(int argc, char** argv)
{
       wrp = fopen("write_bresenham.csv", "w");
  if (wrp < 0)
    {
        perror("unable to write to file");
        return 1;
    }

mydisplay();
fclose(wrp);
}

