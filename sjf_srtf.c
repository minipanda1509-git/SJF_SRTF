// SJF_SRTF.cpp : Defines the entry point for the console application.
//
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>


float srtf_wt, srtf_tat, sjf_wt,sjf_tat;  // For measuring the improvements of SRTF over SJF

//-----------------------------------------------------SJF------------------------------------------------------------------------

int NextProcess(int n,int *arrival_time,int *burst_time,int clock_time) //Chooses the next process to be sent to the queue for SJF
{
	int i,index=-1,count=0;

	for(i=0;i<n;i++)
	{

		if(arrival_time[i]!=-1 && arrival_time[i]<=clock_time) // If the process has not been chosen  (arrival_time[i]!=-1)
			                                                   //and if it has already arrived (arrival_time[i]<=clock_time) 
		{ 
			if(count==0)
			{
				index=i;
			}

			else
			{
		
				if(burst_time[i]<burst_time[index])  //The one with shorter burst time will be chosen first
					index=i;
			}

			count++;
		}
	}
	return index;  //returns the index of process (in sorted array) to be sent next to CPU
}

void NonPrempt_SJF(int n,int *arrival_time,int *burst_time,int *indice)
{
	int i,processes_executed=0,k;
	int clock_time=0;
	int index;
	int *waiting_time=(int *)malloc(sizeof(int)*n);
	int *tat=(int *)malloc(sizeof(int)*n);
	float avg_waiting_time=0;
	float avg_tat=0;
	printf("---------------Non Premptive SJF---------------\n\n");
	while(processes_executed<n)  //Until all processes get executed
	{
	   	index=NextProcess(n,arrival_time,burst_time,clock_time); //gets index of the process to be executed next

		if(index==-1)  //if no process is there in the queue
		{
			clock_time++; //increment the clock 
			continue;    
		}

		waiting_time[index]=clock_time-arrival_time[index]; 
		printf("|%d",clock_time);
		clock_time+=burst_time[index]; // The process is sent to CPU and it stays there for burst_time[index] time
		
		for(k=0;k<burst_time[index]/2;k++)
			printf("%c",177);
		printf("P%d",indice[index]+1);
		for(k=0;k<burst_time[index]/2;k++)
			printf("%c",177);
		printf("%d|",clock_time);
		
		tat[index]=clock_time-arrival_time[index];
		arrival_time[index]=-1; //This process is done
		processes_executed++; //So increment the count

		 
	}

	for(i=0;i<n;i++)
	{
		avg_waiting_time+=waiting_time[i];
		avg_tat+=tat[i];
	}

	avg_waiting_time/=n;
	avg_tat/=n;

	sjf_wt=avg_waiting_time;
	sjf_tat=avg_tat;

 	printf("\n\nAverage waiting time %f \n",avg_waiting_time);
	printf("Average turn around time %f ",avg_tat);
}

//--------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------SRTF------------------------------------------------------------------------

int NextProcess_2(int n,int *arrival_time,int *burst_time,int clock_time) //Chooses the next process to be sent to the queue for SJF
{
	int i,index=-1,count=0;

	for(i=0;i<n;i++)
	{

		if(arrival_time[i]!=-1 && arrival_time[i]<=clock_time) // If the process has not been chosen  (arrival_time[i]!=-1)
                                                              //and if it has already arrived (arrival_time[i]<=clock_time)
		{
			if(count==0)
			{
				index=i;
			}

			else
			{
		
				if(burst_time[i]<burst_time[index])  //The shorter process gets more priority
					index=i;
			}

			count++;
		}
	}

    return index;
}

void Prempt_SJF(int n,int *arrival_time,int *burst_time, int *indice)
{

	int clock_time=0,i,index,start=0;
	int processes_executed=0;
	int *lastServed=(int *)malloc(n*sizeof(int)); //last clock_time when process was served
	int *waiting_time=(int *)malloc(n*sizeof(int));
	int *tat=(int *)malloc(n*sizeof(int));
	float avg_waiting_time=0;
	float avg_tat=0;
	int prev_process=-1;

	for(i=0;i<n;i++)
		waiting_time[i]=0;

	for(i=0;i<n;i++)
		lastServed[i]=arrival_time[i];  // stores the last served time of the each process
	                                   //  will get updated after every moment when CPU takes it 
	printf("---------------Premptive SJF---------------\n\n");
	
	while(processes_executed<n)
	{
		index=NextProcess_2(n,arrival_time,burst_time,clock_time); //Gets the index of the process to be served next
		if(index==-1) //if no process is there in the queue
		{
			clock_time++; //increment the clock
			continue;
		}
        
		else if(index==prev_process) //prev_process
		{
			printf("  ");
		}

		else 
			{
				if(start==0)
					{
						printf("|%d  P%d ",clock_time,indice[index]+1);
						start=1;
				}
				else
				    printf("%d||%d   P%d ",clock_time,clock_time,indice[index]+1);
				prev_process=index;
		    }

		burst_time[index]--;  //One unit of execution is over so decrement the burst time of the process

		waiting_time[index]+=clock_time-lastServed[index]; 

		clock_time++;

		lastServed[index]=clock_time; //the last served time of this process gets updated

		if(burst_time[index]==0) //Nothing is left for this process
			{
				processes_executed++; //increment the count
				tat[index]=clock_time-arrival_time[index];
				arrival_time[index]=-1;  //The process is done executing
		    }
	}

	printf("  %d|",clock_time);
	for(i=0;i<n;i++)
	{
	//	printf("\n%d %d %d",i,waiting_time[i],tat[i]);
		avg_waiting_time+=waiting_time[i];
		avg_tat+=tat[i];
	}

	avg_waiting_time/=n;
	avg_tat/=n;

	srtf_wt=avg_waiting_time;
	srtf_tat=avg_tat;

	printf("\n\nAverage waiting time %f \n",avg_waiting_time);
	printf("Average turn around time %f ",avg_tat);
}

//-----------------------------------------------------------------------------------------------------------------------------


void Sort(int n,int *arrival_time,int *burst_time, int *index)  //Sorts according to the arrival time
{
	int i,j;
	int tmp1,tmp2,tmp3;

	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(arrival_time[j]>arrival_time[j+1])
			{

				tmp1=arrival_time[j];
				tmp2=burst_time[j];
				tmp3=index[j];
				
				arrival_time[j]=arrival_time[j+1];
				burst_time[j]=burst_time[j+1];
				index[j]=index[j+1];
				

				arrival_time[j+1]=tmp1;
				burst_time[j+1]=tmp2;
				index[j+1]=tmp3;
			}

			else if(arrival_time[j]==arrival_time[j+1]) // If there is a tie
			{
				if(burst_time[j]>burst_time[j+1]) //then check for the burst time.. the shorter one gets more priority
				{
					tmp1=arrival_time[j];
				    tmp2=burst_time[j];
				
				    arrival_time[j]=arrival_time[j+1];
				    burst_time[j]=burst_time[j+1];

				    arrival_time[j+1]=tmp1;
				    burst_time[j+1]=tmp2;
				}
			}
		}
	}

}

const char* getfield(char* line, int num)  // Reads field from csv file
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main()
{
	int i,n=0;
	int arrival_time[50],burst_time[50],index[50];
	int arrival_copy[50],burst_copy[50],index_copy[50];
    char str[20];
	printf("File name..? ");
	scanf("%s",str);

  
	FILE* fptr = fopen(str, "r");

    char line[1024];

    while (fgets(line, 1024, fptr))
    {
    	n++;
    	if(n==1) continue;
    	
	
        char* tmp1 = strdup(line);
        char* tmp2 = strdup(line);
		char *tmp3;

		index[n-2]=n-2;
		burst_time[n-2]= atoi(getfield(tmp1,2)); //2nd field
		arrival_time[n-2]= atoi(getfield(tmp2,3)); //3rd field
	    free(tmp1);
	    free(tmp2);
	    
    }
    
    n--;

	Sort(n,arrival_time,burst_time,index);

	for(i=0;i<n;i++)
		{
			index_copy[i]=index[i];
			arrival_copy[i]=arrival_time[i];
			burst_copy[i]=burst_time[i];
	}


	NonPrempt_SJF(n,arrival_time,burst_time,index);
	printf("\n\n");
	Prempt_SJF(n,arrival_copy,burst_copy,index);
	printf("\n\nImprovement of performance of preemptive over non-preemptive SJF scheduling\n");
	printf("In terms of average waiting time: %0.02f %%\n", (sjf_wt-srtf_wt)/sjf_wt*100);
	printf("In terms of turnaround time: %0.02f %%\n", (sjf_tat-srtf_tat)/sjf_tat*100);
	getch();
	return 0;
}

/*
  5
  2 6
  5 2
  1 8
  0 3
  4 4
  */

