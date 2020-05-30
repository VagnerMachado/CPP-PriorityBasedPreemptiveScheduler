

//============================================================================
// Name        : vagner.cpp
// Author      : Vagner Machado QCIC 23651127
// Version     : One and only!
// Copyright   : Free to use for legal purposes.
// Description : Job Scheduler - Project 3 - CSC 340 - Professor Svadlenka
//============================================================================

/*


 	 This C++ program implements a priority based preemptive scheduler where:
   - Process with highest integer value has highest priority.
   - Round Robin algorithm for processes with same priority.
   - Round Robin has time quantum 10 time units.
   - Default program output prints execution details, waiting and turnaround times for processes.

   To aid implementation the following classes qwere defined:

 * class Process     -   Defined the qualities and behaviors of a process in this application
 * class LinkedList  -   Defines the qualities and behaviors of a Process storing Linked List in this application
 * class Scheduler   -   Defines the qualities and behaviors of a scheduler for Linked List of Processes

 ** USAGE **

  The program needs a tab separated .txt file as argument 1 with the following format for each line
  processnumber as (String)    tab   priority as (int)   tab   burstTime as (int)   tab   arrivalTime as (int)   newline
  i.e.: P12\t8\t18\t3\n indicates Process 12 has priority 8, burst time of 18 time units and arrives at time unit 3.

 ** OPTIONAL ARGUMENTS **

   -help      -> Prints this message to console.
   -timeline  -> Prints a timeline for the process execution.
   -report    -> Prints a detailed report of all processes executed

   # These optional arguments can be passed in any order **AFTER** the input text file argument as first argument
   # The parameter -help at any position overrides all other parameters.
   # Any argument not listed here will cause this message to be printed

 ** COMPILATION AND EXECUTION **

   g++ -c vagner.cpp          -> compiles the source file into object file
   g++ -o app.exe vagner.o    ->  links object file to executable
   ./app.exe {inputFile.txt | -help} [-help] [-timeline] [-report]  -> runs program with data from inputFile.txt and optional arguments if desired

 ** Output is displayed onto the console and written to  vagnerOutput.txt  **

 ** Note that a successful run will delete all contents from input file **

   -Vagner;

 */


#include <iostream>
#include <fstream>
#include <sstream>

//============================================================================
/*
 * Process class
 * Defines instance data and methods needed to instantiate and manipulate Processes
 */

class Process
{
private:

	//instance data
public:
	std::string processNumber;
	int burstTime;
	int timeLeft;
	int priority;
	int waiting;
	int turnaround;
	bool running;
	int arrival;
	Process *next;

	/*
	 * Process constructor
	 * Uses the data passed as parameter to instantiate
	 * a Process to be added to the ready queue.
	 */
	Process(std::string pn, int pr, int bu, int arr)
	{
		arrival = arr;
		processNumber = pn;
		burstTime = bu;
		timeLeft = bu;
		priority = pr;
		waiting = 0;
		turnaround = 0;
		running = false;
		next = NULL;
	}

	/*
	 * toString -  return a string with the instance data for the Process
	 */
	std::string toString()
	{
		std::stringstream ss;
		std::string status = running == false ? "false" : "true";
		ss << "Process # ........ " << processNumber << "\n"
				<< "Arrival Time ..... " << arrival << "\n"
				<< "Burst Time ....... " << burstTime << "\n"
				<< "Time Left ........ " << timeLeft << "\n"
				<< "Priority ......... " << priority << "\n"
				<< "Waiting Time ..... " << waiting << "\n"
				<< "Turnaround Time .. " << turnaround << "\n"
				<< "Running Status ... " << status << "\n\n";
		return ss.str();
	}
}; //end of Process class

//===========================================================================

/*
 * Linked List class
 * Defines the instance data and methods needed to create a linked list
 * to serve as a Process ready queue.
 */

class LinkedList
{
private:
	//instance data
public:
	Process *head;
	Process *tail;

	/*
	 * LinkedList constructor
	 * Instantiates a linked lists with head and tail having NULL values.
	 */
	LinkedList()
	{
		head = NULL;
		tail = NULL;
	}

	/*
	 * addProcess -  adds the Process passed as parameter to the
	 * linked list. Process is added either to:
	 * 		. empty linked list
	 * 		.not empty linked list
	 *
	 * Those two scenarios are analyzed before adding Process
	 *
	 * NULL processes are rejected.
	 */
	void addProcessToTail(Process * p)
	{
		if (p == NULL)
			return;
		if (head == NULL)
		{
			head = tail = p;
		}
		else
		{
			tail->next = p;
			tail = p;
		}
	}

	/*
	 * findLargestPriority - scans through the linked list to
	 * report the value of the largest priority process in the linked list.
	 * Returns the largest priority or -1 case the linked list is empty.
	 */
	int findLargestPriorityProcess()
	{
		if(head == NULL)
			return -1;
		int largest = 0;
		Process * traverse = head;
		while(traverse != NULL)
		{
			if(traverse->priority > largest)
				largest = traverse->priority;
			traverse = traverse->next;
		}
		return largest;
	}

	/*
	 * removeProcessbyNumber - removes a process in the linked list by processNumber
	 * A Process to be deleted can be either in
	 * 		. head
	 * 		. tail
	 * 		. middle
	 *
	 * 	Those three cases are inspected to perform a deletion.
	 * 	The process name must match in order to delete (p1, p2, ...)
	 * 
	 deleteProcess: indicates if process removed must also be deleted (deallocated).
	 * So processes removed from ready queue can
	 * have a false flag for deletePrecess and return
	 * removed process which can be added to another queue, termiatedQueue.
	 * 
	 * Then when time comes, deleted Processes from terminated
	 * queue can call this method with true flag
	 */
	Process *removeProcessByNumber(std::string procNum, bool deleteProcess)
	{
		Process *traverse = head;
		Process *previous = head;

		//traverse the linked list
		while (traverse != NULL)
		{
			//case a match
			if ((procNum.compare(traverse->processNumber)) == 0)
			{
				//case match in the head
				if (traverse == head)
				{
					if (head->next != NULL)
						head = head->next;
					else
						head = tail = NULL;
				}
				//case match in the tail
				else if (traverse == tail)
				{
					tail = previous;
					tail->next = NULL;
				}
				//case match is in the middle
				else
					previous->next = traverse->next;

				//free memory
				if (deleteProcess)
				{
					delete traverse;
					return NULL;
				}
				else
				{
					traverse->next = NULL; //took me while to realize this bug.
					//before not having next = null, my terminated queue was
					//printing all the processes after traverse due to next field being not nullified.
					return traverse;
				}
			}
			//otherwise step forward
			else
			{
				if (traverse != head)
					previous = previous->next;
				traverse = traverse->next;
			}
		}
		return NULL; //case not a match for parameter is found
	}

	/*
	 * removeProcessByPriority removes a process in the linked list by priority
	 * A Process to be deleted can be either in
	 * 		. head
	 * 		. tail
	 * 		. middle
	 *
	 * 	Those three cases are inspected to perform a deletion.
	 * 	The process priority must match in order to delete (9, 8, ...)
	 * 
	 * deleteProcess: indicates if process removed must also be deleted (deallocated).
	 * So processes removed from ready queue can
	 * have a false flag for deletePrecess and return
	 * removed process which can be added to another queue (termiatedQueue).
	 * 
	 * Then when time comes, deleted Processes from terminated
	 * queue can call this method with true flag
	 */
	Process * removeProcessByPriority(int prio, bool deleteProcess)
	{
		Process *traverse = head;
		Process *previous = head;

		//traverse the linked list
		while (traverse != NULL)
		{
			//case a match
			if (prio == traverse->priority)
			{
				//case match in the head
				if (traverse == head)
				{
					if (head->next != NULL)
						head = head->next;
					else
						head = tail = NULL;
				}
				//case match in the tail
				else if (traverse == tail)
				{
					tail = previous;
					tail->next = NULL;
				}
				//case match is in the middle
				else
					previous->next = traverse->next;

				//free memory
				if (deleteProcess)
				{
					delete traverse;
					return NULL;
				}
				else
				{
					traverse->next = NULL; //took me while to realize this bug.
					//before not having next = null, my terminated queue was
					//printing all the processes after traverse due to next field being not nullified.
					return traverse;
				}
			}
			//otherwise step forward
			else
			{
				if (traverse != head)
					previous = previous->next; //control trailing pointer
				traverse = traverse->next;
			}
		}
		return NULL; //case not a match for parameter is found
	}

	/*
	 * removeHeadProcess - removes the process in the head of the linked list
	 * and returns it to the calling function. Returns NULL case there is not a Process to return.
	 */
	Process * removeHeadProcess()
	{
		//empty linked list
		if(head == NULL)
			return NULL;
		//set pointer, step head forward.
		else
		{
			Process * proc = head;
			if(head->next == NULL)
				head = tail = NULL;
			else
				head = head->next;
			proc->next = NULL;
			return proc;
		}
	}

	/*
	 * toString - Return a strign with data for the Processes in the linked list
	 * by calling the toString defined in class Process
	 */
	std::string toString()
	{
		std::stringstream ss;
		Process *traverse = head;
		if (head == NULL)
		{
			ss << "Linked List is Empty\n\n";
			return ss.str();;
		}
		while (traverse != NULL)
		{
			ss << traverse->toString();
			traverse = traverse->next;
		}
		return ss.str();
	}

}; //end of LinkedList class


//==========================================================================

/*
 * class Scheduler - defines the behaviors or an preemptive, high priority
 * based scheduler with round robin algorithm for processes with same
 * priority. The time quantum for round robin is 10 time units.
 *
 * The only public function of the class is run(), which runs the scheduler
 * with the input provided by the user as arguments.
 *
 */
class Scheduler
{

private:
	/*
	 * printUsageDetails - return a string with the usage details for the program in case of error or -help parameter
	 */
	std::string printUsageDetails()
	{
		std::stringstream ss;
		ss <<
				"\n* This C++ program implements a priority based preemptive scheduler where:\n" <<
				"   - Process with highest integer value has highest priority.\n" <<
				"   - Round Robin algorithm for processes with same priority.\n" <<
				"   - Round Robin has time quantum 10 time units.\n" <<
				"   - Default program output prints execution details, waiting and turnaround times for processes.\n\n" <<
				"\t\t ** USAGE **\n\n" <<
				"* The program needs a tab separated .txt file as argument 1 in the following format for each line\n" <<
				"   processnumber as (String)    tab   priority as (int)   tab   burstTime as (int)   tab   arrivalTime as (int)   newline\n" <<
				"   i.e.: P12\\t8\\t18\\t3\\n indicates Process 12 has priority 8, burst time of 18 time units and arrives at time unit 3.\n\n" <<
				"\t\t ** OPTIONAL ARGUMENTS **\n\n" <<
				"   -help      -> Prints this message to console.\n" <<
				"   -timeline  -> Prints a timeline for the process execution.\n" <<
				"   -report    -> Prints a detailed report of all processes executed\n" <<
				"   # These optional arguments can be passed in any order AFTER the input text file argument\n"
				"   # The parameter -help at any position overrides all other parameters.\n" <<
				"   # Any argument not listed here will cause this message to be printed\n\n" <<
				"\t\t ** COMPILATION AND EXECUTION **\n\n" <<
				"   g++ -c vagner.cpp          -> compiles the source file into object file\n" <<
				"   g++ -o app.exe vagner.o    ->  links object file to executable\n" <<
				"   ./app.exe {inputFile.txt | -help} [-help] [-timeline] [-report]  -> runs program with data from inputFile.txt and optional arguments if desired \n\n" <<
				"   ** Output is displayed onto the console and written to  vagnerOutput.txt  **\n\n" <<
				"   ** Note that a successful run will delete all contents from input file ** \n\n"  <<
				"   -Vagner\n\n";

		return ss.str();
	}

	/*
	 * deleteFirstLIneFromFile - deletes the first line of input file.
	 * Gets called when a new process is instantiated with
	 * the data contained in the first line of the Process data file.
	 */
	void deleteFirstLineFromFile(std::string data, std::fstream & outfile)
	{
		//read form file
		std::fstream file;
		file.open(data, std::fstream::in);
		if (!file.is_open())
		{
			std::cout << "\n*** Error opening process file in deleteFirstLineFromFile() ***\n";
			std::cout << printUsageDetails();
			outfile << "\n*** Error opening process file in deleteFirstLineFromFile() ***\n";
			outfile << printUsageDetails();
			outfile.close();
			exit(-1);
		}

		std::string line = "";
		std::string rest = "";

		//ignores processing the first line in the file.
		std::getline(file, line);

		//reads the remainder lines and overwrites the file
		while (std::getline(file, line))
		{
			if (line.compare("") != 0) //case cursor ends at first spot in blank line
				rest += line + "\n";
			line = "";
			//appends if not the first line to update file
		}
		file.close();
		file.open(data, std::fstream::out);
		file << rest;
		file.close();
	}

	/*
	 * readIncomingProcessFromFile - Reads the first line in the
	 * data.txt file and verifies if
	 * the arrival time is same the program clock. If so, it
	 * instantiates a new Process and returns it. Otherwise,
	 * it returns NULL.
	 */
	Process *readIncomingProcessFromFile(int time, std::string data, std::fstream & outfile)
	{
		//read form file
		std::fstream file;
		file.open(data, std::fstream::in);
		std::string word = "";
		if (file.is_open())
		{
			//test for blank file i.e all input was processed
			getline(file, word);
			if (word.compare("") == 0)
				return NULL;
		}
		else
		{
			std::cout << "\n*** Error opening process file in readIncomingProcessFromFile() #1 ***\n";
			std::cout << printUsageDetails();
			outfile << "\n*** Error opening process file in readIncomingProcessFromFile() #1 ***\n";
			outfile << printUsageDetails();
			outfile.close();
			exit(-1);
		}
		file.close();

		//if there is more lines of input
		file.open(data, std::fstream::in);
		word = "";
		char c;

		//array to store member data for Processes
		std::string *input = new std::string[4];
		int place = 0;
		bool more = true;
		if (file.is_open())
		{
			while (place < 4)
			{
				more = true;
				//works on tab and newline delimiters, not carriage return - beware
				while (more)
				{
					file.get(c);
					if (c != '\t' && c != '\n')
						word += c;
					else
					{
						more = false;
						input[place++] = word;
						word = "";
					}
				}
			}
		}
		//error opening the file
		else
		{
			std::cout << "\n*** Error opening process file in readIncomingProcessFromFile #2 ***\n";
			std::cout << printUsageDetails();
			outfile << "\n*** Error opening process file in readIncomingProcessFromFile() #1 ***\n";
			outfile << printUsageDetails();
			outfile.close();
			exit(-1);
		}
		//check if process arrival time is program clock time
		file.close();
		if (stoi(input[3]) == time) //if so return a Process with data read in
		{
			deleteFirstLineFromFile(data, outfile); //call method that deletes the first line of input with new Process
			return new Process(input[0], stoi(input[1]), stoi(input[2]), stoi(input[3]));
		}
		else //otherwise return null i.e, set to arrive in the future.
			return NULL;
	}

	void incrementTurnaroundAndWaitTime(LinkedList &readyQueue, LinkedList &runningProcess)
	{
		Process * traverse = readyQueue.head;
		while(traverse != NULL)
		{
			traverse->turnaround++;
			traverse->waiting++;
			traverse = traverse->next;
		}
		traverse = runningProcess.head;
		while(traverse != NULL)
		{
			if(!traverse->running)
				traverse->waiting++;
			else
				traverse->timeLeft--;
			traverse->turnaround++;
			traverse = traverse->next;
		}
	}

	/*
	 * builds a timeline of execution for the processes. It blends the process name and clock time to
	 * form a properly formatted table
	 */
	void buildTimeline(std::string &processTimeline, std::string &clockTimeline, std::string &blend, std::string procNumber, int timeCounter)
	{
		//supports printing timelines up to time 999 nicely formatted.
		std::string buffNum = timeCounter < 10 ? " " : "";
		std::string buffProc = timeCounter > 99 ? " " : "";
		processTimeline +=  buffProc + procNumber + " | ";
		clockTimeline += buffNum + std::to_string(timeCounter) + " | ";
		if(timeCounter % 20 == 0 && timeCounter != 0)
		{
			processTimeline += "\n";
			clockTimeline += "\n\n";
			blend += processTimeline + clockTimeline;
			processTimeline = "";
			clockTimeline = "";
		}
	}

	/*
	 * buildReport - builds a report of process numbers, turnaround and waiting times
	 * and returns a string with those values formatted properly
	 */
	std::string buildReport(LinkedList &list)
	{
		std::string delim = "\n---------------------------------------\n";
		std::string result = "\n\tPROCESSS EXECUTION REPORT" + delim + "        PROCESS ";
		Process * trav = list.head;
		while(trav != NULL)
		{
			result += trav->processNumber + "  ";
			trav = trav->next;
		}

		result += "\n   WAITING TIME ";
		trav = list.head;
		while(trav != NULL)
		{
			std::string buff = trav->waiting < 10 ? " " : "";
			result += buff + std::to_string(trav->waiting) + "  ";
			trav = trav->next;
		}

		result += "\nTURNAROUND TIME ";
		trav = list.head;
		while(trav != NULL)
		{
			std::string buff = trav->turnaround < 10 ? " " : "";
			result += buff + std::to_string(trav->turnaround) + "  ";
			trav = trav->next;
		}

		return result + delim;
	}

public:

	//constructor for a scheduler;
	Scheduler()
{

}

	/*
	 * run function
	 * Coded to function as a priority based preemptive scheduler with round robin algorithm
	 * for processes having the same priority. The quantum q for round robin is 10 units of time.
	 *
	 * Reads in input file to instantiate processes and add them to ready queue, sets Process
	 * to run on CPU and manipulates the variables in each process as time counter increments.
	 *
	 * When all processes are finished, displays the the turn around and wait times for
	 * each process in the ready queue.
	 */
	int run(int c, char ** arr)
	{
		//controls optional parameters
		bool printFullReport = false;
		bool printTimeline = false;

		std::string fileName;				//input filename
		std::string processTimeline = "";	//aggregator for a time line option
		std::string clockTimeline = "";     //aggregator for a time line option
		std::string finalTimeline = "\n\t\t\t*** PROCESS EXECUTION TIMELINE ***\n\n";  //will contain both timelines above
		std::string bufferTimeline = "--------------------------------------------------------------------------------------------------------\n";
		finalTimeline += bufferTimeline;

		//file to receive output
		std::fstream output;
		output.open("vagnerOutput.txt", std::fstream::out);
		if(!output.is_open())
		{
			std::cout << "\n ** OUTPUT FILE COULD NOT BE CREATED, SEE CONSOLE FOR OUTPUT **\n\n";
		}
		if(c == 1)
		{
			std::string usage =  printUsageDetails();
			output << usage;
			std::cout <<usage;
			return -1;
		}
		else
		{
			//prints usage details case -help needed
			if(strcmp("-help", arr[1]) == 0)
			{
				std::string usage =  printUsageDetails();
				output << usage;
				std::cout <<usage;
				return -1;
			}


			fileName = (arr[1]);

			//decided to remove the file name check because "file" and "file.txt" as name will work as long as data is formatted properly.
			//left the code here for reference

			//validates for a .txt file
			/*		std::size_t found = fileName.find(".txt");
					if (found == std::string::npos)
					{
						std::cout << "\n***   INVALID PROCESS DATA INPUT, MUST BE A .txt file   ***\n\n";
						printUsageDetails();
						return -1;
					}
			 */
			//validates optional parameters
			for(int i = 2; i < c; i++)
			{
				// -help overrides any other parameter
				if (strcmp("-help", arr[i]) == 0)
				{
					std::cout << "\n***   HERE IS SOME HELPFUL INFORMATION   ***\n\n";
					output << "\n***   HERE IS SOME HELPFUL INFORMATION   ***\n\n";
					std::cout << printUsageDetails();
					output << printUsageDetails();
					return -1;
				}
				else if(strcmp("-report", arr[i]) == 0)
					printFullReport = true;
				else if(strcmp("-timeline", arr[i]) == 0)
					printTimeline = true;
				//invalid parameters causes error
				else
				{
					std::cout << printUsageDetails();
					output << printUsageDetails();
					return -1;
				}
			}
		}

		LinkedList readyQueue;      // processes waiting to run
		LinkedList terminatedQueue; // when process is done it is added here to get analyzed.
		LinkedList runningProcess;  // list of processes running, solo or round robin

		//global variables
		bool roundRobin = false;
		int roundRobinTimer = 10;
		bool CPU_busy = false;
		int timeQuantum = 10;

		int i; //so i can print it outside the loop
		std::cout << "\n\t EXECUTION DETAILS \n\n";
		output << "\n\t EXECUTION DETAILS \n\n";
		int defaultValue= 100;

		//sets the length of the loop, case pass the larger data file, loop goes to 250.
		//default data file, 100 max.
		if(fileName.find("data_extra") != std::string::npos)
			defaultValue = 250;

		for (i = 0; i < defaultValue; i++)
		{
			//control variables
			bool newProcess = true;
			int newProcessCount = 0;
			while (newProcess)
			{
				Process * incomingProcess = readIncomingProcessFromFile(i, fileName, output);
				if (incomingProcess == NULL)
					newProcess = false;
				else
				{
					//report new process admitted from file
					std::cout << "Process " << incomingProcess->processNumber << " admitted from file into ready Queue at time "<< i << "\n";
					output << "Process " << incomingProcess->processNumber << " admitted from file into ready Queue at time "<< i << "\n";
					readyQueue.addProcessToTail(incomingProcess);
					newProcessCount++; //track if many processes came in at the same time
				}
			}
			//get the largest priority of ready processes
			int largestPriority = readyQueue.findLargestPriorityProcess();

			//if cpu is free and there is some process to run
			if(!CPU_busy && largestPriority != -1)
			{
				//admit processes by largest priority
				Process * incoming = readyQueue.removeProcessByPriority(largestPriority, false);
				int numberOfRunningProcesses = 0;

				//add processes with largestPriority to running Processes
				while(incoming != NULL)
				{
					//std::cout << "\nAdded processs to running queue from file" << incoming->processNumber;
					numberOfRunningProcesses++;
					runningProcess.addProcessToTail(incoming);
					incoming = readyQueue.removeProcessByPriority(largestPriority, false);
				}
				//set round robin and timer in needed
				if(numberOfRunningProcesses > 1)
				{
					roundRobin = true;  //round robin counter start no matter what
					roundRobinTimer = timeQuantum;
				}
				//run new process
				runningProcess.head->running = true;

				//report new execution
				std::cout << "Process " << runningProcess.head->processNumber << " started to execute at time " << i << "\n";
				output << "Process " << runningProcess.head->processNumber << " started to execute at time " << i << "\n";

				//make cpu busy
				CPU_busy = true;
			}
			//CPU is busy so have to check if preempting will be needed
			else if(CPU_busy && largestPriority != -1)
			{
				//empty the running process queue
				if(largestPriority > runningProcess.head->priority)
				{
					std::string processRemoved = runningProcess.head->processNumber;
					roundRobin = false;
					Process * runningToReady = runningProcess.removeHeadProcess();
					while(runningToReady != NULL)
					{
						runningToReady->running = false;
						readyQueue.addProcessToTail(runningToReady);
						runningToReady = runningProcess.removeHeadProcess();
					}

					//admit the processes with largest priority
					Process * incoming = readyQueue.removeProcessByPriority(largestPriority, false);
					int numberOfRunningProcesses = 0;

					//add processes with largestPriority to running Processes
					while(incoming != NULL)
					{
						numberOfRunningProcesses++;
						runningProcess.addProcessToTail(incoming);
						incoming = readyQueue.removeProcessByPriority(largestPriority, false);
					}
					if(numberOfRunningProcesses > 1)
					{
						roundRobin = true;
						roundRobinTimer = timeQuantum;
					}
					runningProcess.head->running = true;

					//report context switch
					std::cout << "   **  Context switch at time " << i << ": Process preempted: "
							<< processRemoved << ", Process admitted: " << runningProcess.head->processNumber << "\n" ;
					output  << "   **  Context switch at time " << i <<": Process preempted: "
							<< processRemoved << ", Process admitted: " << runningProcess.head->processNumber << "\n" ;

					//report new execution
					std::cout << "Process " << runningProcess.head->processNumber << " started to execute at time " << i << "\n";
					output << "Process " << runningProcess.head->processNumber << " started to execute at time " << i << "\n";

					roundRobinTimer = timeQuantum;
					CPU_busy = true;

				}
				else if(largestPriority == runningProcess.head->priority)
				{
					roundRobin = true;

					//admit new processes with largest priority
					Process * incoming = readyQueue.removeProcessByPriority(largestPriority, false);
					//add processes with largestPriority to running Processes
					while(incoming != NULL)
					{
						runningProcess.addProcessToTail(incoming);
						incoming = readyQueue.removeProcessByPriority(largestPriority, false);
					}
					//set status to running and make cpu busy
					runningProcess.head->running = true;

					//new process is admitted into the tail of the running queue so no new running message is needed
					//std::cout << "Process " << runningProcess.head->processNumber << " started to execute at time " << i << "\n";
					CPU_busy = true;
				}
			}

			roundRobinTimer--;

			//otherwise if there is still round robin
			bool contextSwitch = false;

			if(roundRobin)
			{
				//roundRobinTimer--;
				if (roundRobinTimer == 0)
				{
					contextSwitch = true;
					std::string processRemoved = runningProcess.head->processNumber;
					buildTimeline(processTimeline, clockTimeline, finalTimeline, runningProcess.head->processNumber, i);
					runningProcess.addProcessToTail(runningProcess.removeHeadProcess());
					runningProcess.tail->running = false;
					runningProcess.head->running = true;
					roundRobinTimer = timeQuantum;

					//report context switch and new execution, current process gets last unit of time, then switch -> hence i + 1
					std::cout << "   **  Context switch at time " << i + 1 <<": Process preempted: "
							<< processRemoved << ", Process admitted: " << runningProcess.head->processNumber << "\n" ;
					std::cout << "Process " << runningProcess.head->processNumber << " started to execute at time " << i + 1 << "\n";
					output << "   **  Context switch at time " << i + 1 <<": Process preempted: "
							<< processRemoved << ", Process admitted: " << runningProcess.head->processNumber << "\n" ;
					output << "Process " << runningProcess.head->processNumber << " started to execute at time " << i + 1 << "\n";
				}
			}
			//increment turnaround, waiting if needed, decrement time left.
			incrementTurnaroundAndWaitTime(readyQueue, runningProcess);

			//handles process termination
			if(runningProcess.head != NULL && runningProcess.head->timeLeft == 0)
			{
				std::cout << "Process " << runningProcess.head->processNumber << " terminated execution at time " << i << "\n";
				output << "Process " << runningProcess.head->processNumber << " terminated execution at time " << i << "\n";

				buildTimeline(processTimeline, clockTimeline, finalTimeline, runningProcess.head->processNumber, i);
				terminatedQueue.addProcessToTail(runningProcess.removeHeadProcess());
				terminatedQueue.tail->running = false;
			}

			//Case there is a context switch on ended process, terminate it
			//i came up with this solution when testing with more processes in input file.
			else if(runningProcess.tail != NULL && runningProcess.tail->timeLeft == 0)
			{
				std::cout << "Process " << runningProcess.head->processNumber << " terminated execution at time " << i << "\n";
				output << "Process " << runningProcess.head->processNumber << " terminated execution at time " << i << "\n";

				buildTimeline(processTimeline, clockTimeline, finalTimeline, runningProcess.head->processNumber, i);
				terminatedQueue.addProcessToTail(runningProcess.removeProcessByNumber(runningProcess.tail->processNumber, false));
				terminatedQueue.tail->running = false;
			}


			// case a process is running and no context switch happened
			else if (runningProcess.head != NULL && !contextSwitch)
				buildTimeline(processTimeline, clockTimeline, finalTimeline, runningProcess.head->processNumber, i);

			//otherwise CPU is idle
			else if (runningProcess.head == NULL)
			{
				buildTimeline(processTimeline, clockTimeline, finalTimeline, "--", i);
				std::cout << "CPU idle at time " << i << "\n";
				output << "CPU idle at time " << i << "\n";
			}

			//check if all process terminated, if so round robin is over and CPU is free
			if(runningProcess.head == NULL)
			{
				roundRobin = false;
				CPU_busy = false;
			}
		}
		std::cout << "\n";
		output << "\n";

		//default wait time and turnaround report
		std::string defaultReport = buildReport(terminatedQueue);
		std::cout << "\n" << defaultReport;
		output << "\n" << defaultReport;

		//prints all details for processes
		if(printFullReport)
		{
			std::string report = terminatedQueue.toString();
			std::cout << "\n\nFULL PROCESS REPORT\n-------------------------\n";
			std::cout << report;
			std::cout << "-------------------------\n\n";
			output << "\n\nFULL PROCESS REPORT\n-------------------------\n";
			output << report;
			output << "-------------------------\n\n";
		}

		//build and print final timeline
		if(printTimeline)
		{
			buildTimeline(processTimeline, clockTimeline, finalTimeline, "--", i);
			std::cout <<  finalTimeline << bufferTimeline;
			output <<  finalTimeline << bufferTimeline;
		}
		output.close();

		return 0;
	}
};

/*
 * main - instantiates a new scheduler and runs it with input provided
 * by the user, returns -1 for error or 0 for successful execution.
 */
int main(int c, char ** arr)
{
	Scheduler s;
	return s.run(c, arr);
}
