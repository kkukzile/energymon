

extern "C"{
#include"energymon-odroid.h"
#include<stdio.h>
#include"energymon-time-util.h"
}
#include<iostream>
#include<chrono>


int main(){

static const int IGNORE_INTERRUPT = 0;
static const char* filename = NULL;
static volatile uint64_t running = 1;
static int count = 0;
energymon em;
float power;
energymon_get_odroid(&em);
em.finit(&em);
uint64_t min_interval,energy_last,last_us,exec_us;
uint64_t energy;
float energy_now;
FILE* fout = stdout;
int ret = 0;
min_interval = em.finterval(&em);
if (filename != NULL) 
	fout = fopen(filename, "w");
energy_last = em.fread(&em);
last_us = energymon_gettime_us();
 energymon_sleep_us(min_interval, &IGNORE_INTERRUPT);
	  while (running) {
		if (count) {
		  running--;
		}
energy_now = em.fread(&em);
//std::cout<<energy_now;
		if (fprintf(fout, "\n%f ", energy_now) < 0) {
		  ret = 1;
		  if (filename == NULL) {
			// should never happen writing to stdout
			perror("Writing");
		  } else {
			perror(filename);
		  }
		  break;
		}
		fflush(fout);
		std::cout<<std::chrono::high_resolution_clock::now().time_since_epoch().count();
		if (running) {
		  energymon_sleep_us(min_interval, &IGNORE_INTERRUPT);
		}
	  }
		
	
	
	  // cleanup
	  if (filename != NULL && fclose(fout)) {
		perror(filename);
	  }
	  if (em.ffinish(&em)) {
		perror("em.ffinish");
	  }
	
	  return ret;
}
