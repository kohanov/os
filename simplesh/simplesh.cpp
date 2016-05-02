#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;
int fd0 = 0, fd1 = 1;
bool first = false;//indicates, was first command executed or not
vector<int> pids;

void newProc(vector<char*> args, bool end) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        fprintf(stderr, "error\n");
        exit(1);
    }
    pid_t cpid = fork();
    int status;
    if (cpid != 0) {//parent
        if (end) {
            close(pipefd[0]);
            close(pipefd[1]);
        }
        if (first) {
            close(fd0);
            close(fd1);
        }
        pids.push_back(cpid);
        waitpid(cpid, &status, 0);
    } else {//child
        if (first) {
            dup2(fd0, 0);
            close(fd0);
            close(fd1);
        }
        if (!end) {
            dup2(pipefd[1], 1);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        args.push_back(NULL);
        if (execvp(args[0], static_cast<char * const *>(args.data())) == -1) {
            fprintf(stderr, "Execvp error\n");
        }
    }
    fd0 = pipefd[0];
    fd1 = pipefd[1];
    first = true;
}

void sigint_handler(int signal) {
    fprintf(stderr, "Signal %d came for %d programms\n", signal, (int) pids.size());
    for (int i = 0; i < pids.size(); i++) {
        kill(pids[i], signal);
    }
}

int main(int argc, char** argv) {
    struct sigaction action;
    action.sa_handler = &sigint_handler;
    sigaction(SIGINT, &action, NULL);
    printf("$\n");
    int bufferSize = 1024;
    vector<char> buffer(bufferSize);
    int readBytes = 0;
    string args;
    vector<char*> arguments(0);
    while((readBytes = read(0, buffer.data(), bufferSize)) > 0) {
        for (int i = 0; i < readBytes; i++) {
            if (buffer[i] == '|' || buffer[i] == (char)13 || i == readBytes - 1) {//end of subcommand
                if (args != "") {
                    arguments.push_back(strdup(args.c_str()));
                }                
                newProc(arguments, (i + 1 == readBytes));
                if (i == readBytes - 1) {
                    printf("$\n");
                    first = false;
                    pids.clear();
                }
                arguments.clear();
                args = "";
                i++;
            } else if (buffer[i] == ' ') {
                if (args != "") {
                    arguments.push_back(strdup(args.c_str()));
                }
                args = "";
            } else {//another symbol
                args += buffer[i];
            }
        }
    }
    return 0;
}
