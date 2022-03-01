/*kunaljoshi1Check out this Author's contributed articles., et al. 
“Sleeping Barber Problem in Process Synchronization.” GeeksforGeeks, 
14 Aug. 2019, www.geeksforgeeks.org/sleeping-barber-problem-in-process-synchronization/.

--I used this swebsite to understand the problem and how the semaphores are used in the code*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define waitTime 4
#define closingTime 25
#define max_clients 10


void *barberChair(void *param);
void *customer(void *param);

sem_t chairs;//semaphore for chairs in waiting room
sem_t client;//semaphore for waiting customers
sem_t barber;
int nchairs; //number of chairs given by user
int nclients; // number of clients given by user
int queue[max_clients]; //FIFO queue used to prevent starvation
int pos=0;

int main(int argc, char *argv[]) {
	//declaration of threads
	pthread_t barber;
	pthread_t clients[max_clients];
	int threadnum[max_clients];

	//checks if correct arguents are sent
	if (argc != 3){
	   printf("Please enter 2 arguments: (number of clients) (number of chairs)\n");
	   exit(0);
   	}

   	nclients=atoi(argv[1]);
   	nchairs=atoi(argv[2]);

   	//checks if number of clients given is <= maximum allowed number of clients 
   	if(nclients > max_clients){
   		printf("Only %d maximum clients allowed\n", max_clients );
   		exit(0);
   	}

   	//populates array with the specific number for each thread
   	for (int i = 0; i < max_clients; i++) {
      threadnum[i] = i;
  	}

  	//initializes semaphores
   	sem_init(&chairs,0,1);
   	sem_init(&client,0,0);
   	sem_init(&barber,0,0);

   	pthread_create(&barber, NULL, barberChair, NULL);//creates the barber thread 

   	for (int a = 0; a < nclients; a++){//creates all instances of client threads 
	   pthread_create(&clients[a], NULL, customer, (void *)&threadnum[a]);
   }

   sleep(closingTime);//initial tine taken until the barber shop is closed


   printf("The barber is going home now!\n");
   return(0);
 }

void *barberChair(void *param) {
  
	while(1) {
		sem_wait(&client); ///barber is asleep, waiting for client to wake him


		sem_wait(&chairs); //barber is awake, locking to modify number of chairs available

		++nchairs; //increments the available

		sem_post(&barber);//barber has cleint in chair

		sem_post(&chairs);//number of avalable chairs have been changed succesfully

		sleep(waitTime);//time required to cut hair
	} 
}

void *customer(void *param) {
   	int cid = *(int *)param;

   	printf("Customer %d heading towards the barber shop\n", cid);
   	sleep(1);
	printf("Customer %d has arrived at the barber shop\n", cid);
	
	sem_wait(&chairs);//locking to modify numbe rof chairs
	if (nchairs > 0){//checka if there are any avaible chairs
		--nchairs;//decrements if available
		queue[pos]=cid;//popoulates are with the threads thata rrive in a FIFO order
		++pos;
		while (1){
			if(queue[0] == cid){//checks if this is the correct thread to be executed next in the queue
				printf("Customer %d entering wait room.\n",cid);
				sem_post(&client);//wake barber
	        	printf("Customer %d is waking up the barber.\n",cid);

	        	sem_post(&chairs);//wake barber

			   	sem_wait(&barber);//waiting until barber is ready
			   	printf("The barber is cutting hair.\n");
			   	printf("The barber has finished cutting hair\n");
			   	printf("Customer %d is leaving barber shop.\n",cid);
			   	sleep(waitTime);//time required to cut hair 

			   	for (int i =0; i<(nclients-1); i++){//once a hair is cut, that thread is out of queue and the others move up a space 
	      			queue[i] = queue[i+1];
	    		}
			   	break;

		   	}
		}
	}else{
		printf("Customer %d has left, no chairs available\n",cid);//leave if there are no available chairs
		sem_post(&chairs);
	}
}