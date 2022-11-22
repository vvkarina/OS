#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "parent.h"
#include "errorlib.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/wait.h>

std::vector<std::string> ParentRoutine(const std::vector<std::string> &input)
{
    const char endLine = '\n';
    char readChar;
    std::string str;
    std::vector<std::string> output;
    int fd1[2], fd2[2], fd3[2], errc;
    if (pipe(fd1) == -1 ||
        pipe(fd2) == -1 ||
        pipe(fd3) == -1)
    {
        Oerror("can't create a pipe\n",-1);
    }
    int pid1, pid2;
    if ((pid1 = fork()) == 0)
    {
        if (dup2(fd1[0], STDIN_FILENO) == -1 ||
            dup2(fd3[1], STDOUT_FILENO) == -1 ||
            close(fd1[1]) == -1 ||
            close(fd3[0]) == -1 ||
            close(fd2[1]) == -1 ||
            close(fd2[0]))
        {
            Oerror("Error init a pipe\n",-1);
        }

        if (execlp("./child1", "child1", NULL) == -1)
        {
            Oerror("can't open file child1:\n",-1);
        }
    }
    if (pid1 > 0 && (pid2 = fork()) == 0)
    {
        if (dup2(fd2[1], STDOUT_FILENO) == -1 ||
            dup2(fd3[0], STDIN_FILENO) == -1 ||
            close(fd2[0]) == -1 ||
            close(fd3[1]) == -1 ||
            close(fd1[1]) == -1 ||
            close(fd1[0]) == -1)
        {
            Oerror("Error init a pipe\n",-1);
        }
        if (execlp("./child2", "child2", NULL) == -1)
        {
            Oerror("can't open file child2:\n",-1);
        }
    }
    if (pid1 == -1 || pid2 == -1)
    {
        Oerror("can't create processes child:\n",-1);
    }
    if (pid1 != 0 && pid2 != 0)
    {
        if (close(fd1[0]) == -1 ||
            close(fd2[1]) == -1)
        {
            Oerror("can't close pipe read:\n",-1);
        }
        for (const auto &s : input)
        {
            if (write(fd1[1], s.c_str(), s.size()) == -1)
            {
                Oerror("can't write in pipe 1:\n",-1);
            }
            if (write(fd1[1], &endLine, 1) == -1)
            {
                Oerror("can't write in pipe 1:\n",-1);
            }
            str.clear();
            while ((errc = read(fd2[0], &readChar, 1)))
            {
                if (errc == -1)
                {
                    Oerror("can't read from pipe 2:\n",-1);
                }
                if (readChar == '\n')
                {
                    break;
                }

                str += readChar;
            }

            output.push_back(std::move(str));
        }
        close(fd1[1]);
        close(fd2[0]);
        close(fd3[1]);
        close(fd3[0]);
        wait(NULL);
    }
    return output;
}