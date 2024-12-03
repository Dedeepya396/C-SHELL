
## .myshrc
I have done both aliases part and functions part, I am implementing aliases by comparing whether the line contains alias or not and for functions I am doing by comparing the line and `func` word

Aliases I implemented are `reveall`, `home`. Functions I have implemented are `mk_hop`, `hop_seek`.

## Redirection
I am changing file descriptors according to the redirection present in the command, after that I am executing the command checking whether they are user commands or system commands, for system commands I am checking a flag if it is 0 then I will run the command in background process else in foreground process

## Pipes

I am connecting input and outputs of one pipes to another properly and executing command. 


## Pipes and redirection

This code is same as the pipes one but the difference is I call the function for redirection instead of exec

## Activities
I have created a struct `Activities` and I created an array of structs which is a global variable. Whenever I am creating a process, I am adding the details of that process into the array. 
#### Assumption:
Also I am adding both foreground and background processes into the array. For my comfort in accessing pids of fg processes, I have added them and printing them. Although printing can be done only for fg processes because of the presence of fg_bg flag in the struct but I wanted to see pids of fg processes so I am printing both

## Signals

For signals, I am sending the given signal%32 to the given pid, if pid is not present then it will print a statement.
For ctrl z,c I have used `signal()` function and `signal_handler` function. For ctrl z, I have kept a global variable which store the pid of current running fg process or -1 if there is no such process,if it is not -1 then I will send `SIGTSTP` in a child process

I have created a process in ctrl z signal handler because if I didnt even my shell is getting suspended

#### Assumptions

If `sleep x` is running then I press ctrl d, it will not log out of the shell immediately, it will log out after the execution of `sleep x`

## Neonate 
In this I take the input and I will get wait time, Now I will print the pids after every wait_time no of secs. There is a function `is_key_pressed()` which checks whether x is pressed or not. If it is pressed then printing will stop
#### Assumptions
The printing will not stop instantly, if presently wait time is going on. Once that wait_time is done, it will stop. I am checking whether x is pressed before and after waiting

# PART - C

## iMan

In this I create a socket, connect and will send a get Request which is created by observing the url of website. Then I will read the response received, for me, header is ending with `\n\n` although when I asked my friends headers are ending with something else. The part after `\n\n` will be the body, In that I am ignoring contents in `<` and `>`, hence printing only manpage

## Assumptions
I have assumed that every command is separated by a space, for example `hop .. ; sleep 5 & echo hello ;` is considered as a valid command in my shell and not `hop ..;sleep 5&echo hello;`. The same is when both redirection and piping is present
I have declared size of every array I am using as buf_size which is 4096
In my seek optional code it is necessary to give the input as `seek flag search_name /target_directory` but not `seek flag search_name ./target_directory`.
I have renamed the file `.myshrc` to `myshrc` as `.myshrc` is not getting uploaded into git repository, So if we have to check the aliases or functions present in another file then it is necessary to change the filename
Also the function present in `myshrc` can be used only in the start of the command but not elsewhere
