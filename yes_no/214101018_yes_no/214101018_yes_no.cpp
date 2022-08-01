// sp1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include<conio.h>
#include<stdlib.h>
long int mod(long int x)
{
	if(x<0)
		return -x;
	return x;
}


// Calculating DC shift from the file 
int dc_shift(char filename[])
{
	FILE *f=fopen(filename,"r");
	int sum=0;
	int count=0;
	int x=0;
	char data[100];
	int i=0;
	//skip the sound text data
	for(i=0;i<5;i++)
	{
		fgets(data,100,f);
	}

	while(!feof(f))
	{
		fscanf(f,"%d\n",&x);
		sum+=x;
		count++;
	}
	fclose(f);
	if(sum==0)
	{
		return 0;
	}
	else
	{
		return sum/count;

	}
	fclose(f);
}


// Normalization of wave-form
// filename is original sound-wave file  and filedest is modified wave-form file
void normalization(char filename[],char filedest[])
{
	FILE *f=fopen(filename,"r");
	long int i=0;
	long int max=1;
	long int x=0;
	//finding the modulus of maximun amplitude  
	while(!feof(f))
	{
		fscanf(f,"%ld\n",&x);
		if(mod(x)>max)
			max=mod(x);
	}
	//printf("maximum Amplitude is : %d \n",max);
	double range=5000;
	double normalize_factor=(range/max);		//normalize factor
	fseek(f,0,SEEK_SET);
	char data[100];
	for(i=0;i<5;i++)
	{
		fgets(data,100,f);
	}
	long int y=0;
	
	FILE *f1=fopen(filedest,"w");
	// normalizing wafeform by multiplying with normalization factor
	while(!feof(f))
	{
		fscanf(f,"%ld\n",&x);
		y=x*normalize_factor;
		fprintf(f1,"%ld\n",y);
	}
	fclose(f);
	fclose(f1);
}




int _tmain(int argc, _TCHAR* argv[])
{
	char file_name[]="yes_no.txt";								// sound file name 
	int window=240;												// window(chunk) size 
	int dc=0;													// variable store the dc-shift value									
	char name[]="DC.txt";
	char dcname[]="dc_file.txt";
	
	dc=dc_shift(name);											// dc_shift retun the dc shift value
	FILE *f,*f1;
	char data[100];
	long int x=0,i=0;
	
	printf("File name %s\n",file_name);

	// DC shift and store in dc_file.txt after DC shift
	if(dc>0)
	{
		f=fopen(file_name,"r");
		f1=fopen(dcname,"w");
		for(i=0;i<5;i++)
		{
			fgets(data,100,f);						//print file details 
			printf("%s",data);
		}
		while(!feof(f))
		{
			fscanf(f,"%ld\n",&x);
			if(x>0)
				x=x-dc;
			if(x<0)
				x=x+dc;
			fprintf(f1,"%ld\n",x);
		}
	
	}
	else
	{
		
		f=fopen(file_name,"r");
		f1=fopen(dcname,"w");

		for(i=0;i<5;i++)
		{
			fgets(data,100,f);
			printf("%s",data);
		}
		while(!feof(f))
		{
			fscanf(f,"%ld\n",&x);
			fprintf(f1,"%ld\n",x);
		}
	}
	fclose(f);
	fclose(f1);

	char normname[]="norm_file.txt";					// Normalized file name 
	normalization(dcname,normname);						//Normalization function called 
	
	FILE *fp=fopen(normname,"r");
	if(fp==NULL)
	{
		printf("Error");
		return -1;
	}

	long int x1=0,x2=0;


	//skip starting 1000 chunck so that starting error will remove 
	for(i=1;i<=10*window;i++)
	{
		fscanf(fp,"%ld\n",&x1);
	}	

	

	//calculate zcr and energy per window 
	FILE *f3;
	char en_file[]="enzcr.txt";						// enzcr.txt file store the energy and zcr value of all windows 
	f3=fopen(en_file,"w");
	int zcrcount=0;									// count the zcr value of current window
	long int en=0;									// store the energy of current window
	
	if(f3==NULL)
	{
		printf("error");
		return -1;
	}
	long int count=1;							// variable count use to count the number of time loop iterate 
	
	while(!feof(fp))
	{
		// reading data from normalized file 
		fscanf(fp,"%ld\n",&x2);
		
		
		/*ZCR count per window x1 store the previous sample amplitude and x2 store 
		the current sample amplitude if x1 and x2 have opposite sign then zcr increase by 1*/

		if(x1<=0 && x2>0)			
		{
			zcrcount++;
		}
		else if(x1>=0 && x2<0)
		{
			zcrcount++;
		}
		x1=x2;
		/* energy calculation */
		en+=x2*x2;
		if(count%window==0)							//if window completed then zcr and energy will stiore in enzcr file 
		{	
			en=en/window;							// avreage the energy of window 
			fprintf(f3,"%ld %d\n",en,zcrcount);		// store enrgy and zcr in file enzcr.txt
			zcrcount=0;								// window is completed so zcrcount is 0 and enrgy is 0 so we can calculate en and zcr for next window
			en=0;
		}
		count++;
	}
	fclose(f3);
	fclose(fp);
							
	fp=fopen(en_file,"r");							// enzcr.txt file opening foe calculating the ambient noise 
	if(fp==NULL)
	{
		printf("Error");
		getch();
		return -1;
	}
	
	//calculate the silence sound enery schunck means how many window we take 
	int schunk=8;
	long int *senergy=(long int *)malloc(schunk*sizeof(long int));
	int *szcr=(int *)malloc(schunk*sizeof(int));
	for(i=0;i<schunk;i++)
	{
		fscanf(fp,"%ld %d\n",&senergy[i],&szcr[i]);
	}	
	//average the silence energy
	long int threshold=10*schunk;
	for(i=0;i<schunk;i++)
	{
		threshold+=senergy[i];
	}
	// decide threshold energy
	threshold=(threshold*10)/schunk;
	//printf("threshold -> %ld\n",threshold);
	
	fclose(fp);
	fp=fopen(en_file,"r");
	if(fp==NULL)
	{
		printf("Error");
		getch();
		return -1;
	}
	
	
	long int *energy=(long int *)malloc(100*sizeof(long int));
	int *zcr=(int *)malloc(100*sizeof(int));
	int size=0,m=0;
	while(!feof(fp))
	{
		// reading energy and zcr fro enzcr.txt 
		fscanf(fp,"%ld %d\n",&energy[size],&zcr[size]);
		m=zcr[size];
		if(size>0 && size%100==0)
		{
			// increasing size of array when array is full
			zcr=(int*)realloc(zcr,(size+100)*sizeof(int));
			energy=(long int*)realloc(energy,(size+100)*sizeof(long int));
		}
		zcr[size]=m;
		size++;
	}
	/** Finding start and end of sounds by analysing energy ********/
	int *start,*end;
	count=0;
	
	start=(int *)malloc(10*sizeof(int));										// store start index of sounds sample
	end=(int *)malloc(10*sizeof(int));											// store end index of sounds sample
	for(i=0;i<10;i++)
	{
		start[i]=-1;
		end[i]=-1;
	}
	for(i=1;i<size;i++)
	{
		if(energy[i]>threshold && energy[i-1]>threshold && energy[i+1]>threshold && start[count]==-1)		// if energy of 3 window is greater then theshold then sound starts  
		{
			start[count]=i-1;
			if(count>0)
			{
				if(start[count]-end[count-1]<5){									// if start of new sound and end of previous sound index is less then 5 then it merge with previous sound
					start[count]=-1;
					count--;
				}
			}
		}
		if(energy[i]<threshold && energy[i-1]<threshold && energy[i+1]<threshold && start[count]>-1)		// if energy of 3 window is less then theshold then sound ended 
		{
			end[count]=i;
			count++;
		}
	}

	printf("\nNo of Sample Sound found :%d\n",count); 
	//for(i=0;i<count;i++)
	//	printf("Sound [%d]-> start [%d], end [%d]\n",i+1,start[i],end[i]);
	
	long int p1=0,p2=0,zcr1=0,zcr2=0,j=0;
	for(i=0;i<count;i++)
	{
		zcr1=0;													// zcr1 store average of 70% zcr from start
		zcr2=0;													// zcr2 store average of 30% zcr from end
		p1=(end[i]-start[i])*70/100;									
		p2=(end[i]-start[i]);
		for(j=0;j<p1;j++)
		{
			zcr1+=zcr[start[i]+j];
		}
		for(j=p1+1;j<p2;j++)
		{
			zcr2+=zcr[start[i]+j];
		}
		zcr1=zcr1/p1;
		zcr2=zcr2/(p2-p1);
		//printf("zcr1 =%ld zcr2=%ld   ",zcr1,zcr2);
		if(zcr2>zcr1)
		{
			printf("Sound number %d is Yes\n",i+1);
		}
		else
		{
			printf("Sound number %d is No\n",i+1);
		}
	
	}
	printf("programme Ended\n\n");


	
	free(szcr);
	free(senergy);
	free(zcr);
	free(energy);
	free(start);
	free(end);
	getch();
	return 0;


}
