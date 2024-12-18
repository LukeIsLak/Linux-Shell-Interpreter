# P1 : Simple Shell Interpreter

### Overview
This program is to mimic a Simple Shell Interpreter for CSC 360 Programming Assignment 1

### Features
The program is capable of the following feature:
    - Foreground Execution
        - Able to execute commands
    - Change in Directory
        - Able to change the current working directory of the process
    - Background Execution
        - Able to run commands in the backgrounds, will let user know when it is completed
        - COMMANDS:
            - bg <command> : Add a command to run in the background
            - bglist : Prints a list of all currently running Background processes
            - (additional) bgkill <process number> : Kills the specific process with that id
    - (additional) Command History
        - Can use the arrow keys change between previously inputed commands
        - Up and Down arrow keys work

### Technologies Used
- Project is done entirely in C, additional help with MAKEFILE

### Installation
1. **Prerequisites**: Install a C compiler such as gcc
2. **Clone the repository**:
    ```
    git clone https://gitlab.csc.uvic.ca/courses/2024091/CSC360_COSI/assignments/lukekuligowicz/p1.git
    ```

### Usage
1. **How to run the program**:

    using make, should get a ssi file
    ```
    make
    ```

    Upon that
    '''
    ./ssi
    '''

### File Structure
Main files (All created by Luke Kuligowicz)
    - main.c : main holder file, manages the program
    - linkedlists.h : helper file to handle and control linked lists
    - command.h : helper file to handle command history
    - background.h : helper file to handle background execution related tasks
    - token.h : helper file to tokenize / untokenize strings
    - MAKEFILE : make file
    - README.txt : readme