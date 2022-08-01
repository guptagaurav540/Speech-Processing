// 214101018.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#define N 320
#define P 12
#define Q 12
#define FRAME 5
#define SKIP 500
#define M_PI 3.1415926
// find modulus of amplitude 
float mod(long int x)
{
	if (x < 0)
		return -x;
	return x;
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
		if (mod(x) >= 500.0)
		{
			for (j = 0; j < 100; j++)
			{

				fscanf(fp, "%f\n", &x);
				if (mod(x) >= 500.0)
					i++;
				if (i > 30)
					return;
			}
		}
	}
}
 
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
float *Durbin_ai_calculation(float R[P + 1])
{
	int i = 0, j = 0, l = 0;
	float E[P + 1], k[P + 1];
	//float a[P+1][P+1];
	float *a[P + 1];
	for (i = 0; i < P + 1; i++)
	{
		a[i] = (float *)malloc((i + 1) * sizeof(float));
	}
	//float a[P+1][P+1];
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
float *Calculate_Ri(FILE *fp)
{
	float *R = (float *)malloc((P + 1) * sizeof(float));
	float x[N];
	int i = 0;
	int j = 0, k = 0;
	// read x[i] values from file through pointer 0<=i<N 
	for (i = 0; i < N; i++)
	{
		fscanf(fp, "%f\n", &x[i]);
	}

	for (i = 0; i <= P; i++)
	{
		R[i] = 0;
	}
	//calculate R[i] for 0<=i<=12
	for (i = 0; i <= P; i++)
	{
		for (j = 0; j < N - i; j++)
		{
			R[i] += (x[j]) * x[j + i];
		}
		//	printf("%d %d %f\n",k,i,R[i]);
	}
	// return array of R
	return R;
}

// Normalization of wave-form
// filename is original sound-wave file  and filedest is modified wave-form file
void Normalization(char filename[], char filedest[])
{
	//printf("input %s output %s \n",filename,filedest);
	FILE *f = fopen(filename, "r");
	long int i = 0;
	float max = -1;
	float x = 0;
	// skip 5 lines
	char data[100];
	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f);
	}
	//finding the modulus of maximun amplitude
	while (!feof(f))
	{
		fscanf(f, "%f\n", &x);
		//printf("Amplitude is : %f \n",x);

		if (mod(x) > max)
			max = mod(x);
	}
	//max=293.513;
	//printf("maximum Amplitude is : %f \n",max);
	float range = 5000;
	float normalize_factor = (range / max); //normalize factor
	//normalize_factor = 1;
	fseek(f, 0, SEEK_SET);

	for (i = 0; i < 5; i++)
	{
		fgets(data, 100, f);
	}
	skip_window(f);

	float y = 0;

	FILE *f1 = fopen(filedest, "w");
	// normalizing wafeform by multiplying with normalization factor
	while (!feof(f))
	{
		fscanf(f, "%f\n", &x);
		y = x * normalize_factor;
		fprintf(f1, "%f\n", y);
	}
	fclose(f);
	fclose(f1);
}
// Raised sign 
void apply_RSW(float cis[])
{
	for(int i = 1; i <= 12; ++i)
	{
		float theta = (M_PI * i) / 12.0;
		float w = 1 + 6 * sin(theta);
		cis[i] *= w;
	}
}

/* calculating c[i] for a fram when R[i] and a[i] are given*/
float *calculate_ci(float R[], float a[])
{
	float *c = (float *)malloc((P + 1) * sizeof(float));
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
		//printf("c[%d] %lf\n",i,c[i]);
	}
	//return c array which contain c[0],c[1],c[2].........c[12]
	// apply raised sign window on c[i]
	
	apply_RSW(c);

	return c;
}
/* calculating tokhera distance for given test and reference c[i] values */
float Tokhera_distance_calculation(float w[], float c_t[], float c_r[])
{
	float distance = 0, x = 0;
	int i = 0;
	// applying formula 
	for (i = 1; i < P + 1; i++)
	{
		x = (c_t[i] - c_r[i]);
		distance += w[i] * x * x;
	}
	// return distance 
	return distance;
}
int _tmain(int argc, _TCHAR* argv[])
{
	/********************************Training *****************************************/
	// taking input file in sequence of a,e,i,o,u
	// for training we take o to 9th file for every vowel
	printf("Gaurav Kumar\nRoll-no - 214101018\nTraining started.........\n");
	char input_test[] = "214101018_a_0.txt";
	
	// c_a.txt conatin the c's value of training files 
	char c_file[] = "c_a.txt";
	//tokhera weight 
	float w[] = {0, 1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
	// normalized file names 
	char filename[] = "norm_a_0.txt";
	float *c[10][FRAME];			
	// contain the average c values of each vowel each frame 	
	float c_training[5][5][P + 1];
	FILE *f_c;
	FILE *fp;
	int k = 0;
	int i = 0, j = 0, l = 0;
	// contain the R[i] values for a frame 
	float *R;
	// contain the a[i] values for a frame 
	float *a;
	
	for (l = 0; l < 5; l++){		
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < P + 1; j++)
			{
				c_training[l][i][j] = 0;
			}
		}
	}
	char vowel = 'a';
	// training start for each vowel 
	for (l = 0; l < 5; l++)				// for every vouwel 0=>a,1=>e,2=>i,3=>o,4=>u
	{

		if (l == 0)
			vowel = 'a';
		else if (l == 1)
			vowel = 'e';
		else if (l == 2)
			vowel = 'i';
		else if (l == 3)
			vowel = 'o';
		else
			vowel = 'u';
		//printf("vowel -> %c\n", vowel);

		input_test[10] = vowel;
		filename[5] = vowel;
		// calculating c for each vowel 
		// 10 input file for each vowel 
		for (k = 0; k < 10; k++)
		{
			// change input file name so it automatically detect next file 
			if (k == 0)
			{
				input_test[12] = '0';
				filename[7] = '0';
			}
			if (k != 0)
			{
				input_test[12] += 1;
				filename[7] += 1;
			}
			char file_path[50]="214101018/";
			strcat(file_path,input_test);
			printf("file loading.....%s\n",file_path);
			char norm_path[50]="norm/";
			strcat(norm_path,filename);
			// normalization of file 
			// in normalization we normalize file in range of 5000 and skip the 
			// silent part also
			Normalization(file_path, norm_path);
			fp = fopen(norm_path, "r");
			// Calculating R and ai and ci for 5 consecotive frames from normalized file 
			for (i = 0; i < FRAME; i++)
			{
				// Calculate_Ri return the R[i] values for one frame 
				R = Calculate_Ri(fp);
				// Durbin_ai_calculation return the a[i] values for one frame 
				a = Durbin_ai_calculation(R);
				/* in c[k][i] k denotes kth sound file and i denote ith frame 
				calculate ci return the array of c[i] for a frame when we give r and a values 
				for that frame */ 
				c[k][i] = calculate_ci(R, a);
			}
			free(R);
			free(a);
			fclose(fp);
		}
		/* finding the average of c values for each frame and each c[i]
		we get a c_training[i][j][k] i denote the vowel j denote
		the frame number and k denote the kth c */
		// sum 
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 5; j++)
			{
				for (k = 0; k < P + 1; k++)
				{
					c_training[l][j][k] += c[i][j][k];
				}
			}
		}
		// divide by 10 and dump in file 
		c_file[2] = vowel;
		char c_path[50]="cepstral_co/";
		strcat(c_path,c_file);
		f_c = fopen(c_path, "w");			// c_file storing the c value for each vowel c_a.txt,c_b.txt and so on 
		for (j = 0; j < 5; j++)
		{
			for (k = 1; k < P + 1; k++)
			{
				c_training[l][j][k] /= 10;
				printf("c[%d][%d] %f ", j, k, c_training[l][j][k]);

				if (k < P)
					fprintf(f_c, "%f\t", c_training[l][j][k]);
				else
					fprintf(f_c, "%f\n", c_training[l][j][k]);
			}
			printf("\n");
		}
		fclose(f_c);
	}
	/******** training completed *********************************************
	c_training[i][j][k] => i implies alphabet j implies jth frame and k th c's
	c_training[0] => alphabet a
	c_training[1] => alphabet e
	c_training[2] => alphabet i
	c_training[3] => alphabet o
	c_training[4] => alphabet u
	**************************************************************************/
	/*****testing **************/
	printf("Testing start..............\n");

	char testing_file[] = "214101018_a_10.txt";
	char testing_norm[]="norm_a_10.txt";
	
	char v2='a';
	int percentage=0;
	// testing for each vowel so we have 5 vowel so k<5
	for(k=0;k<5;k++){
		if (k == 0)
			vowel = 'a';
		else if (k == 1)
			vowel = 'e';
		else if (k == 2)
			vowel = 'i';
		else if (k == 3)
			vowel = 'o';
		else
			vowel = 'u';
		testing_file[10]=vowel;
		testing_norm[5]=vowel;
		// for every vowel we test for 10 files so l<10 
		for (l = 0; l < 10; l++)
		{
			if (l == 0){
				testing_file[13] = '0';
				testing_norm[8]='0';
			}
			else
			{
				testing_file[13] += 1;
				testing_norm[8]+=1;
			}
			// normalizeation of testing files 
			char file_path[50]="214101018/";
			char norm_path[50]="norm/";
			strcat(file_path,testing_file);
			strcat(norm_path,testing_norm);
			Normalization(file_path,norm_path);
			printf("file %s testing \n",testing_file);
			
			// open normalized file 
			fp = fopen(norm_path, "r");
			float *c_test[5];
			/*calculating R[i],a[i] and c[i] for each frame 
			and c_test[j] contain the ith frame c values and c[i][j] means ith frame jth c value
			*/
			for (i = 0; i < FRAME; i++)
			{
				// calculatig R
				R = Calculate_Ri(fp);
				// calculating a 
				a = Durbin_ai_calculation(R);
				// calculating c[i] for each frame
				c_test[i] = calculate_ci(R, a);
			}
			fclose(fp);
			float distance[5]; // [i][j] i denote
			for (i = 0; i < 5; i++)
			{
				distance[i] = 0;
			}
			// calculating distance of each frame and taking sum.
			// distance[i] conatin the distance of tested file from ith vowel 
			for (i = 0; i < 5; i++)
			{
				for (j = 0; j < 5; j++)
				{
					// Tokhera_distance_calculation return the distance of jth frame from ith vowel jth frame  
					distance[i] += Tokhera_distance_calculation(w, c_training[i][j], c_test[j]);
				}
				distance[i]=distance[i]/5;
			}
			for (i = 0; i < 5; i++)
			{
				if (i == 0)
					v2 = 'a';
				else if (i == 1)
					v2 = 'e';
				else if (i == 2)
					v2 = 'i';
				else if (i == 3)
					v2 = 'o';
				else
					v2 = 'u';
				printf("distance from %c %f\n",v2, distance[i]);
			}
			

			// finding minimum distance 
			float min = distance[0];
			int min_index = 0;
			for (i = 0; i < 5; i++)
			{
				if (min >= distance[i])
				{
					min = distance[i];
					min_index = i;
				}
			}
			// printing vowel name for teting file 
			printf("file %s is ", testing_file);
			if (min_index == 0){
				printf("Vowel a\n");
				if(k==0)
					percentage++;
			}
			else if (min_index == 1){
				printf("Vowel e\n");
				if(k==1)
					percentage++;
			}
			else if (min_index == 2){
				printf("Vowel i\n");
				if(k==2)
					percentage++;
			}
			else if (min_index == 3){
				printf("Vowel o\n");
				if(k==3)
					percentage++;
			}
			else{
				printf("Vowel u\n");
				if(k==4)
					percentage++;
			}
			printf("\n");
		}
	}

	printf("%d%% accuracy\n\n",percentage*2);
	printf("program ended");
	getch();

	return 0;
}
