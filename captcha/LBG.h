// 214101018.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#define N_W 320
#define P 12
#define Q 12
#define FRAME 50
#define SKIP 500
#define M_PI 3.1415926
int K=0;
long int f_seek=0;
int peak=0;
int sliding_size=65;


// find modulus of amplitude 
long double mod(long double x)
{
	if (x < 0)
		return -x;
	return x;
}


long int energy_file(char file[])
{


    FILE *f=fopen(file,"r");
    if(f==NULL)
    {
        printf("file Error");
    }
    char data[100];
	for (int i = 0; i < 5; i++)
	{
		fgets(data, 100, f);
	}
	
    
    int count=0,markar=0,x;
    long double en=0,max=0;
    while(!feof(f))
    {
     //   printf("a");
        fscanf(f,"%d\n",&x);
        en+=x*x;
        count++;
        if(count%N_W==0)
        {
            if(max<en)
            {
                max=en;
                markar=count/N_W;
            }
            en=0;
        }
    }
    fclose(f);
    return markar;


}



// skip window find the starting of sound it skips silent part
void skip_window(FILE *fp)
{
	/* if in a 100 consecutive amplitude if at least 30 amplitude have 
	mod(amplitude)>500 means sound started */
	float x;
	int i = 0, j = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%f\n", &x);
		i = 0;
		if (mod(x) >= 100.0)
		{
			for (j = 0; j < 100; j++)
			{

				fscanf(fp, "%f\n", &x);
				if (mod(x) >= 100.0)
					i++;
				if (i > 30)
					return;
			}
		}
	}
}
 
// Normalization of wave-form
// filename is original sound-wave file  and filedest is modified wave-form file
void Normalization(char input_file[], char output_file[])
{
	int total_required_window=((T-1)*sliding_size*N_W/100+N_W)/N_W+1;
	FILE *f_input = fopen(input_file, "r");
	if(f_input==NULL)
	{
		printf("Input file not opening\n");
		return;
	}
    long int high_en_index=energy_file(input_file);
    
	long int i = 0;
	long double max = -1;
	long double x = 0;
	char data[100];
	// skip 5 lines 
	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
	//finding the modulus of maximun amplitude
	int total_window=0;
	while (!feof(f_input))
	{
		fscanf(f_input, "%Lf\n", &x);
		if (mod(x) > max)
			max = mod(x);
		total_window++;
	}
	total_window/=N_W;
	float range = 5000;		
	float normalize_factor = (range / max); //normalize factor
	//normalize_factor = 1;
	fseek(f_input, 0, SEEK_SET);

	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
    long int count_x=0;
	while(high_en_index+(total_required_window/2+1)>total_window)
	{
		high_en_index-=5;
	}
	if(high_en_index-(total_required_window/2)<0)
	{
		high_en_index=45;
	}
// 	skip_window(f_input);
    while(count_x < high_en_index-(total_required_window/2))
    {
        for(i=0;i<N_W;i++)
        {
            fscanf(f_input,"%d\n",&x);
        }
        count_x++;
    }
	long double y = 0;

	// normalizing wafeform by multiplying with normalization factor
	int j=0,t=0;
    int count1=1;
	FILE *f_output = fopen(output_file, "w");
	if(f_output==NULL)
	{
		printf("Normalized file creation fail \n");
		return;
	}




	while(!feof(f_input) && count_x<high_en_index+total_required_window/2+1)
	{
		fscanf(f_input, "%Lf\n", &x);
		y = x * normalize_factor;
		fprintf(f_output, "%Lf\n", y);
        count1++;
        if(count1%N_W==0)
        {
            count_x++;
        }
	}
	printf("start window %d\tfinish %d = %d\n",high_en_index-total_required_window/2,high_en_index+total_required_window/2+1,(high_en_index-total_required_window/2)-(high_en_index+total_required_window/2+1));
	
	//printf("total window %d\nCount %d\n",total_required_window,count_x);
	fclose(f_input);
	fclose(f_output);
}

/*
// Normalization of wave-form
// filename is original sound-wave file  and filedest is modified wave-form file
void Normalization(char input_file[], char output_file[])
{
	FILE *f_input = fopen(input_file, "r");
	if(f_input==NULL)
	{
		printf("Input file not opening\n");
		return;
	}
    long int high_en_index=energy_file(input_file);
    
	long int i = 0;
	long double max = -1;
	long double x = 0;
	char data[100];
	// skip 5 lines 
	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
	//finding the modulus of maximun amplitude
	int total_window=0;
	while (!feof(f_input))
	{
		fscanf(f_input, "%Lf\n", &x);
		if (mod(x) > max)
			max = mod(x);
		total_window++;
	}
	total_window/=N_W;
	float range = 5000;		
	float normalize_factor = (range / max); //normalize factor
	//normalize_factor = 1;
	fseek(f_input, 0, SEEK_SET);

	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
    long int count_x=0;
	while(high_en_index+43>total_window)
	{
		high_en_index-=5;
	}
	while(high_en_index-42<0)
	{
		high_en_index+=5;
	}
// 	skip_window(f_input);
    while(count_x < high_en_index-42)
    {
        for(i=0;i<N_W;i++)
        {
            fscanf(f_input,"%d\n",&x);
        }
        count_x++;
    }
	long double y = 0;

	// normalizing wafeform by multiplying with normalization factor
	int j=0,t=0;
    int count1=1;
	FILE *f_output = fopen(output_file, "w");
	if(f_output==NULL)
	{
		printf("Normalized file creation fail \n");
		return;
	}




	while(!feof(f_input) && count_x<high_en_index+43)
	{
		fscanf(f_input, "%Lf\n", &x);
		y = x * normalize_factor;
		fprintf(f_output, "%Lf\n", y);
        count1++;
        if(count1%N_W==0)
        {
            count_x++;
        }
	}
	fclose(f_input);
	fclose(f_output);
}

/*

// Normalization of wave-form
// filename is original sound-wave file  and filedest is modified wave-form file
void Normalization(char input_file[], char output_file[])
{
	FILE *f_input = fopen(input_file, "r");
	if(f_input==NULL)
	{
		printf("Input file not opening\n");
		return;
	}
    long int high_en_index=energy_file(input_file);
   
	long int i = 0;
	long double max = -1;
	long double x = 0;
	char data[100];
	// skip 5 lines 
	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
	//finding the modulus of maximun amplitude
	int total_window=0;
	while (!feof(f_input))
	{
		fscanf(f_input, "%Lf\n", &x);
		if (mod(x) > max)
			max = mod(x);
		total_window++;
	}
	total_window/=N_W;
	float range = 5000;		
	float normalize_factor = (range / max); //normalize factor
	//normalize_factor = 1;
	fseek(f_input, 0, SEEK_SET);

	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f_input);
	}
    long int count_x=0;
	
	while(high_en_index+43>total_window)
	{
		high_en_index-=5;
	}
	while(high_en_index-42<0)
	{
		high_en_index+=5;
	}
	long int total_=(85*((sliding_size*320)/100))+(320);
	
	long int start_marker=(high_en_index*320)-(total/2);
	long int end_marker=(high_en_index*320)+(total/2)+320;
	printf("%Ld %Ld %Ld",high_en_index,start_marker,end_marker);

	long double y = 0;

	// normalizing wafeform by multiplying with normalization factor
	int j=0,t=0;
    int count1=1;
	FILE *f_output = fopen(output_file, "w");
	if(f_output==NULL)
	{
		printf("Normalized file creation fail \n");
		return;
	}

	while(!feof(f_input))
	{
		fscanf(f_input, "%Lf\n", &x);
		y = x * normalize_factor;
		if(count1>=start_marker && count1<=end_marker)
        {
			fprintf(f_output, "%Lf\n", y);
            
        }
		count1++;
        
	}
	fclose(f_input);
	fclose(f_output);
}
*/


// Calculating DC shift from the file
int dc_shift(char filename[])
{
	FILE *f = fopen(filename, "r");
	int sum = 0;
	int count = 0;
	int x = 0;
	char data[100];
	int i = 0;
	//skip the sound text data
	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f);
	}

	while (!feof(f))
	{
		fscanf(f, "%d\n", &x);
		sum += x;
		count++;
	}
	fclose(f);
	if (sum == 0)
	{
		return 0;
	}
	else
	{
		return sum / count;
	}
	fclose(f);
}
// calculate a[i] forom given R[i] using Durbin's algorithm 
long double *Durbin_ai_calculation(long double R[P + 1])
{
	int i = 0, j = 0, l = 0;
	long double E[P + 1], k[P + 1];
	//long double a[P+1][P+1];
	long double *a[P + 1];
	for (i = 0; i < P + 1; i++)
	{
		a[i] = (long double *)malloc((i + 1) * sizeof(long double));
	}
	//long double a[P+1][P+1];
	/* applying formula for a[i]
	a[0][0]
	a[1][0]	a[1][1]
	a[2][0]	a[2][1]	a[2][2]
	a[3][0]	a[3][1]	a[3][2]	a[3][3]
	a[4][0]	a[4][1]	a[4][2]	a[4][3]	a[4][4]
	.
	.
	.
	.
	a[12][0]......................................a[12][12]
	top to bottom and right to left manner 
	*/
	E[0] = R[0];
	for (i = 1; i <= P; i++)
	{
		k[i] = R[i];
		for (j = 1; j < i; j++)
		{
			k[i] -= a[i - 1][j] * R[i - j];
		}
		k[i] = k[i] / E[i - 1];

		a[i][i] = k[i];
		for (j = i - 1; j > 0; j--)
		{
			a[i][j] = a[i - 1][j] - k[i] * a[i - 1][i - j];
		}
		E[i] = (1 - k[i] * k[i]) * E[i - 1];
	}
	// return a[12]
	/*
	for(i=0;i<P+1;i++)
	{
		a[P][i]=-a[P][i];
	}*/
	return a[P];
}

/*calculating R[i] from normalized file  R[i] calculated for 0<=i<=12 
Calculate_Ri return an array which contain R[0],R[1] to R[12] values.
*/
long double *Calculate_Ri(FILE *fp)
{
	long double *R = (long double *)malloc((P + 1) * sizeof(long double));
	long double x[N_W];
	int i = 0;
	int j = 0, k = 0;
	// read x[i] values from file through pointer 0<=i<N_W 
	for (i = 0; i < N_W; i++)
	{
		if(i==N_W*sliding_size/100)
		{
			f_seek=ftell(fp);
		}
		fscanf(fp, "%Lf\n", &x[i]);
	}

	for (i = 0; i <= P; i++)
	{
		R[i] = 0;
	}
	//calculate R[i] for 0<=i<=12
	for (i = 0; i <= P; i++)
	{
		for (j = 0; j < N_W - i; j++)
		{
			R[i] += (x[j]) * x[j + i];
		}
		//	printf("%d %d %f\n",k,i,R[i]);
	}
	// return array of R
	return R;
}



// Raised sign 
void apply_RSW(long double cis[])
{
	for(int i = 1; i <= 12; ++i)
	{
		long double theta = (M_PI * i) / 12.0;
		long double w = 1 + 6 * sin(theta);
		cis[i] *= w;
	}
}

/* calculating c[i] for a fram when R[i] and a[i] are given*/
long double *calculate_ci(long double R[], long double a[])
{
	long double *c = (long double *)malloc((P + 1) * sizeof(long double));
	// calculating c[0]	
	c[0] = log(R[0] * R[0]);
	int i = 0, k = 0;
	// appling formula for c[i]
	for (i = 1; i <= P; i++)
	{
		c[i] = a[i];
		for (k = 1; k < i; k++)
		{
			c[i] = c[i] + c[k] * a[i - k] * k / i;
		}
	//	printf("c[%d] %lf\n",i,c[i]);
	}
	//return c array which contain c[0],c[1],c[2].........c[12]
	// apply raised sign window on c[i]
	
	apply_RSW(c);

	return c;
}



/* calculating tokhera distance for given test and reference c[i] values */
long double Tokhera_distance_calculation(long double c_t[],long double c_r[])
{
    // w[] contain the weights for finding the tokhera distance 
    long double w[] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
	// distance variable contain the tokhura diatance 
    long double distance = 0, x = 0;
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
void update_centroid(char file_name[],long double code_book[])
{
    // open 
    FILE *f=fopen(file_name,"r");
    int l=0;
    // c[P] read c's values from clluster a 
    long double  c[P];
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
                fscanf(f, "%Lf,", &c[l]);        
            }
            else
            {    
                fscanf(f, "%Lf\n",&c[l]);
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

void code_book_genrate()
{
	  // Opening universe.csv
    char universe_file[]="Universe.csv";
    printf("Opening Universe file......\n");
    FILE *uni_ptr=fopen(universe_file,"r");

    double distortion1=2000,distortion2=0;
    int i=0,j=0,l=0,index=0;
    
    long double  code_book[32][P];                         // contain code block centroid 
    long double  c[P];                                    // temporary entry reading 
    long double  distance=0,min_distance=0,delta=0.0005;
    int count=0;
    char bucket[1024];			//="buc1.txt";
    i=0;
    j=0;
    double temp;
   int loop=0;
   // LBG algorithm 
   /* start with k=1 after optimization size of K increase by  2 times */
   /*k=1 then k=2 then k=4 then k=8*/
    while(K+1<=32)  
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
	
				_snprintf(bucket,sizeof(bucket),"%s%d%s","buc",l,".txt");
             //   bucket[3]='0'+l;
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
                        fscanf(uni_ptr, "%Lf,", &c[l]);
                    else
                        fscanf(uni_ptr, "%Lf\n",&c[l]);               
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
               	_snprintf(bucket,sizeof(bucket),"%s%d%s","buc",index,".txt");
            
				//bucket[3]='0'+index;
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

                char file[1024];	//="buc0.txt";
                _snprintf(file,sizeof(file),"%s%d%s","buc",l,".txt");
            
			//	file[3]='0'+l;
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
                        fprintf(f2, "%Lf,", code_book[i][l]);
                    else
                        fprintf(f2, "%Lf\n",code_book[i][l]);               
                }
            }
            if(loop!=0)
                printf("loop %d distortion => previous :%Lf Current :%Lf diffrence :%Lf\n",loop,distortion1,distortion2,mod(distortion2-distortion1));
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
}
