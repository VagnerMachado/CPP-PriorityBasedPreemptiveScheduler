# CPP-PriorityBasedPreemptiveScheduler   
---  


This C++ program implements a priority based preemptive scheduler where:  
   - Process with highest integer value has highest priority.  
   - Round Robin algorithm for processes with same priority.  
   - Round Robin has time quantum 10 time units.  
   - Default program output prints execution details, waiting and turnaround times for processes.  
   

   To aid implementation the following classes where defined:  

 * class Process     -   Defined the qualities and behaviors of a process in this application  
 * class LinkedList  -   Defines the qualities and behaviors of a Process storing Linked List in this application  
 * class Scheduler   -   Defines the qualities and behaviors of a scheduler for Linked List of Processes  

 ## USAGE

  The program needs a tab separated .txt file as argument 1 with the following format for each line  
  processnumber as (String)    tab   priority as (int)   tab   burstTime as (int)   tab   arrivalTime as (int)   newline  
  i.e.: P12\t8\t18\t3\n indicates Process 12 has priority 8, burst time of 18 time units and arrives at time unit 3.  

 ## OPTIONAL ARGUMENTS

   -help      -> Prints this message to console.  
   -timeline  -> Prints a timeline for the process execution.  
   -report    -> Prints a detailed report of all processes executed  

   * These optional arguments can be passed in any order **AFTER** the input text file argument as first argument  
   * The parameter -help at any position overrides all other parameters.  
   * Any argument not listed here will cause this message to be printed.  

  ## COMPILATION AND EXECUTION  

   g++ -c vagner.cpp -> compiles the source file into object file  
   g++ -o app.exe vagner.o ->  links object file to executable   
   ./app.exe {inputFile.txt | -help} [-help] [-timeline] [-report] -> runs program with data from inputFile.txt and optional arguments if desired   

 **Output is displayed onto the console and written to  vagnerOutput.txt**

 **Note that a successful run will delete all contents from input file**

   *Vagner*

