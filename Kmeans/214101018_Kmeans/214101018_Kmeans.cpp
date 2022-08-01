// 214101018_Kmeans.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/* k means algorithm */
/* 
Name-Gaurav Kumar 
Roll-214101018
M-Tech
Computer Science and Engineering 
*/

#include<conio.h>
#include<stdio.h>
// Size of code book 
#define K 8
// size of ceprstral coefficient 
#define P 12
// absolute function 
double mod(double x)
{
	if (x < 0)
		return -x;
	return x;
}

/* calculating tokhera distance for given test and reference c[i] values */
double Tokhera_distance_calculation(double c_t[], double c_r[])
{
    // w[] contain the weights for finding the tokhera distance 
    double w[] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
	// distance variable contain the tokhura diatance 
    double distance = 0, x = 0;
	int i = 0;
	// applying formula 
	for (i = 0; i < P ; i++)
	{
		x = (c_t[i] - c_r[i]);
		distance += w[i] * x * x;
	}
	// return distance 
	return distance;
}
/* Update the centroid by taking the average of a cluster . file name contain the cluster name code_book contain the 
centroid values*/ 
void update_centroid(char file_name[],double code_book[])
{
    // open 
    FILE *f=fopen(file_name,"r");
    int l=0;
    // c[P] read c's values from clluster a 
    double c[P];
    // count is used for counting the number of entry in one cluster 
    int count=0;
    for(l=0;l<P;l++)
    {
        code_book[l]=0;
    }

    while (!feof(f))
    {    
        for(l=0;l<P;l++)
        {
            // reading c's values from cluster file 
            if(l<11){
                fscanf(f, "%lf,", &c[l]);        
            }
            else
            {    
                fscanf(f, "%lf\n",&c[l]);
            }
            /// sum of c's
            code_book[l]+=c[l];               
        }
        
        count++;

    }
    // taking average of centroid
    for(l=0;l<P;l++)
    {
        code_book[l]/=count;
    }
    fclose(f);
}
// read a entry from file for given a pointer 
void read_a_entry(FILE *f,double *code_book)
{
    int l=0;
    for(l=0;l<P;l++)
    {
        if(l<11)
            fscanf(f, "%lf,", &code_book[l]);
        else
            fscanf(f, "%lf\n",&code_book[l]);               
    }
}


int _tmain(int argc, _TCHAR* argv[])
{


	// opening universe.csv file 
    char universe_file[]="Universe.csv";
    printf("Opening Universe.csv....\n");
    FILE *uni_ptr=fopen(universe_file,"r");
    
    double distortion1=2000,distortion2=0;
    // randomly choose 8 code vectors for centroid for implementing k-means algorithm 
    int rand[]={100,1000,2000,3000,3500,4000,4700,5500};
    int i=0,j=0,l=0,index=0;
    
    double code_book[K][P];                 // contain the code_book centroid values 
    double c[P];                               // read inputs from file temporarly
    double distance=0,min_distance=0,delta=0.0005;  
    int count=0;
    char bucket[]="buc1.txt";               // cluster
    i=0;
    j=0;
    double temp[P];
    printf("Genrating Random %d Code vector .......:\n",K);
    /* Initialize random centroid for code book */
    while(!feof(uni_ptr))
    {
        if(i==rand[j])
        {
            read_a_entry(uni_ptr,code_book[j]);   
            j++;
        }
        else
        {
            read_a_entry(uni_ptr,temp);
        }
        i++;
    } 
   /* K-mean algorithm*/
   printf("Start Running K-means Algorithm ...............\n");
   int loop=0;
   // update centroid to minimize the distortion diffrence  
    while(1)
    { 
        // create 8 cluster file for storing c's values of diffrent clusters  
        for(l=0;l<K;l++){
            bucket[3]='0'+l;
            FILE *f1=fopen(bucket,"w");
            fclose(f1);
        }
        fseek(uni_ptr, 0, SEEK_SET);
        distortion2=0;
        count=0;
        // pick a entry form universe and put it in right cluster( means put in that cluster which is at mininmum distance )
            
        while(!feof(uni_ptr))
        {
            // read a entry from universe 
            read_a_entry(uni_ptr,c);
            /* find diatance from diffrent centroid (0 to 8) and compare tokhera distance entry belongs
            to the cluster which have mininmum distance */
            min_distance=Tokhera_distance_calculation(c,code_book[0]);
            index=0;            
            for(j=1;j<K;j++)
            {
                distance=Tokhera_distance_calculation(c,code_book[j]);
                // compare distance 
                if(distance<min_distance)
                {
                    min_distance=distance;
                    index=j;
                }
            }
            // opening the cluster 
            bucket[3]='0'+index;
            FILE *f=fopen(bucket,"a");
            // store entry in cluster 
            for(l=0;l<P;l++)
            {
                if(l<11)
                    fprintf(f, "%lf,", c[l]);
                else
                    fprintf(f, "%lf\n",c[l]);               
            }
            distortion2+=min_distance;
            count++;
            fclose(f);
        }    
        // average the total distorion 
        distortion2=distortion2/count;
        // update the centroid of clusters (cluster0,cluster1......cluster8)
        for(l=0;l<K;l++)
        {
            bucket[3]='0'+l;
            update_centroid(bucket,code_book[l]);
        }
        // store centroid in file centroid.txt
        FILE *f2=fopen("centroid.txt","w");
        for(i=0;i<K;i++)
        {
            for(l=0;l<P;l++)
            {
                if(l<11)
                    fprintf(f2, "%lf,", code_book[i][l]);
                else
                    fprintf(f2, "%lf\n",code_book[i][l]);               
            }
        }
        if(loop!=0)
            printf("loop %d distortion => previous :%lf Current :%lf Difference :%lf\n",loop,distortion1,distortion2,mod(distortion2-distortion1));
        loop++;

        // if current distorition - previous distortion is less then delta then loop break otherwise loop continue 
        // after every iteration distortion decreases  
        if(mod(distortion2-distortion1)<delta)
        {
            break;
        } 
        distortion1=distortion2;
        fclose(f2);   
    }
    // close universe.csv file 
    fclose(uni_ptr);
    printf("Program Ended\n");


	getch();



	return 0;
}

