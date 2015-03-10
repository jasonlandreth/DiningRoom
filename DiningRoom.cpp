
/*****
*
*	Created by Jason Landreth
*
*****/
#include "DiningRoom.h"

bool DiningRoom::test(int phil){
  if(phil_state[phil] == HUNGRY)
    {
      if(phil_state[left_neighbor(phil)] != EATING && phil_state[right_neighbor(phil)] != EATING)
		{
			phil_state[phil] = EATING;
			display_states();
			return true;
		}
    }
      else
		return false;
}
void DiningRoom::run_simulation(){
  display_headings();
  display_states();
	for (int i = 0; i < num_phils; i++){ 
		pthread_create(&phil_threads[i], NULL, start_philosopher, &args_array[i]); 
	}
	for (int i = 0; i < num_phils; i++){ 
		pthread_join(phil_threads[i], NULL); 
	}
}
void DiningRoom::grab_forks(int phil){
  bool temp;
  
  pthread_mutex_lock(&table_lock);
  phil_state[phil] = HUNGRY; // HUNGRY = 1
  display_states();
	if(!test(phil)){ 
		pthread_cond_wait(&safe_to_eat[phil], &table_lock); 
	}
  pthread_mutex_unlock(&table_lock);
 }
 
 
 
void DiningRoom::release_forks(int phil)
{
    pthread_mutex_lock(&table_lock);
  phil_state[phil] = THINKING;  //THINKING = 0
  display_states();
  int left = left_neighbor(phil);
  int right = right_neighbor(phil);
	if(test(left) == HUNGRY){ 
		pthread_cond_signal(&safe_to_eat[left_neighbor(phil)]);
	}
	if(test(right) == HUNGRY){ 
	pthread_cond_signal(&safe_to_eat[right_neighbor(phil)]); 
	}

	 pthread_mutex_unlock(&table_lock);
}