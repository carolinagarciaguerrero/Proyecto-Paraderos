#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


double collect_rssi(char *device, int samples, unsigned int delta_time, char *save_file) {

    int rssi_values[samples];
    int offset = 162 + strlen(device) + 14;    // offset to rssi value
    char line[3];    // read line buffer

    double sleep_time = samples * delta_time * 1e-6;
    clock_t t = clock();

    for (int i=0; i<samples; i++) {

        FILE *rssi_file = fopen("/proc/net/wireless", "r");
        fseek(rssi_file, offset, SEEK_SET);

        fread(line, 1, 3, rssi_file);    // get rssi value
        int rssi = atoi(line);
        rssi_values[i] = rssi;

        fclose(rssi_file);
        usleep(delta_time);
    }

    t = clock() - t;
    double elapsed_time = ((double)t) / CLOCKS_PER_SEC + sleep_time;

    FILE *f = fopen(save_file, "wb");
    fwrite(rssi_values, sizeof(int), samples, f);
    fclose(f);

    return elapsed_time;
}


int main(int argc, char **argv) {
	
    char *dev = argv[1];	
    char *save_file = argv[2];
    int samples = atoi(argv[3]);
    int sample_time = atoi(argv[4]);
    	
    double elapsed_time = collect_rssi(dev, samples, sample_time, save_file);
    printf("Elapsed time: %f s", elapsed_time);
    printf("\n");

    return 0;
}
