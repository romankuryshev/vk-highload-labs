#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
    key_t key = ftok("homework_key", 65);
    int shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    int *counter = (int*) shmat(shmid, NULL, 0);
    if (counter == (void*) -1) {
        perror("shmat");
        exit(1);
    }

    struct shmid_ds buf;
    shmctl(shmid, IPC_STAT, &buf);
    if (buf.shm_nattch == 1 && *counter < 0) {
        *counter = 0;
    }

    int value = *counter;
    value++;
    *counter = value;

    printf("Current counter value: %d\n", value);
    sleep(60);
    shmdt(counter);

    return 0;
}
