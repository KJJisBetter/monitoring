#include "system_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>


int cpu_data() {
    //TODO: Improve this function to use more accurate CPU statistics. similar to network_data().
    static unsigned long long prev_user = 0, prev_nice = 0, prev_system = 0, prev_idle = 0;
    static unsigned long long prev_iowait = 0, prev_irq = 0, prev_softirq = 0, prev_steal = 0;

    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        printf("Error opening /proc/stat\n");
        return -1;
    }

    char line[256];
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    unsigned long long total_idle, total_system, total_virtual, total;
    double total_cpu_usage;

    if (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu ", 4) == 0) {
            sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu",
                   &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

            unsigned long long prev_total_idle = prev_idle + prev_iowait;
            unsigned long long total_idle = idle + iowait;

            unsigned long long prev_total = (prev_user + prev_nice + prev_system + prev_idle + prev_iowait + prev_irq + prev_softirq + prev_steal);
            unsigned long long total = (user + nice + system + idle + iowait + irq + softirq + steal);

            unsigned long long total_delta = total - prev_total;
            unsigned long long idle_delta = total_idle - prev_total_idle;

            total_cpu_usage = 100.0 * (total_delta - idle_delta) / total_delta;
            printf("Total CPU Usage: %.2f%%\n", total_cpu_usage);

            prev_user = user;
            prev_nice = nice;
            prev_system = system;
            prev_idle = idle;
            prev_iowait = iowait;
            prev_irq = irq;
            prev_softirq = softirq;
            prev_steal = steal;
        }
    }

    fclose(file);

    return (int)total_cpu_usage;
}



int memory_data() {
    //TODO: Improve this function to use more accurate memory statistics. similar to network_data().
    FILE* file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        printf("Error opening /proc/meminfo\n");
        return -1;
    }

    unsigned long memTotal, memFree, memAvailable, buffers, cached;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "MemTotal: %lu kB", &memTotal) == 1) continue;
        if (sscanf(buffer, "MemFree: %lu kB", &memFree) == 1) continue;
        if (sscanf(buffer, "MemAvailable: %lu kB", &memAvailable) == 1) continue;
        if (sscanf(buffer, "Buffers: %lu kB", &buffers) == 1) continue;
        if (sscanf(buffer, "Cached: %lu kB", &cached) == 1) continue;
    }
    fclose(file);

    unsigned long used = memTotal - memFree - buffers - cached;
    double memory_usage = 100.0 * used / memTotal;

    return (int)memory_usage;
}

// ! after making the other 2 funcs copilot knew exactly what I wanted to do next. Even this comment..... scary stuff.
// ! I'm not sure if I should be happy or scared. I'm happy that it's so smart but scared that it's so smart. <-this comment was also generated by copilot.
// ! anyway it got it wrong it wont copile life aint that easy.
int diskio_data(){
    // TODO: Improve this function to use more accurate disk I/O statistics. similar to network_data()
    const char* target_device = "sda";
    FILE* file = fopen("/proc/diskstats", "r");
    if (file == NULL) {
        printf("Error opening /proc/diskstats\n");
        return -1;
    }


    double total_io_seconds = 0.0;
    unsigned long long total_io_ms = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        int major, minor;
        char device[256];
        unsigned long long reads, reads_merged, sectors_read, read_time;
        unsigned long long writes, writes_merged, sectors_written, write_time;
        unsigned long long io_in_progress, io_time, io_time_weighted;


        // ? added more specifiers that it needed?? bruh.
        // gave me what I wanted but not exactly. Some work to be done but 
        // definitely a good start.
        if (sscanf(buffer, "%d %d %s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                   &major, &minor, device,
                   &reads, &reads_merged, &sectors_read, &read_time,
                   &writes, &writes_merged, &sectors_written, &write_time,
                   &io_in_progress, &io_time, &io_time_weighted) >= 14) {
            if (strcmp(device, target_device) == 0) {
                total_io_ms = read_time + write_time;
                total_io_seconds = total_io_ms / 1000.0;

                printf("Total I/O Wait Time: %.3f seconds\n", total_io_seconds);

                break;
            }
        }
    }

    fclose(file);
    return (unsigned long long)total_io_ms;
}


// ! again copilot knew what I wanted to do next. It's like it's reading my mind. <-this comment was also generated by copilot.
// ! scary stuff.....

struct NetworkData {
    unsigned long long bytes_received;
    unsigned long long packets_received;
    unsigned long long bytes_transmitted;
    unsigned long long packets_transmitted;
};



int network_data(int interval){
    struct NetworkData* stats_start = malloc(sizeof(struct NetworkData)); 
    struct NetworkData* stats_end = malloc(sizeof(struct NetworkData));
    if (stats_start == NULL || stats_end == NULL) {
        perror("Error allocating memory");
        return -1;
    }

    FILE* file = fopen("/proc/net/dev", "r");
    if (file == NULL) {
        perror("Error opening /proc/net/dev");
        return -1;
    }

    memset(stats_start, 0, sizeof(struct NetworkData));
    memset(stats_end, 0, sizeof(struct NetworkData));

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {

        if (strstr(buffer, "Inter-") || strstr(buffer, " face")) {
            continue;
        }

        char interface[32];
        unsigned long long bytes_received, packets_received, bytes_transmitted, packets_transmitted;

        // ! copilot seems to struggle with the amount of specifiers needed. Usually adds more than needed.
        if (sscanf(buffer, "%s %llu %llu %llu %llu",
                   interface,
                   &bytes_received, &packets_received,
                   &bytes_transmitted, &packets_transmitted) == 5) {
                    
                    stats_start->bytes_received = bytes_received;
                    stats_start->packets_received = packets_received;
                    stats_start->bytes_transmitted = bytes_transmitted;
                    stats_start->packets_transmitted = packets_transmitted;                
                    break;
        }
        
    }

    fclose(file);

    sleep(interval);

    file = fopen("/proc/net/dev", "r");
    if (file == NULL) {
        perror("Error opening /proc/net/dev");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), file)) {

        if (strstr(buffer, "Inter-") || strstr(buffer, " face")) {
            continue;
        }

        char interface[32];
        unsigned long long bytes_received, packets_received, bytes_transmitted, packets_transmitted;

        if (sscanf(buffer, "%s %llu %llu %llu %llu",
                   interface,
                   &bytes_received, &packets_received,
                   &bytes_transmitted, &packets_transmitted) == 5) {
                    
                    stats_end->bytes_received = bytes_received;
                    stats_end->packets_received = packets_received;
                    stats_end->bytes_transmitted = bytes_transmitted;
                    stats_end->packets_transmitted = packets_transmitted;
                    break;
        }
    }

    fclose(file);

    unsigned long long net_in = stats_end->bytes_received - stats_start->bytes_received;
    unsigned long long net_out = stats_end->bytes_transmitted - stats_start->bytes_transmitted;

    double net_in_kb = net_in / 1024.0 / interval;
    double net_out_kb = net_out / 1024.0 / interval;

    printf("Network In: %.2f KiB/s Network Out: %.2f KiB/s\n", net_in_kb, net_out_kb);


    free(stats_start);
    free(stats_end);

    return net_in_kb + net_out_kb;
}