// hmm.cpp : Defines the entry point for the console application.
//

// problem_2.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include <math.h>			// for abs()

#include"aaa.h"
#define N 5										// number of state
#define M 32									// size of code book
#define T 85									// time t
#define number_of_observation 20				// number of training observation
long double PI[N];								// PI value 
long double PI_O[N];							// PI previous value 

int obs_seq[number_of_observation][T];			// observation sequence 
long double A[N][N];							// A matrix 
long double B[N][M];							// B matrix
long double A_O[N][N];							// previous A matrix
long double B_O[N][M];							// previous B matrix

long double delta[T][N];						// delta
long double p_star;								// pstar
long double p[T];								// p values 
int q_star[T];									// q star probabilty of best state at time t
long double beta[T][N];							// beta matrix (backward procedure)
long double alpha[T][N];						// alpha matrix (forward procedure) 
long double zeta[T][N][N];						// problem 3 zeta matrix
long double p_o_lemda_denom[T];					// probablity of observation sequence for given model(lemda) 
long double gamma[T][N];						// 
long double new_PI[N];							// new PI calculated in problem number 3
long double a_new[N][N];						// new A value
long double b_new[N][M];						// new B value
long double ps=0;
long double code_book[M][12];
// row sum array[size]
char file_path[500]="196101005_digit/";
char norm_path[500]="norm/";
char roll_number[1024]="214101018";



long double sum_row(long double array[],int size)
{
	int i=0;
	long double sum=0;
	for(i=0;i<size;i++)
	{
		sum+=array[i];
	}
	return sum;
}

// finding the index which contain maximum value 
int find_max_index(long double array[],int size)
{
	int max=0,index=-1,i=0;
	for(i=0;i<size;i++)
	{
		if(max<array[i])
		{
			max=array[i];
			index=i;
		}
	}
	return index;
}



// problem 3 zeta[T][N][N] 
void zeta_calculation(int sequence_no)
{
	int t=0,j=0,i=0;
	int state1=0;
	int state2=0;
	long double denom=0;
	for(t=0;t<T-1;t++)
	{
		denom= 0;
		// calculating denomenator 
		for(state1=0;state1<N;state1++)
		{
			for(state2=0;state2<N;state2++)
			{
				denom+=alpha[t][state1]*A[state1][state2]*B[state2][obs_seq[sequence_no][t+1]-1]*beta[t+1][state2];
				
			}
		}
		// calculating zeta matrix 
		for(state1=0;state1<N;state1++)
		{
			for(state2=0;state2<N;state2++)
			{
				zeta[t][state1][state2]=(alpha[t][state1]*A[state1][state2]*B[state2][obs_seq[sequence_no][t+1]-1]*beta[t+1][state2])/denom;
			}
		}
		p_o_lemda_denom[t]=denom;
		
	}
}

// print zeta matrix
void zeta_print()
{
	int t=0,state1=0,state2=0;
	printf("Zeta print ");
	for(t=0;t<T-1;t++)
	{
		printf("t= %d\n",t);
		for(state1=0;state1<N;state1++)
		{
			for(state2=0;state2<N;state2++)
			{
				printf("%Lg ",zeta[t][state1][state2]);
			}printf("\n");
		}printf("\n");	
	}
}


// gamma print 
void gamma_print()
{
	int t=0,i=0;
	printf("Gamma Print.......\n");
	for(t=0;t<T-1;t++)
	{
		for(i=0;i<N;i++)
		{
			printf("%.3Lf ",gamma[t][i]);
		}
		printf("\n");
	}
}

// gamma calculation gamma[T][N] expecte number of transition from state si at time t
// used for calculating a prime b prime gamma[t][i]=sum (zeta[t][i][j], j=0 to N)
void gamma_calculation(){
	int t=0,i=0,j=0;
	// initialize 0
	for(t=0;t<T;t++)
	{
		for(i=0;i<N;i++)
		{
			gamma[t][i]=0;
		}
	}
	// computing gamma value 
	for(t=0;t<T-1;t++)				// time 
	{
		for(i=0;i<N;i++)			// for state i
		{
			for(j=0;j<N;j++)		// for sum 
			{
				gamma[t][i]+=zeta[t][i][j];
			}
		}
	}
}

// print new PI
void print_pi()
{
	int state1=0;
	for(state1=0;state1<N;state1++)
	{
		printf("%.2Lf ",new_PI[state1]);
	}
}
// new PI computation
// PI[i]= expected frequency in state Si at time t=0
void pi_calculation(char file[],int observation_no){

	char filename[100];
	_snprintf(filename,sizeof(filename),"%s%s_%d%s", "Pi_prime_",file, observation_no,".txt");
	FILE *fop=fopen(filename,"w");

	int state1=0;
	// initialization to 0
	for(state1=0;state1<N;state1++)
	{
		new_PI[state1]=0;
	}
	// computing 
	for(state1=0;state1<N;state1++)
	{
		new_PI[state1]=gamma[0][state1];
		fprintf(fop,"%Lf ",new_PI[state1]);
	}
	fclose(fop);
}


long double power(int i, int j)		// to return pow(i, j)
{
	long double res = 1;
	int k;
	if(j < 0)
	{
		for(k = j; k < 0; k++)
			res /= i;
	}

	else
	{
		for(k = j; k > 0; k--)
			res *= i;
	}

	return res;
}


// print A and B 
void Print_A_B()
{
	
	long double s=0;						// row sum 
	int index=0;						// maximum index
	int state1=0,state2=0,m=0;
	printf("--------------------------------A matrix ------------------------------------\n");
	for(state1=0;state1<N;state1++)
	{
		for(state2=0;state2<N;state2++)
		{
			printf("%Lg  ",a_new[state1][state2]);
	
		}
		printf("\n");
	}
	
	printf("--------------------------------B-matrix-----------------------------------------\n");
	for(state1=0;state1<N;state1++)
	{
		// print new b matrix in file 
		for(m=0;m<M;m++)
		{	
			printf("%Lg  ",b_new[state1][m]);
		}	
		printf("\n");
	}
	
}

// A and B validatation making sochasticated matrix 
void validation_A_B(char file[],int sequence_no)
{
	//printf("after validation\n");

	// create file A_prime_filename_observation number for storing the A prime for observation
	char A_filename[100];
	_snprintf(A_filename,sizeof(A_filename),"%s%s_%d%s", "A_prime_",file, sequence_no,".txt");
	FILE *f=fopen(A_filename,"w");
	
	// create file B_prime_filename_observation number for storing the B prime for observation
	char B_filename[100];
	_snprintf(B_filename,sizeof(B_filename),"%s%s_%d%s", "B_prime_",file, sequence_no,".txt");
	FILE *f1=fopen(B_filename,"w");
	
	long double s=0;						// row sum 
	int index=0;						// maximum index
	int state1=0,state2=0,m=0;
	//printf("A matrix -------------\n");
	for(state1=0;state1<N;state1++)
	{
		s=sum_row(a_new[state1],N);							// row sum for checking schosticated 
		if(s!=0)											// if sum is not zero 
		{
			index=find_max_index(a_new[state1],N);
			if(s>1)											// if sum >0 substract from max value
			{
				a_new[state1][index]-=s-1;
			}
			else											// if sum <0 add to max value 
			{
					a_new[state1][index]+=1-s;
			}
		}
		for(state2=0;state2<N;state2++)
		{
	//		printf("%Lg\t",a_new[state1][state2]);
			fprintf(f,"%Lg ",a_new[state1][state2]);

		}
	//	printf("\n");
		fprintf(f,"\n");
	}
	fclose(f);
	long double threshold = power(10, -30);							// threshold if B matrix element is 0 then add threshold 
	
//	printf("B-matrix---------------------------\n");
	for(state1=0;state1<N;state1++)
	{
		for(m=0;m<M;m++)
		{
			if(b_new[state1][m]==0)
			{
				b_new[state1][m]=threshold;
			}
		}
		s=sum_row(b_new[state1],M);								// sum of row 
		if(s!=0)												// if sum is not 1
		{	
			index=find_max_index(b_new[state1],M);				// find max number index
			if(s>1)
			{
				b_new[state1][index]-=(s-1);					// if sum > 1 then substract
			}
			else
			{
				b_new[state1][index]+=(1-s);					// if sum <1 then add 
			}
		}
		// print new b matrix in file 
		for(m=0;m<M;m++)
		{	
		//	printf("%Lg ",b_new[state1][m]);
			fprintf(f1,"%Lg ",b_new[state1][m]);
		}	
	//	printf("\n");
		fprintf(f1,"\n");
	}
	fclose(f1);
}
// A prime B prime calculation problem 3 
void A_B_prime_calculation(int sequence_no,char file[]){
	printf("\n........................................A Prime calculation...............................................\n");	

	int state1=0,state2=0,t=0;
	 // initialize 0
	for(state1=0;state1<N;state1++)
	{
		for(state2=0;state2<N;state2++)
		{
			a_new[state1][state2]=0;
		}
	}
	// new A calculation
	double long exp_tran_state_i=0;
	for(state1=0;state1<N;state1++)
	{
		exp_tran_state_i=0;					
		// expected transition form state Si, t=0 to T-2
		for(t=0;t<T-1;t++)
		{

			exp_tran_state_i+=gamma[t][state1];
		}
		// expected transition from state Si to Sj/expected transition form state Si, t=0 to T-2
		for(state2=0;state2<N;state2++)
		{
			for(t=0;t<T-1;t++)
			{
				a_new[state1][state2]+=zeta[t][state1][state2];
			}
			a_new[state1][state2]/=exp_tran_state_i;
		}
	}
	

	printf("\n.......................................B prime calculation.....................................\n");
	int m=0;
	// initialize 0
	for(state1=0;state1<N;state1++)
	{
		for(m=0;m<M;m++)
		{
			b_new[state1][m]=0;
		}
	}
	// new beta calculation
	long double denom_sum=0,number_sum=0;
	for(state1=0;state1<N;state1++)
	{
		denom_sum=0;
		for(t=0;t<T;t++)
		{
			denom_sum+=gamma[t][state1];
		}
		for(m=0;m<M;m++)
		{
			number_sum=0;
			for(t=0;t<T;t++)
			{
				if(obs_seq[sequence_no][t]-1==m)
				{
					number_sum+=gamma[t][state1];
				}
			}
			b_new[state1][m]=number_sum/denom_sum;
		}		
	}
	// validation of A and B matrix make schosticated matrix 
	validation_A_B(file,sequence_no);
}

// print beta matrix
void print_beta()
{
	for(int t=0;t<T;t++)
	{
		printf("t= %d ",t);
		
		for(int i=0;i<N;i++)
		{
			printf("%Lg ",beta[t][i]);
		}
		printf("\n");
	}
}
// print alpha matrix
void print_alpha()
{
	for(int t=0;t<T;t++)
	{
		printf("t= %d ",t);
		for(int i=0;i<N;i++)
		{
			printf("%Lg ",alpha[t][i]);
		}
		printf("\n");
	}
}
// beta calculation problem 1 backward 
void beta_calculation(int obs_no)
{
	printf("backward\n");
	int i=0,t=0;

	// 0 initialization 
	for(i=0;i<N;i++)
	{
		for(t=0;t<T;t++)
		{
			beta[t][i]=0;
		}
	}
	// initialization beta T 
	for(i=0;i<N;i++)
	{
		beta[T-1][i]=1;
	}
	long double sum;
	// calculation of beta matrix for t=84 to 0
	for(t=T-2;t>=0;t--)
	{
		
		for(i=0;i<N;i++)
		{
			sum=0;	
			for(int j=0;j<N;j++)
			{
				sum+=A[i][j]*B[j][obs_seq[obs_no][t+1]-1]*beta[t+1][j];
			}

			beta[t][i]=sum;
		}
	}
	
}

// alpha calculation forward procedure 
void alpha_calculation(int obs_no)
{
	
	printf("Forward\n");
	int i=0,t=0;
	for(i=0;i<N;i++)
	{
		for(t=0;t<T;t++)
		{
			alpha[t][i]=0;
		}
	}
	// initialization
		
	for(i=0;i<N;i++)
	{
		
		alpha[0][i]=PI[i]*B[i][obs_seq[obs_no][0]-1];
	
	}
	long double sum;
	for(t=1;t<T;t++)
	{
		for(i=0;i<N;i++)
		{
			sum=0;
		
			for(int j=0;j<N;j++)
			{
				sum+=alpha[t-1][j]*A[j][i];
			}

			alpha[t][i]=sum*B[i][obs_seq[obs_no][t]-1];
		}
	}
}


// read PI file from file 
void read_pi(char filename[])
{
    FILE *f_pi=fopen(filename,"r");
    int i=0;
	long double temp=0;
    for(i=0;i<N;i++)
    {
        PI[i]=0;
    }
    i=0;
    while(!feof(f_pi))
    {
        fscanf(f_pi,"%Lf\t",&temp);
        PI[i++]=temp;
    }
    fclose(f_pi);
}


// read observation sequence 
// read_obs_seq(filename (digit), number of observation )
void read_obs_seq(char filename[],int num_obs)
{
	
    FILE *f_obs=fopen(filename,"r");			// observation file 
    int observation_number=0;
    char temp1[1024];
    int temp=0;
    int time=0;
    for(observation_number=0;observation_number<num_obs;observation_number++)
    {
        for(time=0;time<T;time++)
        {       
            obs_seq[observation_number][time]=0;
        }
    }
    for(observation_number=0;observation_number<num_obs;observation_number++)
    {
        fgets(temp1,sizeof(temp1),f_obs);
        for(time=0;time<T;time++)
        {       
            if(time<T)
                fscanf(f_obs,"%d ",&temp);
            else
                fscanf(f_obs,"%d \n",&temp);
                printf("%d ",temp);
            
            obs_seq[observation_number][time]=temp;
        }
		printf("\n");
    }
    fclose(f_obs);
}

// read A matrix
void read_A(char filename[])
{
    FILE *f_a=fopen(filename,"r");
	if(f_a==NULL)
	{
		printf("A not loaded.............\n");
		return ;
	}
    int state1=0,state2=0;
    for(state1=0;state1<N;state1++)
    {
        for(state2=0;state2<N;state2++)
        {
            A[state1][state2]=0;
        }
    }
    state1=0,state2=0;
    for(state1=0;state1<N;state1++)
    {
        for(state2=0;state2<N;state2++)
        {
            if(state2<N)
            fscanf(f_a,"%Lg ",&A[state1][state2]);
            else
            fscanf(f_a,"%Lg\n",&A[state1][state2]);
			printf("%Lg ",A[state1][state2]);;
        }
		printf("\n");
    }
	fclose(f_a);
}

// read B matrix 
void read_B(char filename[])
{
    FILE *f_beta=fopen(filename,"r");
    int m=0,state=0;
    for(state=0;state<N;state++)
    {
        for(m=0;m<M;m++)
        {
            B[state][m];
        }
    }
    for(state=0;state<N;state++)
    {
        for(m=0;m<M;m++)
        {
            if(m<M)
			{
            	fscanf(f_beta,"%Lg ",&B[state][m]);
			}
            else
			{
            	fscanf(f_beta,"%Lg\n",&B[state][m]);
			}
        }
    }
    fclose(f_beta);   
}

//delta matrix initialization
void delta_initialization(int sequence_no)
{
    int state1,state2;
    for(state1=0;state1<T;state1++)
        for(state2=0;state2<N;state2++)
            delta[state1][state2]=0;
   
    //initalization
    for(state1=0;state1<N;state1++){
        delta[0][state1]=PI[state1]*B[state1][obs_seq[sequence_no][0]-1];
    }
    
}
// viterbi algorithm 
// delta matrix calculation for observation number =sequence number 
void delta_calculation(int sequence_no)
{
	delta_initialization(sequence_no);
    int time=0,state1=0,state2=0;
    long double max=-8888;
    long double temp3;
	int index=-1;
    for(time=1;time<T;time++)
    {
        for(state2=0;state2<N;state2++)
        {			
            max=-1;
		    for( state1=0;state1<N;state1++)
            {
                temp3=delta[time-1][state1]*A[state1][state2];
				if(max<temp3)
				{
                    max=temp3;
				}
            }            
            delta[time][state2]=max*B[state2][obs_seq[sequence_no][time]-1];
		}
    }
}

// print delta 
void print_delta()
{
	printf("delta_print\n");
    int t=0,i=0;
    for(t=0;t<T;t++){

        printf("at time t=%d    ",t);
        for(i=0;i<N;i++)
            printf("%Lg ",delta[t][i]);
        printf("\n\n");
    }
}


// find p star and q star sequence from delta matrix termination 
void find_p_star_q_star()
{
    int t=1,i=0;
    long double max;
    int index=-1;
    for(t=0;t<=T;t++)
    {
        max=-1;//delta[t][1];
        index=-1;
	//	printf("t= %d ",t); 
        for(i=0;i<N;i++)
        {
            if(max<delta[t][i])
            {
                max=delta[t][i];
                index=i;
            }
		//	printf("%Lg ",delta[t][i]);
        }
		//printf("%d\n",index);
        q_star[t]=index;
		p[t]=max;
    }
	p_star=p[T-1];
	ps=p_star;
//	printf("P_star ka value %Lg\n",p_star);
}
void print_qstar()
{
	int t=0;
	for(t=0;t<T;t++)
	{
		printf("%3d",q_star[t]);
	}
	printf("\n\n");

}


void initialize_0()
{
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			A[i][j]=0;
		}
		for(j=0;j<M;j++)
		{
			B[i][j]=0;
			
		}
		PI[i]=0;
	}

}

// find average A and Band PI prime of all observation 
void final_A_B_PI(char file[],int no_of_observations,int itr)
{
	
	int i=0,j=0;
	
	long double a[N][N]={0},b[N][M]={0},pi[N]={0};
	//initialization and storing the previous A matrix B matrix and PI matrix
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			A_O[i][j]=A[i][j];
			a[i][j]=0;
		}
		for(j=0;j<M;j++)
		{
			B_O[i][j]=B[i][j];
			b[i][j]=0;
		}
		PI_O[i]=PI[i];
		pi[i]=0;
	}
	
	// read A_prime_filenaem_observationNumber.txt for all observation and sum matrix after thaat divide all elements of matrix by number of observation
	char A_filename[100];
	char B_filename[100];
	char P_filename[100];
	FILE *f_A,*f_B,*f_p;
	int o=0;
	// for observation 0 to N
	for(o=0;o<no_of_observations;o++){
		// read files 
		_snprintf(A_filename,sizeof(A_filename),"%s%s_%d%s", "A_prime_",file, o,".txt");
		_snprintf(B_filename,sizeof(B_filename),"%s%s_%d%s", "B_prime_",file, o,".txt");
		_snprintf(P_filename,sizeof(P_filename),"%s%s_%d%s", "Pi_prime_",file, o,".txt");
		
		f_A=fopen(A_filename,"r");
		f_B=fopen(B_filename,"r");
		f_p=fopen(P_filename,"r");
		initialize_0();
		// read A B P prime of observation
		read_A(A_filename);
		read_B(B_filename);
		read_pi(P_filename);
		// sum all matrix element wise and store in a ,b, pi matrix
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{
				a[i][j]+=A[i][j];
			}
			for(j=0;j<M;j++)
			{
				b[i][j]+=B[i][j];
			}
			pi[i]+=PI[i];
		}
		fclose(f_A);
		fclose(f_B);
		fclose(f_p);

	}
	_snprintf(A_filename,sizeof(A_filename),"%s%s%s", "A_final",file,".txt");
	_snprintf(B_filename,sizeof(B_filename),"%s%s%s", "B_final",file,".txt");
	_snprintf(P_filename,sizeof(P_filename),"%s%s%s", "Pi_final",file,".txt");
	
	f_A=fopen(A_filename,"w");
	f_B=fopen(B_filename,"w");
	f_p=fopen(P_filename,"w");
	// find average of matrix and store it in file A_final_filename.txt ,B_final_filename.txt.............
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			a[i][j]/=no_of_observations;
			fprintf(f_A,"%Lg ",a[i][j]);
		}
		
		fprintf(f_A,"\n");
		for(j=0;j<M;j++)
		{
			b[i][j]/=no_of_observations;
			
			fprintf(f_B,"%Lg ",b[i][j]);
		}
		fprintf(f_B,"\n");
		pi[i]/=no_of_observations;
		fprintf(f_p,"%Lf ",pi[i]);
	}
	fclose(f_A);
	fclose(f_B);
	fclose(f_p);

}
void training(char training_file[])
{
	char pi_filename[1024];
	char a_filename[1024];
	char observation_filename[1024];
	char b_filename[1024];
	char p_star_f[1024];
	long double p_o_lemda=0;
    int num=0;
	FILE *f;
	int num_obs=number_of_observation;
	int iteration2=0,iteration1=0;
	int count=0;
	_snprintf(observation_filename,sizeof(observation_filename),"%s%s%s","obs_seq_",training_file,".txt");
	printf("%s",observation_filename);
			
	read_obs_seq(observation_filename,number_of_observation);
	
	
	
	while(iteration2<1)
	{	
 		iteration1=0;
		while(iteration1<20)
		{
		
			
			for(int i=0;i<number_of_observation;i++)
			{
				printf("-------------------------------count %d ---------------------------------------\n",count++);
				printf("iteration number - %d\n\ndigit %s processing..........\n",iteration1,training_file);	
		
				printf("Obseravtion sequence number - %d\n\n",i);
			
				if(iteration1==0 && iteration2==0)
				{
					_snprintf(pi_filename,sizeof(pi_filename),"%s%s", "Pi",".txt");
					_snprintf(a_filename,sizeof(a_filename),"%s%s", "A",".txt");
					_snprintf(b_filename,sizeof(b_filename),"%s%s", "B",".txt");
				}
				else if(iteration1==0 && iteration2!=0)
				{	
					_snprintf(pi_filename,sizeof(pi_filename),"%s%s%s", "Pi_final", training_file,".txt");
					_snprintf(a_filename,sizeof(a_filename),"%s%s%s", "A_final",training_file,".txt");
					_snprintf(b_filename,sizeof(b_filename),"%s%s%s", "B_final",training_file,".txt");
				}
				else{
					_snprintf(pi_filename,sizeof(pi_filename),"%s%s_%d%s", "Pi_prime_", training_file,i,".txt");
					_snprintf(a_filename,sizeof(a_filename),"%s%s_%d%s", "A_prime_", training_file,i,".txt");
					_snprintf(b_filename,sizeof(b_filename),"%s%s_%d%s", "B_prime_",training_file,i,".txt");
				}
				read_A(a_filename);
				read_B(b_filename);
				read_pi(pi_filename);
				// problem 1
				alpha_calculation(i);
				//print_alpha();
				beta_calculation(i);
				//print_beta();
				// problem 2 veterbi algorithm
				delta_calculation(i);
				//print_delta();
			
				find_p_star_q_star();
			
				printf("P star value = %Lg\n",p_star);
				//fprintf(f,"%Lg ",ps);
				printf("q star ----\n");
				print_qstar();
				// problem 3
				zeta_calculation(i);
				gamma_calculation();
				pi_calculation(training_file,i);
				A_B_prime_calculation(i,training_file);
				// print A B matrix
				Print_A_B();
				printf("\n------------------------------------------------------------------------------------------------------------------\n");
		
			}
			iteration1++;
		}
		printf("---------------------Average A prime Bprime and PI --------------------------------------");
		final_A_B_PI(training_file,number_of_observation, iteration2);		
		iteration2++;
	}
}

void load_code_book()
{
	printf("code book loading ..........\n");
	int i=0,j=0;
	for(i=0;i<M;i++)
	{
		for(j=0;j<12;j++)
		{
			code_book[i][j]=0;
		}
	}

	FILE *f=fopen("codebook2.txt","r");
	for(int i=0;i<M;i++)
	{
		for(j=0;j<12;j++)
		{
			if(j<11)
				fscanf(f,"%Lf\t,",&code_book[i][j]);
			else 
				fscanf(f,"%Lf\t\n",&code_book[i][j]);
		}
	}
	fclose(f);

}
// calculate P(O|lemda) for given module 
long double calculate_p_o_lemda()
{
	long double p_o_le=0;
	for(int i=0;i<N;i++)
	{
		p_o_le+=alpha[T][i];
	}
	return p_o_le;
}

// genrate observation sequence for testing file in obs_seq.txt
void genrate_testing_seq(char testing_file[])
{
	// load code book 
	load_code_book();
	char testing_norm[1024]="norm.txt";				//="norm_a_10.txt";
	FILE *f_norm;
	int l=0;
	long double distance[M]; // [i][j] i denote
	long double *c_test;
	int frame=0,j=0;
	// contain the R[i] values for a frame 
	long double *R;
	// contain the a[i] values for a frame 
	long double *a;

	// for every vowel we test for 10 files so l<10 
	// normalizeation of testing files 
	strcat(file_path,testing_file);
	strcat(norm_path,testing_norm);
	Normalization(file_path,norm_path);
	printf("file %s testing \n",testing_file);
	printf("Normalization_done............\n");
		
	// open normalized file 
	f_norm = fopen(norm_path, "r");
	// read observation sequence 
	char file_observation[]="obs_seq.txt";	
	FILE *f_observation=fopen(file_observation,"w");
	
	fprintf(f_observation,"-----------------------------------%s-----------------------\n",testing_file);
	for (frame = 0; frame < 85;frame++)
	{
		// calculatig R for ith frame 
		R = Calculate_Ri(f_norm);
		fseek(f_norm, (long)f_seek, SEEK_SET);
		// calculating a for given R 
		a = Durbin_ai_calculation(R);
		// calculating c[i] for each frame
		c_test = calculate_ci(R, a);
		//printf("c calculated..........\n");
		// initialize distance =0
		for (j = 0; j < M; j++)
		{
			distance[j] = 0;
		}
		// calculating distance of each frame and taking sum.
		// distance[i] conatin the distance of tested file from ith vowel 
		for (j = 0; j < M; j++)
		{	
			// Tokhera_distance_calculation return the distance of jth frame from ith vowel jth frame  
			distance[j] = Tokhera_distance_calculation(code_book[j], c_test);
		//	printf("distance %g..........\n",distance[j]);
		}

		// finding minimum distance and index  
		long double min = distance[0];
		int min_index = 0;
		for (j = 0; j < M; j++)
		{
			if (min >= distance[j])
			{
				min = distance[j];
				min_index = j;
			}
		}
		//printf("%d ",min_index);
		// insert observation sequence for given sound sample 
		fprintf(f_observation,"%d ",min_index+1);
	}
	printf("\n");
	fprintf(f_observation,"\n");
	
	fclose(f_norm);
	fclose(f_observation);

}

int testing(char testing_file[])
{
	// genrate observation sequence for sound
	genrate_testing_seq(testing_file);
	char pi_filename[1024];
	char a_filename[1024];
	char observation_filename[1024]="obs_seq.txt";
	char b_filename[1024];
	printf("%s",observation_filename);
			
	read_obs_seq(observation_filename,1);
	long double p_o_lemda[10]={0};	
	int number_of_digit=10,digit_i=0;		
	for( digit_i=0;digit_i<number_of_digit;digit_i++)
	{
		_snprintf(pi_filename,sizeof(pi_filename),"%s%d%s", "Pi_final",digit_i,".txt");
		_snprintf(a_filename,sizeof(a_filename),"%s%d%s", "A_final",digit_i,".txt");
		_snprintf(b_filename,sizeof(b_filename),"%s%d%s", "B_final",digit_i,".txt");	
		
		read_A(a_filename);
		read_B(b_filename);
		read_pi(pi_filename);
		// problem 1
		alpha_calculation(0);
		//print_alpha();
		beta_calculation(0);
		//print_beta();
		// problem 2 veterbi algorithm
		delta_calculation(0);
		//print_delta();
		p_o_lemda[digit_i]=calculate_p_o_lemda();
		printf("\n------------------------------------------------------------------------------------------------------------------\n");
	}
	long double min=0;
	int sound_index=0;
	for(digit_i=0;digit_i<number_of_digit;digit_i++ )
	{
		if(min<p_o_lemda[digit_i])
		{
			min=p_o_lemda[digit_i];
			sound_index=digit_i;
		}
	}
	printf("Sound is %d ",sound_index);
	return sound_index;

}

void genrate_universe(char testing_file[])				
{
	char testing_norm[1024];				//="norm_a_10.txt";
	FILE *f_norm;
	int l=0;
	long double distance[M]; // [i][j] i denote
	
	long double *c_test;
	int i=0,j=0;
	// contain the R[i] values for a frame 
	long double *R;
	// contain the a[i] values for a frame 
	long double *a;
	_snprintf(testing_norm,sizeof(testing_norm),"%s%s", "norm_",testing_file);
	// normalizeation of testing files 
	strcat(file_path,testing_file);
	strcat(norm_path,testing_norm);
	Normalization(file_path,norm_path);
	printf("file %s testing \n",testing_file);
	printf("Normalization_done............\n");
		
	// open normalized file 
	f_norm = fopen(norm_path, "r");
	FILE *f_universe=fopen("universe.csv","a+");	
	int count_w=85;



	for(i=0;i<count_w;i++)
	{
//		printf("Frame %d processing\n",i);
		//printf("frame %d ............\n",i);
		// calculatig R
		R = Calculate_Ri(f_norm);
		//printf("R calculated..........\n");
		// calculating a 
		a = Durbin_ai_calculation(R);
		// calculating c[i] for each frame
		//printf("a calculated..........\n");
		c_test = calculate_ci(R, a);
		for(j=1;j<=12;j++)
		{
			if(j<12)
				fprintf(f_universe,"%f,",c_test[j]);
			else{
				fprintf(f_universe,"%f\n",c_test[j]);
			}
		}
	}
	fclose(f_norm);
	fclose(f_universe);
	printf("\n");

}

void universe()
{
	FILE *f=fopen("universe.csv","w");	
	fclose(f);
	int i=0;
	char filename[1024];
	int number_of_input=20;
	for(int j=0;j<10;j++)
	{	
		for(int i=1;i<=number_of_input;i++)
		{
			_snprintf(filename,sizeof(filename),"%s_%d_%d%s",roll_number,j,i,".txt");
			genrate_universe(filename);	
		}
	}			
}

void genrate_training_observation_sequence(char training_file[],char file_s[])				
{
	char testing_norm[1024];				//="norm_a_10.txt";
	//tokhera weight 
	FILE *f_norm;
	int l=0;
	long double distance[M]; // [i][j] i denote
	
	long double *c_test;
	int frame=0,j=0;
	// contain the R[i] values for a frame 
	long double *R;
	// contain the a[i] values for a frame 
	long double *a;

	// for every vowel we test for 10 files so l<10 
	_snprintf(testing_norm,sizeof(testing_norm),"%s%s", "norm_",training_file);
	// normalizeation of testing files 
	strcat(file_path,training_file);
	strcat(norm_path,testing_norm);
	Normalization(file_path,norm_path);
	printf("file %s testing \n",training_file);
	printf("Normalization_done............\n");
		
	// open normalized file 
	f_norm = fopen(norm_path, "r");
	
	char file_o[1024];
	_snprintf(file_o,sizeof(file_o),"%s%s%s","obs_seq_",file_s,".txt");
	FILE *f_observation=fopen(file_o,"a+");
	
	fprintf(f_observation,"-----------------------------------%s-----------------------\n",training_file);
	for (frame = 0; frame < 85;frame++)
	{
		// calculatig R
		R = Calculate_Ri(f_norm);
		fseek(f_norm, (long)f_seek, SEEK_SET);
		// calculating a 
		a = Durbin_ai_calculation(R);
		// calculating c[i] for each frame
		c_test = calculate_ci(R, a);

		for (j = 0; j < M; j++)
		{
			distance[j] = 0;
		}
		// calculating distance of each frame and taking sum.
		// distance[i] conatin the distance of tested file from ith vowel 
		for (j = 0; j < M; j++)
		{	
			// Tokhera_distance_calculation return the distance of jth frame from ith vowel jth frame  
			distance[j] = Tokhera_distance_calculation(code_book[j], c_test);
		}

		// finding minimum distance 
		long double min = distance[0];
		int min_index = 0;
		for (j = 0; j < M; j++)
		{
			if (min >= distance[j])
			{
				min = distance[j];
				min_index = j;
			}
		}
		printf("%d ",min_index);
	
		fprintf(f_observation,"%d ",min_index+1);
	}
	printf("\n");
	fclose(f_norm);
	
	fprintf(f_observation,"\n");
	fclose(f_observation);

}
void genrate_sequence_training(char file[])
{
	
	load_code_book();
	

	int seq=5;
	char file_o[1024];
	//int number_of_digit= number_of_observation;
	_snprintf(file_o,sizeof(file_o),"%s%s%s","obs_seq_",file,".txt");
	FILE *f_ob=fopen(file_o,"w");
	fclose(f_ob);
	for(int i=1;i<=number_of_observation;i++)
	{
		_snprintf(file_o,sizeof(file_o),"%s_%s_%d%s",roll_number,file,i,".txt");
		genrate_training_observation_sequence(file_o,file);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	char ff[1024]="one_1.txt";
	//universe();
	//code_book_genrate();
	int number_of_digit=10;
	char digit[10][1024]={"0","1","2","3","4","5","6","7","8","9"};
	//char digit[3][1024]={"1","7","8"};
	
	/*for(int i=0;i<number_of_digit;i++)
	{
		genrate_sequence_training(digit[i]);
	}*/
	/*for(int i=0;i<10;i++)
	{
		training(digit[i]);
	}*/
	//testing
	char file_name[1024]; 
	int count=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			_snprintf(file_name,sizeof(file_name),"%s_%d_%d%s","196101005",i,j+21,".txt");
			if(testing(file_name)==i)
			{
				count++;
			}
		}
	}
	printf("accuracy %d\n",count);
	getch();

	return 0;
}






int _tmain(int argc, _TCHAR* argv[])
{

	int number_of_digit=10,i=0;
	char digit[10][1024]={"0","1","2","3","4","5","6","7","8","9"};
	char file_name[1024]; 
	int count=0;
	
	printf("214101018 \nGuarav Kumar\nHMM\n");

	printf("Main Menu :\n");
	printf("1.\tCreate Universe : \n");
	printf("2.\tCode book Genaration :\n");
	printf("3.\tTraining :\n");
	printf("4.\tTesting :\n");
	int choice=0;
	printf("Enter Your Choice :\n");
	scanf("%d",&choice);
	switch (choice)
	{
	case 1:
		universe();
		break;
	case 2:
		code_book_genrate();
		break;
	case 3:
		for( i=0;i<number_of_digit;i++)
		{
			genrate_sequence_training(digit[i]);
		}
		for(i=0;i<10;i++)
		{
			training(digit[i]);
		}
		break;
	case 4:
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				_snprintf(file_name,sizeof(file_name),"%s_%d_%d%s",roll_number,i,j+21,".txt");
				if(testing(file_name)==i)
				{
					count++;
				}
			}
		}
		printf("accuracy %d\n",count);

		break;
	default:
		break;
	}
	getch();

	return 0;
}