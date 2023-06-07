#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>


enum { MAX_SYM = 100 };


typedef struct 
{
    int msg_type;
    char data[MAX_SYM];
} Message;


unsigned long long
ArgToLL(char *arg)
{
    return strtoll(arg, NULL, 10);
}


int
main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    long long key = ArgToLL(argv[1]);
    int cnt_proc = ArgToLL(argv[2]);
    unsigned long long value1 = ArgToLL(argv[3]), 
                    value2 = ArgToLL(argv[4]), 
                    max_value = ArgToLL(argv[5]);

    int *sons_pid = calloc(cnt_proc, sizeof(*sons_pid));
    int msg_id = msgget(key, 0666 | IPC_CREAT);

    Message current_msg;


    for (int i = 0; i < cnt_proc; ++i) {
        if (!(sons_pid[i] = fork())) {
            while (1) {
                unsigned long long x1, x2, x3;
                msgrcv(msg_id, (struct msgbuf*) (&current_msg), MAX_SYM, i + 1, 0);
                sscanf(current_msg.data, "%llu", &x1);
                msgrcv(msg_id, (struct msgbuf*) (&current_msg), MAX_SYM, i + 1, 0);
                sscanf(current_msg.data, "%llu", &x2);
                x3 = x1 + x2;
                printf("%d %llu\n", i, x3);
                if (x3 > max_value) {
                    current_msg.msg_type = cnt_proc + 1;
                    msgsnd(msg_id, (struct msgbuf*) (&current_msg), strlen(current_msg.data) + 1, 0);
                } else {
                    current_msg.msg_type = x3 % cnt_proc + 1;
                    sprintf(current_msg.data, "%llu", x2);
                    msgsnd(msg_id, (struct msgbuf*) (&current_msg), strlen(current_msg.data) + 1, 0);
                    sprintf(current_msg.data, "%llu", x3);
                    msgsnd(msg_id, (struct msgbuf*) (&current_msg), strlen(current_msg.data) + 1, 0);
                }
            }
            _exit(0);
        } else if (sons_pid[i] < 0) {
            for (int j = 0; j < i; ++j) {
                kill(sons_pid[j], SIGKILL);
                wait(NULL);
            }
            free(sons_pid);
            msgctl(msg_id, IPC_RMID, NULL);
            exit(1);
        }
    }

    // send message
    current_msg.msg_type = 1;
    sprintf(current_msg.data, "%llu", value1);
    msgsnd(msg_id, (struct msgbuf*) (&current_msg), strlen(current_msg.data) + 1, 0);
    sprintf(current_msg.data, "%llu", value2);
    msgsnd(msg_id, (struct msgbuf*) (&current_msg), strlen(current_msg.data) + 1, 0);
    //

    // wait finish message
    msgrcv(msg_id, (struct msgbuf*) (&current_msg), MAX_SYM, cnt_proc + 1, 0);
    //

    for (int i = 0; i < cnt_proc; ++i) {
        if (sons_pid[i] != 0) {
            kill(sons_pid[i], SIGKILL);
            wait(NULL);
        }
    }

    free(sons_pid);
    msgctl(msg_id, IPC_RMID, NULL);
    
    return 0;
}
