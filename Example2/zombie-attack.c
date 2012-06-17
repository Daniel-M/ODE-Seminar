/*
    FILE NAME: muskin-ca-mg-parv.c
  DESCRIPTION: This code uses GSL to calculate the solution to a zombie attack model - see "Integracion-Numerica-ODE.pdf"
	       In order to understand the model (Look on the Github link)

      
       AUTHOR: Daniel Mejia Raigosa
       E-MAIL: danielmejia55@gmail.com
       GITHUB: https://github.com/Daniel-M/ODE-Seminar
         DATE: 27 Sept 2011
      VERSION: 1.1
      
    This file is part of "ODE-Seminar".

    "ODE-Seminar" is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    "ODE-Seminar" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "commonroutines.c"

#include<gsl/gsl_odeiv2.h>
#include<gsl/gsl_errno.h>


int reactions(double t, const double y[], double f[], void *params){
  
  /***************************************************************/
  /* Here one defines the system of ODEs dy[i]/dt = f[i] 
   * where y[i] stands for the i-th variable and f[i] its function
   * of some of the y[i] variables.
   * The parameters are passed as an array called para (might be other name)
   * The variables are difined as
   * y[0]=zombies
   * y[1]=sanos no imnume
   * y[2]=inmune
  /***************************************************************/
  
  
  double *para=(double*)params;;
  
  f[0]=(para[0]*y[1]-para[2])*y[0]-para[1]*(y[1]+y[2]);
  f[1]=(-para[0]*y[0]-para[4])*y[1]+para[3]*y[1];
  f[2]=-(para[5]*y[0]+para[7])*y[2]+para[6]*(y[1]+y[2]);
  
  
  return GSL_SUCCESS;
  /* ALWAYS Must Return GSL_SUCCESS ! */
}

int main(int argc, char **argv){
  

  /* Here on ask for initial values on each concentration IN THIS STAGE ON DEFINES INITIAL CONDITIONS
     Note that we have 5 conditions, setting 0.0 for the initial complex concentration */
  
  double y[3]={1,40,4};

  
  double p1=askvalue("a1?",-1e6,1e6),p2=askvalue("b1?",-1e6,1e6),p3=askvalue("d1?",-1e6,1e6),p4=askvalue("b2?",-1e6,1e6);
  double p5=askvalue("d2?",-1e6,1e6),p6=askvalue("a2?",-1e6,1e6),p7=askvalue("b3?",-1e6,1e6),p8=askvalue("d36?",-1e6,1e6);  // Rate constans, one can use an "askvalue" for demand them to the user
  
  /* Here one defines the parameters as an array of 4 elements
     this array is passed to the routine of solution */
  
  double parametros[8]={p1,p2,p3,p4,p5,p6,p7,p8}; 
  
  double t=0.0,tf;
  double yerr[3];
  double stepp;
  
  //FILE *resultados; // Deleted only one file results
  //opendatafile(&resultados,"resultados.txt","w");
  
  /* added file separation for each complex */
  
  /* here one creates the pointer to write to a file */
  
  FILE *filedataca;
  FILE *filedatamg;
  FILE *filedatacaparv;

  
  /* using opendatafile routine in "commonroutines.c" as a file handler */
  
  
  /* Opening files to write and putting some information on each */
  
  opendatafile(&filedataca,"results-zombie.txt","w"); //open the file for write
  fprintf(filedataca,"### Time\tZombie\n");
  
  opendatafile(&filedatamg,"results-sano.txt","w"); //open the file for write
  fprintf(filedatamg,"### Time\tSano\n");
  
  opendatafile(&filedatacaparv,"results-inmune.txt","w"); //open the file for write
  fprintf(filedatacaparv,"### Time\tInmune\n");
  
    
  fprintf(filedataca,"%E %E\n",t,y[0]);
  fprintf(filedatamg,"%E %E\n",t,y[1]);
  fprintf(filedatacaparv,"%E %E\n",t,y[2]);

  
  /***************************************************************/
  /* Now one defines the stuff that GSL Needs to solve the system */
  /***************************************************************/
  
  /* Here one creates the step method one can change where says "gsl_odeiv2_step_rk8pd" by
     
     * gsl_odeiv2_step_rk2  - Runge Kutta (2,3) Method
     * gsl_odeiv2_step_rk4  - 4th Order Runge Kutta Method (The Classical)
     * gsl_odeiv2_step_rkf45 - Runge-Kutta-Fehlberg (4,5) Method (General Purpose, GSL Recommended)
     * gsl_odeiv2_step_rkck - Runge- Kutta Cash-Karp (4,5) Method
     * gsl_odeiv2_step_rk8pd - Runge- Kutta Prince-Dormand (8,9) Method (defined here by default) */
     
  const gsl_odeiv2_step_type *Type=gsl_odeiv2_step_rkf45;
  
  
  /* Here one says to GSL that must create memory for 5 ODES (note the five) 
   * this must be changed when one changes the number of equations */

  gsl_odeiv2_step *Step=gsl_odeiv2_step_alloc(Type,3);
  
 /* Here one defines the system of ODEs as Sys and then calls the created system "reactions"
  * The NULL it's because we haven't defined a jacobian matrix, one uses the jacobian when
    the method demands but for the methods listed above one don't needs to define a jacobian
    so one puts NULL
  * then one puts 5 cuz we have 5 ODEs to integrate
  * the on puts &parameters on makes a "reference call" to the defined array of parameters to pass to the system
    remember that in "reactions" one uses them*/
 
  gsl_odeiv2_system Sys={reactions,NULL,3,&parametros};
  
  
  
  /*askvalue returns double, usage "askvalue(char mensaje[200], double xmin, double xmax)"*/
  stepp=askvalue("Paso de Integración",0,1);
  tf=askvalue("Tiempo de Integración",1e-6,1e6);
  
  printf("\n");
  
  while(t<tf){
  
     /* The function "gsl_odeiv2_step_apply(Step,t,stepp,y,yerr,dy/dt_in,dy/dt_out,&Sys)"
       applies the stepping function Step to the system of equations deﬁned by Sys,
       using the step-size stepp to advance the system from time t and state y to time t+h.
       The new state of the system is stored in y on output, with an estimate of the absolute 
       error in each component stored in yerr. If the argument dydt_in is not null it should
       point an array containing the derivatives for the system at time t on input. This is
       optional as the derivatives will be computed internally if they are not provided, but
       allows the reuse of existing derivative information. On output the new derivatives of
       the system at time t+h will be stored in dydt_out if it is not null.
       
       * Note that both dy/dt_in and dy/dt_out are defined as null below*/
    
    
    
    int status=gsl_odeiv2_step_apply(Step,t,stepp,y,yerr,NULL,NULL,&Sys);
    

    
    //fprintf(resultados,"\n%.6E %.6E %.6E %.6E %.6E %.6E",t,y[0],y[1],y[2],y[3],y[4]); // Deleted, used in one file results
    
    
    fprintf(filedataca,"%E %E\n",t,y[0]);
    fprintf(filedatamg,"%E %E\n",t,y[1]);
    fprintf(filedatacaparv,"%E %E\n",t,y[2]);
  
    
    printf("%.6E %.6E %.6E %.6E\n",t,y[0],y[1],y[2]);
    
    t++;
    
  }
  
  fclose(filedataca);
  fclose(filedatamg);
  fclose(filedatacaparv);

   
  
  /* If GNU Plot it's intalled uncomment the lines below to create plots of the results on images */
   
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'kinetic-all.png';plot 'results-zombie.txt' w lp,'results-sano.txt' w lp,'results-inmune.txt' w lp\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'kinetic-ca.png';plot 'results-zombie.txt' w lp\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'kinetic-mg.png';plot 'results-sano.txt' w lp\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'kinetic-parv.png';plot 'results-inmune.txt' w lp\"");
 
  
  exit(0);
} //End of Main Code