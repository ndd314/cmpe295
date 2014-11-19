/*******************************************************
 * Program: 1p1IncrementDDA.c  Coded by: Harry Li      *
 * Version: x1.0;    status: development;              *
 * Compile and build:                                  *
 * (no need for openGL, no need to use the following)  *
 * gcc -lGL -lGLU -lglut line.c -o main.o              *
 * Date: Sept 23, 2014                                 *
 * Modified: Nov. 15, 2014                             *  
 * Purpose:                                            *
 * Use this program to compute the light source        *
 * array and detector array.                           *
 *******************************************************/
/*******************************************************
 The algorithm description:             
  Given:                                               
  (1) user provided input middle point Pm(xm,ym) on     
      the DDA line (of the light source array).        
  
  Then                            
  (1) From a given middle point Pm(xm,ym), then find    
      the slop of a line linking Pm(xm,ym) to the        
      origin (x0,y0) of the world coordinate system, 
      e.g. x0=y0=0, by the equation:                                
                                                       
     Slop_origin = (y0 - ym) / (x0 - xm)  ...(1)         
                                                      
  (2) Then find the slop of the light source array    
      which is orthogonal to this given slop, by      
      the function module below:                      
                                                       
      void getDir( MY_VDIR_STRUCT *vDir );                           
                                                      
      where MY_VDIR_STRUCT is defined as the struct   
                                                      
  typedef struct                                       
	{
	float dx1;
	float dy1;
	float dx2;
	float dy2;  	
	} MY_VDIR_STRUCT;  

  void getDir( MY_VDIR_STRUCT *vDir );   

       the computed direction vector (orthogoanl to 
       slop_origin is returned as 

       (vDir->x2, vDir->y2)  

       respectively. 

   (3) DDA computation requires direction vector converted to 
       a slop as follows, e.g., 

       Slop_lightArray = vDir->y2 / vDir->x2  ...(2) 


   (4) Now find upper end point and lower end point of the 
       light souce array, by forming the following straight
       line equation, 

       (y_upperEnd - ym) / (y - ym)  =  (x_upperEnd - xm) / (x - xm)  ...(3) 
                                                                        
       or, 

       y_k = Slop_lightArray x_k + offset       ...(4)

       where Slop_lightArray is given from (2), e.g., can be derived 
       similarly from equation (3) as, 

        (y_upperEnd - ym) / (x_upperEnd - xm)   ...(5) 

       and the offset can be derived from equation (3) as 

       offset = ym - xm (y_upperEnd - ym) / (x_upperEnd - xm)  ...(6) 

       From this straight line equation (4), and from the given condition
       of total nunber of points of the light array, NumLightArray, 
       find the upper end point P_upperEnd(x_upperEnd, y_upperEnd), and 
       the lower end point P_lowerEnd(x_lowerEnd, y_lowerEnd), by letter
       
       y_k = Slop_lightArray x_k + offset       ...(4)

       for Slop_lightArray < 1 (assuming Pm(xm, ym) has to be in the II quandrant) 
 
       x_upperEnd = xm + NumLightArray/2 , and via euqation (4), we have 
       y_upperEnd = Slop_lightArray x_upperEnd + offset 

       and 

       x_lowerEnd = xm - NumLightArray/2, and 
       y_lowerEnd = Slop_lightArray x_lowerEnd + offset
       
       if Slop_lightArray > 1, then print error message to ask the user to 
       redefine the light source array location. 

    (5) Now with the given upper end point and lower end point, 

       (x_lowerEnd, y_lowerEnd), (x_upperEnd, y_upperEnd) 

       call DDA alorithm to find the line. 

****************************************************************/ 

#include<stdio.h>
#include <stdlib.h>

#define ARRAY_LEN 1024 //define array length 
#define GRIDARRAY_LEN 512 //define grid square 512X512  

typedef struct 
{
int x_start;             
int y_start;
int x_end;               
int y_end;
int i_accumulator;  
float x[ARRAY_LEN];      
float y[ARRAY_LEN];
} MY_STRUCT;                //for DDA


typedef struct 
{
float dx1;
float dy1;
float dx2;
float dy2;  
} MY_VDIR_STRUCT;           //Direction vector 

void UserMidPtLightArray( int *xm, int *ym, int NumLightArray );   //user input
void getDir( MY_VDIR_STRUCT *vDir );          // find orthogonal direction vector 
void getDDAline( MY_STRUCT * );               // DDA  
 
//------------------main--------------------------// 
int main(int argc, char** argv)
{ 

FILE *fd_wr, *fd_wr1;                    //file descrtor write to disk file

MY_STRUCT      my_struct;  
MY_VDIR_STRUCT my_vdirstruct; 
MY_STRUCT      my_struct1;  
my_struct.i_accumulator = 0;    //initializtion
my_struct1.i_accumulator = 0;  // detector initialization

int x_lowerEnd, y_lowerEnd, x_upperEnd, y_upperEnd;    //for increment DDA
int x_lowerEnd1, y_lowerEnd1, x_upperEnd1, y_upperEnd1;    //for increment DDA source detector
int x0 = 0, y0 = 0, xm, ym;
int NumLightArray = 35; 
float Slop_origin, Slop_lightArray, offset, Slop_detector, offset_detector;                     
int lindex;
char x='x', y='y';               //char to be written back to file as caption 


/*-------------------------------------------------------------------------*
  Step 1. Get input Pm(xm,ym), then compute slop 
 *-------------------------------------------------------------------------*/
	UserMidPtLightArray(&xm, &ym, NumLightArray);
  	//x0 = y0 = 0; xm = 10; ym = 10;  NumLightArray = 11;   //for testing 
	printf("\nYour coordinates for the testing object are (%d, %d)\n", xm, ym);
  	Slop_origin = (y0 - ym) / (x0 - xm) ; 
	printf("Slope with respect to the origin is : %f\n", Slop_origin);
/*---------------------------------------------------------------------*
  Step 2. Then find the slop of the light source array    
      which is orthogonal to this given slop
 *---------------------------------------------------------------------*/          
	my_vdirstruct.dx1 = xm; 
   	my_vdirstruct.dy1 = ym; 
                           
 	getDir( &my_vdirstruct );                           

/*---------------------------------------------------------------------*
  step 3, DDA computation requires direction vector 
      converted to a slop
 *---------------------------------------------------------------------*/
	Slop_lightArray = my_vdirstruct.dy2 / my_vdirstruct.dx2;  
	printf("Orthogonal slope is : %f\n", Slop_lightArray);
	Slop_detector = Slop_lightArray;
	printf("Orthogonal slope_detector is : %f\n", Slop_detector);
/*---------------------------------------------------------------------* 
       step 4, Find upper end point and lower end point  
 *---------------------------------------------------------------------*/ 
        offset = ym - xm * Slop_lightArray; 
	printf("Equation of the Light array is y = %fx + %f\n", Slop_lightArray, offset);

        offset_detector = ym - xm * Slop_detector; 
	printf("Equation of the Light array is y = %fx - %f\n", Slop_detector, offset_detector);
      
       // check to make sure slop < 1
        //if (abs(Slop_lightArray) > 1){
	if(Slop_lightArray > 1) {
             perror("Light array slop is greater than 1, re-enter xm,ym");
           }
        //else if (abs(Slop_lightArray) < 1){ 
	else if(Slop_lightArray < 1) {
          x_upperEnd = xm + NumLightArray/2;  
          y_upperEnd = Slop_lightArray * x_upperEnd + offset;  
          x_lowerEnd = xm - NumLightArray/2;  
          y_lowerEnd = Slop_lightArray * x_lowerEnd + offset; 
	printf("x_upperEnd = %d, y_upperEnd = %d\n", x_upperEnd, y_upperEnd);
	printf("x_lowerEnd = %d, y_lowerEnd = %d\n", x_lowerEnd, y_lowerEnd);	

          x_upperEnd1 = -1*xm + NumLightArray/2;  
          y_upperEnd1 = -1*Slop_detector * -1*x_upperEnd1 - offset_detector;  
          x_lowerEnd1 = -1*xm - NumLightArray/2;  
          y_lowerEnd1 = -1*Slop_detector * -1*x_lowerEnd1 - offset_detector; 
	printf("x_upperEnd1 = %d, y_upperEnd1 = %d\n", x_upperEnd1, y_upperEnd1);
	printf("x_lowerEnd1 = %d, y_lowerEnd1 = %d\n", x_lowerEnd1, y_lowerEnd1);
           }
/*--------------------------------------------------------------------*
       step 5 Now with the given upper end point and lower end point, 
       (x_lowerEnd, y_lowerEnd), (x_upperEnd, y_upperEnd)
       call DDA alorithm to find the line. 
       New starting and ending points for DDA    
       (x_lowerEnd, y_lowerEnd), (x_upperEnd, y_upperEnd)
 ----------------------------------------------------------------------*/ 
   	my_struct.x_start = x_lowerEnd; 
   	my_struct.y_start = y_lowerEnd; 

   	my_struct.x_end = x_upperEnd; 
   	my_struct.y_end = y_upperEnd; 
 
   	my_struct.x[my_struct.i_accumulator] = my_struct.x_start ; //initializtion
   	my_struct.y[my_struct.i_accumulator] = my_struct.y_start ; //initializtion

   	getDDAline( &my_struct );
 
   	my_struct1.x_start = x_lowerEnd1; 
   	my_struct1.y_start = y_lowerEnd1; 

   	my_struct1.x_end = x_upperEnd1; 
   	my_struct1.y_end = y_upperEnd1; 
 
   	my_struct1.x[my_struct1.i_accumulator] = my_struct1.x_start ; //initializtion
   	my_struct1.y[my_struct1.i_accumulator] = my_struct1.y_start ; //initializtion

   	getDDAline( &my_struct1 );

   	fd_wr = fopen("write_incremental_dda.csv", "w+");
   	if (fd_wr < 0)
    	{
        perror("unable to write to file");
        return 1;
    	}

    	fprintf(fd_wr, "%c, %c\n", x, y);
    	for (lindex = 0; lindex <= my_struct.i_accumulator; lindex++)
    	{
         //fprintf(fd_wr, "%.2f, %.2f\n", my_struct.x[lindex], my_struct.y[lindex]);
	fprintf(fd_wr, "%d, %d\n", (int)my_struct.x[lindex], (int)my_struct.y[lindex]);
    	}
 
	fclose(fd_wr);

   	fd_wr1 = fopen("write_incremental_dda1.csv", "w+");
   	if (fd_wr1 < 0)
    	{
        perror("unable to write to file");
        return 1;
    	}

    	fprintf(fd_wr1, "%c, %c\n", x, y);
    	for (lindex = 0; lindex <= my_struct1.i_accumulator; lindex++)
    	{
         //fprintf(fd_wr, "%.2f, %.2f\n", my_struct.x[lindex], my_struct.y[lindex]);
	fprintf(fd_wr1, "%d, %d\n", (int)my_struct1.x[lindex], (int)my_struct1.y[lindex]);
    	}
 
	fclose(fd_wr1);
} //main  
 
//---------------------------------------------------//
//             DDA subroutine                        //
//---------------------------------------------------//
// Find DDA line in x[] and y[]                      //  
//---------------------------------------------------//
//      starting pt (x_start, y_start) as integer    //
//      ending pt   (x_end, y_end)  as integer       //
//      i_accumulator as integer, gives the total    //
//                 number of points of the DDA line. // 
//---------------------------------------------------//  

void getDDAline( MY_STRUCT *xx )   
{
#include <math.h>
#include <stdio.h>

int   i; 
float fslop1, fslop2; 
float fx_buf[ARRAY_LEN];    // intermediate data 
float fy_buf[ARRAY_LEN];    // intermediate data 

//-------compute slop----------------------//

if ((xx->x_end - xx->x_start) == 0) {
  printf( "same starting and ending x, try different x value \n");
  exit; 
}

fslop1 = (xx->y_end - xx->y_start);  
fslop1 = fslop1 / (xx->x_end - xx->x_start);  
//printf("\nfslop1 = %f\n", fslop1);

// for swapping the (x,y) coordinates if x_start>x_end
if((abs(fslop1) <= 1 && (xx->x_start > xx->x_end)) || (abs(fslop1) > 1 && (xx->y_start > xx->y_end))){
  int temp, temp1;
  temp = xx->x_start;
  xx->x_start = xx->x_end;
  xx->x_end = temp;
  temp1 = xx->y_start;
  xx->y_start = xx->y_end;
  xx->y_end = temp1;
}

xx->x[xx->i_accumulator] = xx->x_start;
xx->y[xx->i_accumulator] = xx->y_start;
//printf("xx->x[] = %d\n", xx->x[xx->i_accumulator]);
//printf("xx->y[] = %d\n", xx->y[xx->i_accumulator]);

fx_buf[xx->i_accumulator] = xx->x_start;  // convert to floating point  
fy_buf[xx->i_accumulator] = xx->y_start;  // convert to floating point  
//printf("fx_buf = %f, fy_buf = %f\n", fx_buf[xx->i_accumulator], fy_buf[xx->i_accumulator]);
//printf("After swapping x_start = %d, y_start = %d\n", xx->x_start, xx->y_start);
//printf("After swapping x_end = %d, y_end = %d\n", xx->x_end, xx->y_end);

if (abs(fslop1) > 1)
{fslop2 = 1/fslop1;}

while ( (xx->x[xx->i_accumulator] != xx->x_end) &&   
        (xx->y[xx->i_accumulator] != xx->y_end) ) {  

//printf("outside loop, x = %4d y = %4d\n",xx->x[xx->i_accumulator], xx->y[xx->i_accumulator]);  

if (abs(fslop1) <= 1) {  //absolute value of the slop1 is less than 1 
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] + 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator] + fslop1; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
//printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
//printf("Sub 11: x = %4d y = %4d\n",xx->fx_buf[xx->i_accumulator+1], xx->fy_buf[xx->i_accumulator+1]);
} //end if 

//---------------------------------------
// for slop1 > 1  
// from y = slop1 * x + b 
// we have 
// x = 1/slop1 * y - b/slop1 
// so rewrite x as y
// y = slop2 * x - slop2 * b 
// where
// slop2 = 1/slop1   
//--------------------------------------- 

else if ((fslop1 * fslop1) > 1) {
xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] + 1; 
fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator] + fslop2; 
xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
//printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end else if  

xx->i_accumulator++;
} //end while loop 

} //end getDDAline(.)  

/*----------------------------------------------------------------------*
  Module for orthogonal direction vector
 *----------------------------------------------------------------------*/

void getDir( MY_VDIR_STRUCT *vDir )  //define subroutine prototype 
{
#include <math.h>
#include <stdio.h>

vDir->dx2 = 1.0; 
vDir->dy2 = -1 * (vDir->dx1 / vDir->dy1) ; 

//printf("Sub vDir: vDir->x2 = %4d \n",vDir->dx2 );  
//printf("Sub vDir: vDir->y2 = %4d \n",vDir->dy2 );  

}
//end of the getDir subroutine   


void UserMidPtLightArray( int *xm, int *ym, int NumLightArray )
{
	printf("\nHello User !!\nWelcome to the 3D World !!\n");
   	printf("\nPlease provide the location of your testing object: x, y ->\n");
   	scanf("%d", xm);
   	scanf("%d", ym);
}

