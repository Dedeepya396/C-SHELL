# C Shell – Custom Unix Shell in C

This is a custom-built shell implemented in C that supports a variety of Unix-like command line features along with additional custom commands and behaviors.

---

## Features

## Command Execution

- Supports running system and custom commands.
- Allows running multiple commands separated by `;` or `&`.
- Foreground and background execution is supported.
- If a foreground command takes more than 2 seconds, the shell logs its details and adjusts the prompt based on a flag stored in a file.


## Directory Navigation (`hop` command for `cd`)

- Supports changing directories using relative (`.` `..`) and absolute paths.
- Recognizes `~` as home directory and `-` for previous directory.
- Automatically determines whether a path is relative or absolute and changes accordingly.


## File and Directory Listing (`reveal` command for `ls`)

- Lists files, directories, and executables in the current directory.
- Can show hidden files using a flag.
- Can display permissions and types in a detailed format.
- Results are sorted in lexicographical order.


## Command Logging (`log`)

- Maintains a history of the last 15 commands entered.
- Ignores commands that are duplicates of the most recent one or include the word `log`.
- Allows viewing the logged commands.
- Supports clearing the log when needed.


## Process Information (`proclore` command)

- Displays detailed information about processes.
- Without arguments, it shows details about the current shell.
- With a process ID, it displays information about that specific process.


## File Search (`seek` command)

- Searches through a given directory path for:
  - Directories
  - Files
  - Executables
- Supports flags to customize which types of entries to count and display.
- Requires paths to start with `/` and not `./`.


## Prompt Display

- Displays the current working directory in the shell prompt.
- If the previous command took more than 2 seconds, the prompt includes additional info from a log file.


## .myshrc Support

- On shell startup, `.myshrc` is read to configure aliases and user-defined functions.
- **Aliases** (e.g., `reveall`, `home`) are recognized by identifying lines starting with `alias`.
- **Functions** (e.g., `mk_hop`, `hop_seek`) are identified using the keyword `func`.


## Input/Output Redirection

- Supports standard I/O redirection (`<`, `>`, `>>`) by manipulating file descriptors.
- After redirection is set up, commands are executed as system or user-defined.


## Piping

- Supports Unix-style piping (`|`) between commands.
- Connects output of one command to the input of the next.

### Combined: Pipes + Redirection

- Handles cases where both redirection and piping are present.


## Activities Tracker

- Tracks background and foreground processes.
- Records process details like PID and execution mode.
- Details are shown when asked


## Signal Handling

- Supports sending signals to processes using PID and signal number.
- Implements handlers for:
  - `Ctrl+Z` (SIGTSTP): Suspends the current foreground process via a child shell process to avoid suspending the shell itself.
  - `Ctrl+C` (SIGINT): Clean handling to avoid shell termination.


## Neonate

- Periodically prints newly created child PIDs at a specified time interval and stops when `x`is pressed.
- Uses keyboard listener to stop the output when the key `x` is pressed.
- Key presses are checked before and after each interval.


## iMan – Internet Manual Pages

- Connects to a remote server to fetch online manpages using HTTP.
- Sends a GET request built from a known manpage URL pattern.
- Parses the HTTP response:
  - Assumes the header ends with `\n\n`.
  - Extracts the body and removes HTML tags (`<...>`) to display only the content.
- Provides a clean, text-only view of online manual entries.

---
##  Build & Run

### Compile
```bash
make
