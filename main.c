#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void show_memory_info() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    char line[256];
    long memTotal = 0, memFree = 0, swapTotal = 0, swapFree = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %ld kB", &memTotal)) continue;
        if (sscanf(line, "MemFree: %ld kB", &memFree)) continue;
        if (sscanf(line, "SwapTotal: %ld kB", &swapTotal)) continue;
        if (sscanf(line, "SwapFree: %ld kB", &swapFree)) continue;
    }
    fclose(fp);

    long memUsed = memTotal - memFree;
    long swapUsed = swapTotal - swapFree;

    printf("Memoria total instalada: %ld MB\n", memTotal / 1024);
    printf("Memoria usada: %ld MB\n", memUsed / 1024);
    printf("Swap usada: %ld MB\n\n", swapUsed / 1024);
}

void show_cpu_info() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) return;

    char line[256];
    char model[256] = {0};
    int cores = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0 && model[0] == 0) {
            sscanf(line, "model name\t: %[^\n]", model);
        }
        if (strncmp(line, "processor", 9) == 0) {
            cores++;
        }
    }
    fclose(fp);

    printf("Procesador: %s\n", model);
    printf("Número de cores: %d\n\n", cores);
}

void show_cpu_load() {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return;

    char line[256];
    int cpu_id = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) == 0 && line[3] != ' ') {
            long user, nice, system, idle, iowait, irq, softirq, steal;
            sscanf(line, "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld",
                   &cpu_id, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
            long total = user + nice + system + idle + iowait + irq + softirq + steal;
            long busy = total - idle - iowait;

            double load = (double) busy / (double) total * 100.0;
            printf("Carga CPU core %d: %.2f%%\n", cpu_id, load);
        }
    }
    fclose(fp);
    printf("\n");
}

int main() {
    while (1) {
        system("clear");
        show_memory_info();
        show_cpu_info();
        show_cpu_load();
        sleep(2);
    }
    return 0;
}
