// 214101018_LBG.cpp : Defines the entry point for the console application.
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
// cepstral coefficient size
#define P 12
int K=0;
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


int _tmain(int argc, _TCHAR* argv[])
{

	  // Opening universe.csv
    char universe_file[]="Universe.csv";
    printf("Opening Universe file......\n");
    FILE *uni_ptr=fopen(universe_file,"r");

    double distortion1=2000,distortion2=0;
    int i=0,j=0,l=0,index=0;
    
    double code_book[8][P];                         // contain code block centroid 
    double c[P];                                    // temporary entry reading 
    double distance=0,min_distance=0,delta=0.0005;
    int count=0;
    char bucket[]="buc1.txt";
    i=0;
    j=0;
    double temp;
   int loop=0;
   // LBG algorithm 
   /* start with k=1 after optimization size of K increase by  2 times */
   /*k=1 then k=2 then k=4 then k=8*/
    while(K+1<=8)  
    {
        distortion1=2000;
        if(K==0)
        {
            K=1;
            printf("Finding Centroid of Universe for...K=%d\n",K);
            // find centroid of file and initialize code book 
            update_centroid(universe_file,code_book[0]);
            // Now K=1
            
        }
        else
        {
            // split code book in two part means increase size of code book by 2 times 
            for(l=0;l<K;l++)
            {
                for(i=0;i<P;i++)
                {
                    // one entry of code book is centroid+0.03 and other is centroid-0.03
                    code_book[K+l][i]=code_book[l][i]-0.03;
                    code_book[l][i]=code_book[l][i]+0.03;
                }
            }

            K=K*2;
            printf("Code book Size increased ..K=%d\n",K);
        }
        loop=0;
        // Update centroid by applying K-means algorihtm 
        while(1)
        {
            // create cluster file 
            for(l=0;l<K;l++)
            {
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
                for(l=0;l<P;l++)
                {
                    if(l<11)
                        fscanf(uni_ptr, "%lf,", &c[l]);
                    else
                        fscanf(uni_ptr, "%lf\n",&c[l]);               
                }
                /* find distance from each cluster centroid and find minimum */
                min_distance=Tokhera_distance_calculation(c,code_book[0]);
                index=0;
                for(j=1;j<K;j++){   
                    distance=Tokhera_distance_calculation(c,code_book[j]);
                    if(distance<min_distance)
                    {
                        min_distance=distance;
                        index=j;
                    }
                }
                // put entry in cluster 
                bucket[3]='0'+index;
                FILE *f=fopen(bucket,"a");
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
            // average the total distortion  
            distortion2=distortion2/count;
            // update centroid of each clusters 
            for(l=0;l<K;l++)
            {
                char file[]="buc0.txt";
                file[3]='0'+l;
                update_centroid(file,code_book[l]);
            }
            //printf("loop %d ",loop++);
            //store cluster centroid in centroid.txt
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
                printf("loop %d distortion => previous :%lf Current :%lf diffrence :%lf\n",loop,distortion1,distortion2,mod(distortion2-distortion1));
            loop++;
            // if current distorition - previous distortion is less then delta then loop break otherwise loop continue 
             // after every iteration distortion decreases  
        
            if(mod(distortion2-distortion1)<delta)
            {
                break;
            } 
            distortion1=distortion2;
            fclose(f2);
            if(K==1)
                break; 
        }
    }
    // close universe file 
    fclose(uni_ptr);
	printf("Program ended ...");
	getch();
	return 0;
}

