#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>

int main() {
    int parent_pipe[2];
    int child_pipe[2];
    char buff[50];

    if(pipe(parent_pipe) || pipe(child_pipe)) {
        perror("pipe(...)");
        exit(1);
    }

    // As noted elsewhere, you're using `fork()` incorrectly.
    // `fork()` returns 0 to the child, and a pid to the parent, or -1 if an error
    // occurs.
    int pid = fork();
    if (pid == -1) {
        perror("fork()");
        exit(1);
    }

    if (pid == 0) {
        // this is the child process.  read from child_pipe, write to parent_pipe
        const char child[]="Child Writes. Parent Reads\n";
        int in, out;
        in = child_pipe[0];
        // in = parent_pipe[0];  // uncomment me to test with one pipe pair
        out = parent_pipe[1];
        int i;
        for (i = 0; i < 10; ++i) {
            read(in,buff,50);
            printf("Parent: %s",buff);
            // NOTE: `strlen(child)` doesn't include the nul at the end!
            write(out, child, strlen(child) + 1);
        }
    }
    else {
        // this is the parent process
        const char parent[]="Parent Writes. Child Reads\n";
        int in, out;
        in = parent_pipe[0];
        out = child_pipe[1];
        // out = parent_pipe[1];  // uncomment me to test with one pipe pair
        int i;
        for ( i = 0; i < 10; ++i) {
            write(out, parent, strlen(parent) + 1);
            read(in, buff, 50);
            printf("Child: %s", buff);
        }
    }
}