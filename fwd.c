#include <linux/input.h>
#include <sys/time.h>
#include <inttypes.h>
#include <stdio.h>

struct __attribute__((__packed__)) smaller {
    int16_t type; 
    int16_t code; 
    int32_t value;
    unsigned char done;
};

long gettime() {
    struct timeval val;
    gettimeofday(&val, NULL);
    return (val.tv_sec * 1000000) + val.tv_usec;
}

int main() {
    struct input_event in; 
    struct smaller out;
    FILE *ev;
    ev = fopen("/dev/input/event0", "r");
    FILE *ar;
    ar = fopen("/dev/ttyACM0", "w+");
    if (ev == NULL || ar == NULL) {
        perror("open");
        return 1;
    }

    long was, now;
    was = gettime();
    now = gettime();
    while(1) {
        fread(&in, sizeof(struct input_event), 1, ev);    
	if ((now - was) > (10 * 1000)) {
	    out.type = in.type;
	    out.code = in.code;
	    out.value = in.value;
	    out.done = '\n';
	    if (out.type != 0 && out.code != 0 && out.value != 0)
	        fwrite(&out, sizeof(struct smaller), 1, ar);
            printf("%d, %d, %d\n", out.type, out.code, out.value);
            was = gettime();
	    now = gettime();
        } else {
            now = gettime();
        }
    }
}
