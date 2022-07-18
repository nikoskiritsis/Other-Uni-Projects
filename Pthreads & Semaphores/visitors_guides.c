#include <stdio.h>

#include <pthread.h>

#include <semaphore.h>

#include <unistd.h>

sem_t visitor; //visitors semaphore
sem_t guide; //guides semaphore
sem_t akropolis; //site semaphore

int number_of_visitors=0; //counter for number of visitors
int number_of_guides=0; // counter for number of guides

void* Guide(void* arg){ //guide function
long tid;               //thread id
tid = (long)arg;

	sem_wait(&guide); 
	number_of_guides++;
	printf("\nGuide #%ld entered akropolis...\n", tid);
	sem_post(&guide);
	sem_post(&akropolis);
	sem_post(&akropolis);
	sem_post(&akropolis);
	int i=0;
	while(i==0){     //while loop to keep guide in site if there's not enough guides yet
	sleep(7);
	sem_wait(&guide);
	sem_wait(&visitor);
	if((number_of_guides-1)*3>=number_of_visitors){
		printf("\nGuide #%ld is exiting akropolis...\n", tid);
		number_of_guides--;
		i=1;}
	sem_post(&guide);
	sem_post(&visitor);

	}
}

void* Visitor(void* arg){ //visitor function
long tid;
tid = (long)arg;          //thread id
	sem_wait(&akropolis);
	sem_wait(&visitor);
	if(3*number_of_guides>=(number_of_visitors+1))     //condition for visitor to enter the site
	{
	number_of_visitors++;
	printf("\nVisitor #%ld entered akropolis...\n", tid);
	sem_post(&visitor);
	sleep(5);
	sem_wait(&visitor);
	number_of_visitors--;                 //visitor can get out of the site whenever he wants,no condition required
	printf("\nVisitor #%ld is exiting akropolis ...\n", tid);
	}
	sem_post(&akropolis);
	sem_post(&visitor);
}

void* Inspector(void* arg){                         //inspector function
	sem_wait(&visitor);
	sem_wait(&guide);
	if(number_of_guides*3>=number_of_visitors){                                   //inspector checking ratio
		printf("\nEverything is ok,move along\n");}
	else printf("\nSomething goes wrong\n");
	sem_post(&guide);
	sem_post(&visitor);
}

int main(){

sem_init(&akropolis,0,0);  //initialize semaphores
sem_init(&visitor,0,1);
sem_init(&guide,0,1);
pthread_t v[30], g[3],i1,i2;
pthread_create(&g[0],NULL,Guide, (void *)1);
sleep(1);
pthread_create(&v[0],NULL,Visitor, (void *)1);
sleep(1);
pthread_create(&v[1],NULL,Visitor, (void *)2);
sleep(1);
pthread_create(&v[2],NULL,Visitor, (void *)3);
sleep(1);
pthread_create(&v[3],NULL,Visitor, (void *)4);
sleep(1);
pthread_create(&g[1],NULL,Guide, (void *)2);
pthread_create(&v[4],NULL,Visitor, (void *)5);



sleep(1);
pthread_create(&v[5],NULL,Visitor, (void *)6);
sleep(1);
pthread_create(&v[6],NULL,Visitor, (void *)7);
sleep(1);
pthread_create(&v[7],NULL,Visitor, (void *)8);
pthread_create(&i2,NULL,Inspector,NULL);
sleep(1);
pthread_create(&v[8],NULL,Visitor, (void *)9);
sleep(1);
pthread_create(&g[2],NULL,Guide, (void *)3);
pthread_create(&v[9],NULL,Visitor, (void *)10);
sleep(1);

int j;
for (j=10; j<30; j++) {
pthread_create(&v[j],NULL,Visitor, (void *)(int)(j+1));
sleep(1);
}



pthread_create(&i1,NULL,Inspector,NULL);     //create 2nd inspector semaphore and then use pthread_join to make sure the threads will be terminated
pthread_join(g[0],NULL);
pthread_join(v[0],NULL);
pthread_join(v[1],NULL);
pthread_join(v[2],NULL);
pthread_join(v[3],NULL);
pthread_join(g[1],NULL);
pthread_join(v[4],NULL);
pthread_join(v[5],NULL);
pthread_join(v[6],NULL);
pthread_join(v[7],NULL);
pthread_join(v[8],NULL);
pthread_join(g[2],NULL);
pthread_join(v[9],NULL);
pthread_join(i2,NULL);
int k;
for (k=10; k<30; k++) {
pthread_join(v[k],NULL);
}
pthread_join(i1,NULL);

sem_destroy(&akropolis);
sem_destroy(&visitor);
sem_destroy(&guide);
return 0;
}
