#include <stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int max=20;        // max number that the tray can hold
int pizza_no=4;   //no of pizza in the tray
pthread_mutex_t lock;
pthread_cond_t Tray_Not_Full=PTHREAD_COND_INITIALIZER;  //Signal there is at least one space in the tray
pthread_cond_t Tray_Not_Empty=PTHREAD_COND_INITIALIZER;//signal that there is at least one pizza in the tray 

void* cook(void *arg){

 for(;;) {
     pthread_mutex_lock(&lock);
    if (pizza_no==max){
        printf("\nTray is full cannot produce\n");
        pthread_cond_wait(&Tray_Not_Full,&lock);  //waiting for the signal that the tray is not full 

    }
    sleep(2);
    pizza_no= pizza_no + 1;
    printf("\n Produced a pizza: Remaining :%d\n", pizza_no);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&Tray_Not_Empty);  // sending the signal that there is a pizza to be eaten   
}
    return NULL;

}

void* buy(void *arg)
{
  for(;;) {
    pthread_mutex_lock(&lock);

    if(pizza_no<1){ 
      printf("\nThere is no pizza left. Wait till they have been produced\n");
       pthread_cond_wait(&Tray_Not_Empty,&lock);
        }                       
    pizza_no=pizza_no-1;
    printf("\n Client ate 1 pizza: Remaining: %d\n", pizza_no);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&Tray_Not_Full); 
   }

    return NULL;
}
int main(void) {

pthread_t tid[2];
pthread_mutex_init(&lock, NULL);

pthread_create(&(tid[0]), NULL, &cook, NULL);
pthread_create(&(tid[1]), NULL, &buy, NULL);

pthread_join(tid[0], NULL);
pthread_join(tid[1], NULL);

  return 0;
}
