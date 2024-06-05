#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "system_data.h"

//this was mostly for testing purposes
int main(int argc, char *argv[]){
    if(argc != 2 || atoi(argv[1]) <= 0){
        printf("Usage: %s interval\n", argv[0]);
        return 1;
    }

    int interval = atoi(argv[1]);


        while (true){
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);

            char time_str[100];
            strftime(time_str, sizeof(time_str), "%H:%M:%S", local_time);
            printf("Current time: %s\n", time_str);

            cpu_data();
            memory_data();
            diskio_data();
            double network_data(int interval, struct NetworkUsage* usage); 

            sleep(interval);
        }
   
}