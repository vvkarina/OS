#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include "parent.h"
#include "errorlib.h"

std::vector<std::string> ParentRoutine(const std::vector<std::string> &input)
{
    sem_t *sem1, *sem2, *sem3, *sem4;
    char readChar;
    std::string str;
    std::vector<std::string> output;
    int SIZE = 0;
    std::ofstream inFile;
    inFile.open("file1");
    for (const auto &line : input)
    {
        inFile << line << '\n';
        SIZE += line.size() + 1;
    }
    inFile.close();
    unlink("file2");
    unlink("file3");
    int file1 = open("file1", O_RDWR, S_IRUSR);
    int file2 = open("file2", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int file3 = open("file3", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (file1 == -1 || file2 == -1 || file3 == -1)
    {
        Oerror("open error", -1);
    }
    if (ftruncate(file2, SIZE) == -1 || ftruncate(file2, SIZE) == -1 || ftruncate(file3, SIZE) == -1)
    {
        Oerror("ftruncate", -1);
    }
    sem1 = sem_open("1", O_CREAT | O_EXCL, 0777, 0);
    sem2 = sem_open("2", O_CREAT | O_EXCL, 0777, 0);
    sem3 = sem_open("3", O_CREAT | O_EXCL, 0777, 0);
    sem4 = sem_open("4", O_CREAT | O_EXCL, 0777, 0);
    pid_t pid1;
    pid_t pid2;
    pid1 = fork();
    if (pid1 > 0)
    {
        pid2 = fork();
    }
    if (pid1 == 0)
    {
        char *in = (char *)mmap(NULL, SIZE, PROT_READ, MAP_SHARED, file1, 0);
        char *ans = (char *)mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, file3, 0);
        sem1 = sem_open("1", 0);
        sem2 = sem_open("2", 0);
        while (1)
        {
            sem_wait(sem1);
            for (int i = 0; i < SIZE; ++i)
            {
                if (in[i] >= 'A' && in[i] <= 'Z')
                {
                    ans[i] = std::tolower(in[i]);
                }
                else
                {
                    ans[i] = in[i];
                }
            }
            munmap(in, SIZE);
            munmap(ans, SIZE);
            sem_post(sem2);
        }
    }
    if (pid1 > 0 && pid2 == 0)
    {
        char *in = (char *)mmap(NULL, SIZE, PROT_READ, MAP_SHARED, file3, 0);
        char *ans = (char *)mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, file2, 0);
        sem3 = sem_open("3", 0);
        sem4 = sem_open("4", 0);
        while (1)
        {
            sem_wait(sem3);

            for (int i = 0; i < SIZE; ++i)
            {
                if (in[i] == ' ')
                {
                    ans[i] = '_';
                }
                else
                {
                    ans[i] = in[i];
                }
            }
            munmap(in, SIZE);
            munmap(ans, SIZE);
            sem_post(sem4);
        }
    }
    if (pid1 == -1 || pid2 == -1)
    {
        Oerror("can't create processes child:\n", -1);
    }

    if (pid1 != 0 && pid2 != 0)
    {
        char *ans1 = (char *)mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, file2, 0);
        sem1 = sem_open("1", 0);
        sem2 = sem_open("2", 0);
        sem3 = sem_open("3", 0);
        sem4 = sem_open("4", 0);
        sem_post(sem1);
        sem_wait(sem2);
        str.clear();
        sem_post(sem3);
        sem_wait(sem4);

        for (int i = 0; i < SIZE; ++i)
        {
            readChar = ans1[i];
            if (readChar == '\n')
            {
                output.push_back(std::move(str));
                str.clear();
            }
            else
            {
                str += readChar;
            }
        }
        munmap(ans1, SIZE);
        sem_close(sem1);
        sem_close(sem2);
        sem_close(sem3);
        sem_close(sem4);
        sem_unlink("1");
        sem_unlink("2");
        sem_unlink("3");
        sem_unlink("4");
        kill(pid1, SIGKILL);
        kill(pid2, SIGKILL);
    }
    return output;
}