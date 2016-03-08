 /*
    FILE NAME: commonroutines.c
  DESCRIPTION: This code consist of a collection of useful self-made routines
      
      AUTHOR:  Daniel Mejia Raigosa
      E-MAIL:  danielmejia55@gmail.com
      GITHUB:  http://github.com/Daniel-M
        DATE:  17, May 2011
     VERSION:  2.1.1
     
     
    Copyright (C) 2011  Daniel Mejia Raigosa

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAXDATA 10000  //Maximum Data Input
#define CHAPER 20 // Number of allowed characters


//***************************************************************************

// Aks a 'double' value on the range [xmin,xmax] by the message mensaje[200]
double askvalue(char mensaje[200], double xmin, double xmax) 
{
    double x;
    
    do
      {
	printf("%s",mensaje);
	scanf("%lf",&x);
	
	if(x<xmin || x>xmax)
	  {    
	    printf("\n\tError!!!\n\tThe input value is out of the allowed range [%lf,%lf] \n",xmin,xmax);

	  }
      }while (x<xmin || x>xmax);

    return x;
   
}//end ask value

//***************************************************************************

 // Pimp program on the start up
// usage pimpi(program name,some in number)
void pimpi(char titulo[40],int n)
{
    double i;
    printf("\n/");
    for(i=1;i<=n;i=i++)
      {
	printf("-");
      }

    printf(" %s ",titulo);

    for(i=1;i<=n;i=i++)
      {
	printf("-");
      }
    
    printf("\\ \n");

}//end pimpi

//********************************************************************

 // Pimps program on exit, used whit pimpi and as pimpi
void pimpe(char titulo[40],int n) 
{
    double i,t;
    printf("\n\\");
    for(i=1;i<=n;i=i++)
      {
	printf("_");
      }

    t = strlen(titulo);
    t = t + 2;

    for(i=1;i<=n+t;i=i++)
      {
	printf("_");
      }
    
    printf("/ \n");

}//end pimpe


//*************************************************************************


/* This function Test the correct use of the character number */

void characternumber(char *caracter,int CHAPERA)
{
  int longitud;
  longitud=strlen(caracter);
  if (longitud>CHAPERA)
    {
    system("clear");
    printf("===============FATAL ERROR======================="); 
    printf("\n Max input arguments character allowed are %d\n",CHAPERA);
    printf(" The argument %s \n character lenght was %d !!\n",caracter,longitud);
    printf(" Please use less charactares to success.\n Exiting...\n");
    printf("=================================================\n");
    exit(1);
    }
}


//*************************************************************************


int heaviside(double x)
{

  /* this is the heaviside step function
    
          /
          | 1  if x >=0
    H(x)=<
          | 0  if x < 0
   	  \
  */
  if(x>=0)
    {
      return 1;
    }
  else if(x<0)
    {
      return 0;
    }
}


//********************************************************************


void opendatafile(FILE **filepointer,char *filename,char *option)
{
 
/* This Function serves as a file handler wich evitates crashes
  this function is a kind of remplacement of :

  origin=fopen(originfn,"r"); // open data origin 

  if (origin==NULL){   //handle errors
    system("clear");
    printf("===============FATAL ERROR===================="); 
    printf("\n An Error has ocurred during file reading !!\n");
    printf(" The file \'%s\' seems to be Unexistent!!\n Exiting...\n",argv[1]);
    printf("==============================================\n");
    exit(1);
  }

*/

/* usage opendatafile(&filepointer,filename,option["r","w"...])*/

*filepointer=fopen(filename,option); // open file
 if (*filepointer==NULL) //handle reading error 
    {   
    system("clear");
    printf("===============FATAL ERROR===================="); 
    printf("\n An Error has ocurred during file reading !!\n");
    printf(" The file \'%s\' seems to be Unexistent!!\n Exiting...\n",filename);
    printf("==============================================\n");
    exit(1);
  }
}


//***************************************************************************


void datadquire(char *filename,double x[],int *Nmax)
{
  char info[25];
  int N,i,j,cont;
  FILE *filedata;

  opendatafile(&filedata,filename,"r"); //open the file for reading	
  cont=0; //initialize counter
  i=0;
  j=0;
  N=0;
  while(!feof(filedata)) //checks if the end of file is reached
    {  
      N=N+1;
      fscanf(filedata,"%s\n",&info);
      x[i]=atof(info);
      //      printf(" * Reading line %d : x[%d] %.10lf\n",N,i,j+1,x[i]);
      i=i+1;
    }
  *Nmax=N;
  fclose(filedata);
  //  printf(" * Total Readed Lines %d\n",N);
}


//***************************************************************************


void datadquire2d(char *filename,double x[],double y[],int *Nmax)
{
  char info1[25],info2[25];
  int N,i;
  FILE *filedata;

  opendatafile(&filedata,filename,"r"); //open the file for reading	
  
  i=0;//initialize counter
  N=0;

  while(!feof(filedata)) //checks if the end of file is reached
    {  
      N=N+1;
      fscanf(filedata,"%s %s\n",&info1,&info2);
      x[i]=atof(info1);
      y[i]=atof(info2);
      //printf(" * Reading line %d : x[%d]= %.6E \t y[%d]=%.6E\n",N,i,x[i],i,y[i]);
      i=i+1;
    }
  *Nmax=N;
  fclose(filedata);
  //  printf(" * Total Readed Lines %d\n",N);
}

//***************************************************************************

int linesnumber(char *filename)
{
  int N=0;
  char info[25];
  FILE *filedata;

  opendatafile(&filedata,filename,"r"); //open the file for reading	

   while(!feof(filedata)) //checks if the end of file is reached
    {  
      fscanf(filedata,"%s\n",&info);
      N+=1;
      // printf(" > %d\n",N);
    }
  
  fclose(filedata);
  
  return N;
}

void readline(char *filename,int linea,double *x)
{
  char info[25];
  int N,i,j,cont;
  FILE *filedata;
  
  N=linesnumber(filename);
  
  if (linea<=N)
    {
      
      opendatafile(&filedata,filename,"r"); //open the file for reading
      
      for(i=0;i<linea;i++)
	{  
	  fscanf(filedata,"%s\n",&info);
	  *x=atof(info);
	  //      printf(" * Reading line %d : x[%d] %.10lf\n",N,i,j+1,x[i]);
	}
      
      fclose(filedata);
    }
  //  printf(" * Total Readed Lines %d\n",N);
}
