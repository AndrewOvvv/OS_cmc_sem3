#include <stdio.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


enum { MAX_BUF = 10000 };


int
main(int argc, char *argv[])
{
    char BUF_FILE[MAX_BUF] = {0};
    double (*f)(double);

    char *prefix_file = "#include<math.h>\ndouble\nf(double x)\n{\nreturn ";
    char *suffix_file = ";\n}\n";
    sprintf(BUF_FILE, "%s%s%s", prefix_file, argv[4], suffix_file);

    int fd = open("/tmp/myf.c", O_CREAT | O_RDWR, 0666);
    write(fd, BUF_FILE, strlen(BUF_FILE));
    close(fd);

    if (!fork()) {
        execlp("gcc", "gcc", "-m32", "-c", "-fPIC", "/tmp/myf.c",  "-o", "/tmp/myf.o", "-lm", NULL);
        _exit(1);
    }
    wait(NULL);
    //printf("OKOKOKOKOKOKOKOK ----------------- OKOKOKOKOOOKOKO\n");
    if (!fork()) {
        execlp("gcc", "gcc", "-m32", "-shared", "/tmp/myf.o", "-o", "/tmp/myflib.so", "-lm", NULL); 
        _exit(1);
    }
    wait(NULL);
    //printf("OKOKOKOKOKOKOKOK ----------------- OKOKOKOKOOOKOKO\n");
    
    void *dlib = dlopen("/tmp/myflib.so", RTLD_LAZY);
    f = dlsym(dlib, "f"); 

    double left = strtod(argv[1], NULL);
    double right = strtod(argv[2], NULL);
    long long n = strtoll(argv[3], NULL, 10);

    double dx = (right - left) / (double)n;
    double sum = 0, x;
    for (int i = 0; i < n; i++) {
        x = left + i * dx;
        sum += (*f)(x) * dx;
    }
    printf("%.10g", sum);
    dlclose(dlib);

    if (!fork()) {
        execlp("rm", "rm", "/tmp/myf.c", NULL);
    }
    wait(NULL);
    if (!fork()) {
        execlp("rm", "rm", "/tmp/myf.o", NULL);
    }
    wait(NULL);
    if (!fork()) {
        execlp("rm", "rm", "/tmp/myflib.so", NULL);
    }
    wait(NULL);
    return 0;
}
