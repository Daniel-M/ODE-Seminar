/*
    FILE NAME: euler-rk0.c
  DESCRIPTION: This Code aims to show the use of Euler Method to integrate dy/dt=g*sin(t)
	      
       AUTHOR: Daniel Mejia Raigosa
         DATE: 13 Sept 2011
      VERSION: 0.1
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "commonroutines.c"


double fth(double g,double th)
{
  double res;
  
  res=g*sin(th);
  
  return res;
}


void rk0_method(double h, double t0, double yi,double fty,double *tf,double *yi1)
{
  double t;
  
     
  *tf=t0+h;
  *yi1=yi+h*fty;
    
}


/*main(int argc,char *argv[]) // for input arguments, remeber argv[0]=program name argv[1]= first argument and so on */
int main(void)
{


  double t,t0,th0,th,tf,h,y2;
  
  FILE *salida;
  FILE *exacto;
  
  opendatafile(&salida,"example1.txt","w");
  opendatafile(&exacto,"exacto1.txt","w");
     
  system("clear");
  
  printf("\nIngrese t_f;\n");
  scanf("%lf",&tf);
  
  
  printf("\nIngrese th0:\n");
  scanf("%lf",&th0);

  printf("\nIngrese h:\n");
  scanf("%lf",&h);
  
  
  t=t0;
  th=th0;
  fprintf(salida,"%E %E\n",t,th);
    
  do{
    
    rk0_method(h,t0,th,fth(9.8,th),&t,&y2);
    
    th=y2;
    t0=t;
    
    fprintf(salida,"%E %E\n",t,th);
    
  }while(t<tf);

  
  t=-1;
  th=th0;
  fprintf(exacto,"%E %E\n",t,th);  
  do{
    
    th=acos((1+exp(19.6*t))/(1-exp(19.6*t)));
    fprintf(exacto,"%E %E\n",t,th);
    t=t+1;  
    
  }while(t<tf);
  
  exit(0);
  

} //End of Main Code
