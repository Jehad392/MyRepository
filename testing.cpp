#include <cstdlib>
#include <iostream>
#include <deque>
#include <queue>
#include <algorithm> 
using namespace std;

int main()
{

    class Process
{
public:
    int waitingtime = 0;    // how long the process has been waiting
    int turnaroundtime = 0; // how long it took the process from start to completed
    int responsetime = -1;  // when the process started

    Process() {}//constructor
};
    int process[9][20] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4, 0, 0, 0, 0, 0},           // P1
                {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8, 0, 0, 0},         // P2
                {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6, 0, 0, 0},     // P3
                {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3, 0, 0, 0},          // P4
                {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4, 0},    // P5
                {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8, 0, 0, 0},        // P6
                {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10, 0, 0, 0 ,0, 0},     // P7
                {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0} };         // P8

    // burst and iotime deque
    std::deque<int> burst[9], iotime[9], remainingio[9];

    //load burst and iotime
    for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 19; j = j + 2)
		{
            if (i > 4 && j > 17)
                    int x = 0;

			if (process[i][j] != 0)
			{           
				burst[i].push_back(process[i][j]);
				iotime[i].push_back(process[i][j + 1]);
                remainingio[i].push_back(process[i][j + 1]);
			}
        }
	}
    
    // ready queue and io queue
	std::deque<int> readyQ, ioQ, completed;
    
	for (int i = 1; i < 9; i++)
	{
		readyQ.push_back(i);
	}
	int currentTime = 0; //added bursts 
	int runningProcess; // process running
	int runningBurst = 0; // running process burst
	int runningIO; // running process iotime

	int count = 0;
    while (!readyQ.empty() || !ioQ.empty()) //while the readyQ is not equal to 0, loop
    //while (!readyQ.empty()) //while the readyQ is not equal to 0, loop
	{
        if (!readyQ.empty())
        { 
            // get process from readyQ along w/ cpu burst and io burst
            runningProcess = readyQ.front(); //push running process to front of readyQ
            readyQ.pop_front();
            

            runningBurst = burst[runningProcess].front(); //push burst of running process to front
            burst[runningProcess].pop_front();

            runningIO = iotime[runningProcess].front(); //push io of running process to front
            iotime[runningProcess].pop_front();
        }
        else
        {
            runningProcess = 0;
            runningIO = 0;
        }

        // display current running process
        if (runningProcess > 0)
        {
            cout << endl << "Now Running: P" << runningProcess 
                << " (CPU: " << runningBurst 
                << " , IO: " << runningIO << ")" << endl;
        }
        else
        {
            cout << endl << "Now Running: None (CPU: , IO: )" << endl;
        }

        // Show the other queue in ready with burst times
        cout << "Ready Queue: Process Burst" << endl;

        for (int i = 0; i < readyQ.size(); i++) //display all processes in readyQ
        {
            int readyProcess = readyQ.at(i); //process in readyQ at location is
            int burstTime = burst[readyProcess].front(); //push burst of ready process to front
            cout << "             P" << readyProcess << "      " << burstTime << endl;
        }
        
        // Sort ioQ
        if (ioQ.size() > 0)
        {            
            int s = ioQ.size();
            int array[s][2];

            // Populate ioQ into array
            for (int i = 0; i < s; i++)
            {
                int ioProcess = ioQ.at(i); //display process

                if (ioProcess > 0)
                {
                    int remainingTime = remainingio[ioProcess].front();

                    array[i][0] = remainingTime;
                    array[i][1] = ioQ.at(i);
                }
            }
            int arraySize = sizeof(array) / sizeof(array[0]);

            /*
            // Debug - Printing original array.
            cout << "\nOriginal array: " << endl;
            for (int i = 0; i < arraySize; i++)
            {
                cout << "I/O Time: " << array[i][0] << " P" << array[i][1] << endl;
            }
            */
           
            // 2D Array Sort
            std::qsort(array, s, sizeof(*array),
            [](const void *arg1, const void *arg2)->int
            {
                int const *lhs = static_cast<int const*>(arg1);
                int const *rhs = static_cast<int const*>(arg2);
                return (lhs[0] < rhs[0]) ? -1
                    :  ((rhs[0] < lhs[0]) ? 1
                    :  (lhs[1] < rhs[1] ? -1
                    :  ((rhs[1] < lhs[1] ? 1 : 0))));
            });

            /*
            // Debug - Printing sorted array.
            cout << "\nSorted array: " << endl;
            for (int i = 0; i < arraySize; i++)
            {
                cout << "I/O Time: " << array[i][0] << " P" << array[i][1] << endl;
            }
            */
            
            // Clear ioQ
            for (int i = 0; i < ioQ.size(); i++)
            {
               //ioQ.pop_front();
               ioQ.clear();
            }     
            // Populate sorted ioQ
            for (int i = 0; i < arraySize; i++)
            {
                int ioprocess = array[i][1];
                ioQ.push_back(ioprocess);
            }
            
        }

        // Display ioQ
		cout << "Now in I/O:  Process Remaining I/O time" << endl;
		if (ioQ.size() == 0)
			cout << "             [empty]" << endl;
		else
		{
			for (int i = 0; i < ioQ.size(); i++)
			{ 
				int ioProcess = ioQ.at(i); //display process
				int remainingTime = remainingio[ioProcess].front(); //push remaining time of process to the front of the ioQ
				
                if (remainingTime > 0)
                {
                    cout << "             P" << ioProcess << "      " << remainingTime << endl;
                }
			}
		}
    
        /*
        if (readyQ.empty() && runningProcess < 1)
        {
            idle = true;

            int nextReadyProcess = ioQ.front();
            ioQ.pop_front();

            readyQ.push_back(nextReadyProcess);

            runningBurst = remainingio[nextReadyProcess].front();
            remainingio[nextReadyProcess].pop_front();
        }
        */

        //if (!idle)
        //{
            // Make sure to drain ioQ to completion
            // Update readyQ and ioQ
            int size = ioQ.size();

            for (int i = 0; i < size; i++)
            {
                int ioProcess = ioQ.front(); //error might be here
                ioQ.pop_front();

                int remainingTime;
                
                if (readyQ.empty() && runningProcess < 1) //if readyQ is empty and if nothing is running
                {
                    readyQ.push_back(ioProcess); //fix this

                    runningBurst = remainingio[ioProcess].front();
                    remainingio[ioProcess].pop_front();
                    
                }
                else
                {
                    remainingTime = remainingio[ioProcess].front(); 
                    remainingio[ioProcess].pop_front();
                                           
                    remainingTime -= runningBurst; // remaining time of io - running burst
                    
                    if (remainingTime > 0) // if remaining time is greater than 0, process still in ioQ
                    {
                        ioQ.push_back(ioProcess); // push the process to the back of the ioQ
                        remainingio[ioProcess].push_front(remainingTime); // push the remaining time of the process to the front
                    }
                    if (remainingTime <= 0)// push new process in ready queue
                    {   
                        if (burst[ioProcess].front() < 0) //if process completed, push to completed queue
                        {
                            completed.push_back(ioProcess);  
                            cout << "** P" << ioProcess << " completed at " << currentTime << endl;                  
                        }
                        else
                        {
                            readyQ.push_back(ioProcess);
                        }
                    }
                }
            }
        //}   

        // push running process to I/O queue
        if (runningProcess > 0)
            ioQ.push_back(runningProcess);

        // track current waiting time (add all bursts)
        currentTime += runningBurst;
        cout << endl << "CurrentTime: " << currentTime;
    }    

    //cout << endl << "CurrentTime: " << currentTime;

    return 0;
}