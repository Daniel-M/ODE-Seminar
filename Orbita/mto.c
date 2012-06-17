/*
    FILE NAME: rk4-test2.c
  DESCRIPTION: This Code Solves a Classical Mechanics exercise where one particle's constrained to move
	       On a surface given by z = lambda  Ln(sqrt(x**+y**))
	       The resuling ecuations are
	       
	       z'=pz/m
	       th'=l/m r**
	       r''=l^2/m^2 r^4 + lambda g/r
	      
	      see "Integracion-Numerica-ODE.pdf (Look on the Github link)
	      
       AUTHOR: Daniel Mejia Raigosa
       E-MAIL: danielmejia55@gmail.com
       GITHUB: https://github.com/Daniel-M/ODE-Seminar
         DATE: 29 Sept 2011
      VERSION: 2.1
      
     COMMENTS: * THIS CODE COMES WHIT NO WARRANTIES OF ANY KIND
               * In order to generate the images, you must have GNU Plot installed and working
	       * In the case that you haven't GNU Plot, please comment the lines at the end that make use of it
	
	
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
#include <time.h>
#include "commonroutines.c"


double fz(double m,double pz)
{
  double res;
  
  res=pz/m;
  
  return res;
}


double fth(double m,double pth,double r)
{
  double res;
  
  res=pth/(m*pow(r,2));
  
  return res;
}


double frdot(double u)
{
  double res;
  
  res=u;
 
  return res;
}

double fr2dot(double m,double pz,double pth,double lambda,double g,double r)
{
  double res;
  
  res=pow(pth/m,2)*(1/pow(r,3))+(g*lambda)/r;
 
  return res;
}


void rk4_method(double lambda,double g,double m,double pz,double pth,double x0,double y0,double tf,double h)
{
  double r1[1],r2[1],th[1],z[1];
  double krd1,krd2,krd3,krd4;
  double krdd1,krdd2,krdd3,krdd4;
  double kth1,kth2,kth3,kth4;
  double kz1,kz2,kz3,kz4;
  
  double t,N,cont;
  
  FILE *filedata;
  FILE *filedatap;
  FILE *filedatazx;
  FILE *filedatazy;


  
        
  opendatafile(&filedata,"orbital.txt","w"); //open the file for write
  fprintf(filedata,"### Time\trho\ttheta\tz\n");
  
  opendatafile(&filedatap,"orbitalp.txt","w"); //open the file for write
  opendatafile(&filedatazx,"orbitalzx.txt","w"); //open the file for write
  opendatafile(&filedatazy,"orbitalzy.txt","w"); //open the file for write
      
  r1[0]=sqrt(pow(x0,2)+pow(y0,2));
  r2[0]=0;  // Velocidad inicial cero
  th[0]=atan(y0/x0);
  z[0]=log(r1[0]);
    
  t=0;
  
  fprintf(filedata,"%E %E %E\n",r1[0]*cos(th[0]),r1[0]*sin(th[0]),z[0]);
  fprintf(filedatap,"%E %E\n",r1[0]*cos(th[0]),r1[0]*sin(th[0]));
  fprintf(filedatazx,"%E %E\n",r1[0]*cos(th[0]),z[0]);
  fprintf(filedatazy,"%E %E\n",r1[0]*sin(th[0]),z[0]);
  
  
  do{
    
    krd1 = frdot(r2[0]);
    krdd1=fr2dot(m,pz,pth,lambda,g,r1[0]);
    kth1 = fth(m,pth,r1[0]);
    kz1 = fz(m,pz);
 
    krd2 = frdot(r2[0]+0.5*h*krd1);
    krdd1=fr2dot(m,pz,pth,lambda,g,r1[0]+0.5*h*krdd1);
    kth1 = fth(m,pth,r1[0]+0.5*h*krdd1);
    kz2 = fz(m,pz);
    
    krd3 = frdot(r2[0]+0.5*h*krd2);
    krdd1=fr2dot(m,pz,pth,lambda,g,r1[0]+0.5*h*krdd2);
    kth1 = fth(m,pth,r1[0]+0.5*h*krdd2);
    kz3 = fz(m,pz);
    
    krd4 = frdot(r2[0]+h*krd3);
    krdd1=fr2dot(m,pz,pth,lambda,g,r1[0]+h*krdd3);
    kth1 = fth(m,pth,r1[0]+h*krdd3);
    kz4 = fz(m,pz);
     
    r1[1] = r1[0]+h*(krd1/6.0+krd2/3.0+krd3/3.0+krd4/6.0);
    r2[1] = r2[0]+h*(krdd1/6.0+krdd2/3.0+krdd3/3.0+krdd4/6.0);
    th[1] = th[0]+h*(kth1/6.0+kth2/3.0+kth3/3.0+kth4/6.0);
    z[1] = r2[0]+h*(kz1/6.0+kz2/3.0+kz3/3.0+kz4/6.0);
    
   
    t = t+h;
          
    fprintf(filedata,"%E %E %E\n",r1[1]*cos(th[1]),r1[1]*sin(th[1]),z[1]);
    fprintf(filedatap,"%E %E\n",r1[1]*cos(th[1]),r1[1]*sin(th[1]));
    fprintf(filedatazx,"%E %E\n",r1[1]*cos(th[1]),z[1]);
    fprintf(filedatazy,"%E %E\n",r1[1]*sin(th[1]),z[1]);
    
    r1[0]=r1[1];
    r2[0]=r2[1];
    th[0]=th[1];
    z[0]=z[1];
    
    
  }while(t<tf);
  
  fclose(filedata);
  
}


int main(void)
{

  double x0,y0,lambda,g=9.8,tf,h;
  double m,pz,pth;
    
  system("clear");
  
  printf("\n\n\tHola esto es una prueba de calculos mto\n\n");
  m=askvalue("Ingresa la masa:\n",1e-4,1e4);
  printf("Ingresa las condiciones iniciales x0 y0:\n\n");
  scanf("%lf %lf",&x0,&y0);
  printf("Ingresa las condiciones iniciales pz pth:\n\n");
  scanf("%lf %lf",&pz,&pth);
  printf("lambda de la superficie?:\n");
  scanf("%lf",&lambda);
  
  printf("Tiempo de integracion tf:\n");
  scanf("%lf",&tf);
  printf("Condicion h step:\n");
  scanf("%lf",&h);
  printf("\n [!] Calculando...\n");
  
  // rk4_method(double lambda,double g,double m,double pz,double pth,double x0,double y0,double tf,double h)
  rk4_method(lambda,g,m,pz,pth,x0,y0,tf,h);
  
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'orbita.png';splot 'orbital.txt' w l\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'orbitap.png';plot 'orbitalp.txt' w l\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'orbitazx.png';plot 'orbitalzx.txt' w l\"");
  system("gnuplot -p -e \"set terminal png size 1640,1480;set output 'orbitazy.png';plot 'orbitalzy.txt' w l\"");
    
  exit(0);

} //End of Main Code
