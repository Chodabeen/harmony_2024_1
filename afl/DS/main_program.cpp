#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
#include <glob.h>
#include <signal.h>

void sig_handle(int signum)
{
    printf("signal occurred\n");
}

void check_sanitizer_log(const char* log_pattern) {
    glob_t glob_result;
    glob(log_pattern, GLOB_TILDE, NULL, &glob_result);

    if (glob_result.gl_pathc > 0) {
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            std::cout << "Sanitizer log file found: " << glob_result.gl_pathv[i] << std::endl;
        }
    } else {
        std::cout << "Sanitizer log file not found." << std::endl;
    }

    globfree(&glob_result);
}

int main() {

    signal(SIGSEGV, sig_handle);
    pid_t pid = fork();
    const char* log_pattern = "./asan.log.*";

    if (pid == 0) {
        // Child process
        if (setenv("ASAN_OPTIONS", "log_path=./asan.log:verbosity=1", 1) != 0) {
            perror("setenv");
            exit(EXIT_FAILURE);
        }
        // Execute the program to be monitored
        execl("./your_program", "./your_program", nullptr);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status))
        {
            printf("success exit\n");
        }
        if(WIFSIGNALED(status))
        {
            printf("signal\n");
        }
        // Add a small delay to ensure log file is written
        sleep(1);
        check_sanitizer_log(log_pattern);
    } else {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
