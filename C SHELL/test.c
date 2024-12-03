#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdlib.h>

pid_t shell_pgid;         // Shell's process group ID
struct termios shell_tmodes; // Terminal modes
int shell_terminal;       // Terminal file descriptor
int shell_is_interactive; // Whether the shell is running interactively

// Initialize the shell's process group and take control of the terminal
void initialize_shell() {
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive) {
        // Loop until we're in the foreground
        while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp())) {
            kill(-shell_pgid, SIGTTIN);
        }

        // Ignore job control signals
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);

        // Put the shell in its own process group
        shell_pgid = getpid();
        if (setpgid(shell_pgid, shell_pgid) < 0) {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }

        // Take control of the terminal
        tcsetpgrp(shell_terminal, shell_pgid);

        // Save the terminal's modes for later restoration
        tcgetattr(shell_terminal, &shell_tmodes);
    }
}

// Function to bring a process to the foreground and give it terminal control
void bring_to_foreground(pid_t pid) {
    // Give terminal control to the process
    tcsetpgrp(shell_terminal, pid);

    // Wait for the process to finish or stop
    int status;
    waitpid(pid, &status, WUNTRACED);

    // After the process is done, give terminal control back to the shell
    tcsetpgrp(shell_terminal, shell_pgid);

    // Check if the process stopped
    if (WIFSTOPPED(status)) {
        printf("Process [%d] stopped\n", pid);
    }
}

// Example main function to simulate running a command in the foreground
int main() {
    // Initialize shell to control the terminal
    initialize_shell();

    // Simulate creating a child process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: simulate a command (sleep for 5 seconds)
        execlp("sleep", "sleep", "5", NULL);
    } else {
        // Parent process: bring the child process to the foreground
        bring_to_foreground(pid);

        // After the foreground process completes, the shell regains control
        printf("Shell regained control\n");
    }

    return 0;
}
