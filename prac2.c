#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


long animals;
long food;
long hunters;
long cooks;


pthread_mutex_t anim_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t food_mutex = PTHREAD_MUTEX_INITIALIZER;


int throw()
{
    return (rand() % 6 + 1);
}

int change_food_state(int action)
{
	int res = 0;
	
	pthread_mutex_lock(&food_mutex);
	
	if (action == -1) printf("Osadnik idzie cos zjesc ^^ (zostalo tylko %ld porcji!)\n", food);

	food+=action;
	
	if (food < 0)
	{
		res = 1;
		food = 0;
	}
	
	pthread_mutex_unlock(&food_mutex);
	
	return res;
}


void change_anim_state(int action)
{
	pthread_mutex_lock(&anim_mutex);
	
	if (action)
	{
		animals++;
		printf("Mysliwy upolowal jakies?? zwierze\n");
	}
		
	
	else
	{
		if (animals > 0)
		{
			int temp = throw();
			change_food_state(temp);
			animals--;
			printf("Ktos wlasnie gotuje... %d jedzonka, zostalo %ld zwierzyny\n", temp, animals);
		}
	}
	
	pthread_mutex_unlock(&anim_mutex);
}


void hunt()
{
    if (throw() > throw()) change_anim_state(1);
}

void* hunter_day( void *arg )
{
	int i;
    for (i=0; i < 365; i++)
    {
        hunt();
        if (change_food_state(-1))
        {
        	hunters--;
        	printf("Mysliwy opuscil osade %d dnia\n", i);
        	i = 365;
        	break;
        }
        usleep(10000);
    }

    pthread_exit(NULL);
}


void* cook_day( void *arg )
{
	int i;
    for (i=0; i < 365; i++)
    {
        change_anim_state(0);
        if (change_food_state(-1))
        {
        	cooks--;
        	printf("Kucharz opuscil osade %d dnia\n", i);
        	i = 365;
        	break;
        }
        usleep(10000);
    }

    pthread_exit(NULL);
}


int main(int argc, char** argv)
{

    srand(time(NULL));

    hunters = strtol(argv[1], NULL, 10);
    cooks = strtol(argv[2], NULL, 10);
    animals = strtol(argv[3], NULL, 10);
    food = strtol(argv[4], NULL, 10);//*/

	long temp_hunters = hunters;
	long temp_cooks = cooks;

    printf("l. mysliwych: %ld\n", hunters);
    printf("l. kucharzy: %ld\n\n", cooks);
    
    pthread_t hunt_threads[hunters];
    pthread_t cook_threads[cooks];
    
    
    int i;

    for (i=0; i < temp_hunters; i++)
        if (pthread_create (&hunt_threads[i], NULL, hunter_day, NULL) )
        {
            printf("Blad przy tworeniu hunt_thread%d\n", i);
            return 1;
        }

    for (i=0; i < temp_cooks; i++)
        if (pthread_create (&cook_threads[i], NULL, cook_day, NULL) )
        {
            printf("Blad przy tworeniu cook_thread%d\n", i);
            return 1;
        }



    for (i=0; i < temp_hunters; i++)
        if (pthread_join (hunt_threads[i], NULL) )
        {
            printf("Blad przy konczeniu hunt_thread%d\n", i);
            return 1;
        }

    for (i=0; i < temp_cooks; i++)
        if (pthread_join (cook_threads[i], NULL) )
        {
            printf("Blad przy konczeniu cook_thread%d\n", i);
            return 1;
        }


	printf("\nMinal rok, a w wiosce pozostalo %ld kucharzy oraz %ld mysliwych\n", cooks, hunters);

    return 0;
}
