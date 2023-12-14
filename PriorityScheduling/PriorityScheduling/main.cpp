#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct Process {
    int PID;
    int arrivaltime;
    int burstTime;
    int priority; // if needed idk
};

struct Statistics {
    float avgWaitingTime;
    float avgTurnaroundTime;
    float avgResponseTime;
};

int FirstComeFirstServe(const vector<Process>& processes, vector<Statistics>& stats, ofstream& file) {
    int number = processes.size();

    int waitingtime, total_waitingTime = 0;
    int Turnaroundtime, total_turnaroundTime = 0;
    int total_responseTime = 0; // Initialize total_responseTime
    float avgTurnAround, avgWaitingtime, avgResponseTime;
    int currTime = 0;

    for (int i = 0; i < number; i++) {
        waitingtime = currTime - processes[i].arrivaltime;
        if (waitingtime < 0)
            waitingtime = 0;

        total_waitingTime += waitingtime;

        // Calculate response time
        int responseTime = currTime - processes[i].arrivaltime;
        total_responseTime += responseTime; // Add response time to total

        Turnaroundtime = waitingtime + processes[i].burstTime;
        total_turnaroundTime += Turnaroundtime;

        currTime += processes[i].burstTime;
    }

    // Calculate averages after processing all processes
    avgWaitingtime = (float)(total_waitingTime) / number;
    avgTurnAround = (float)(total_turnaroundTime) / number;
    avgResponseTime = (float)(total_responseTime) / number; // Calculate average response time

    // Add results to the stats vector
    Statistics result;
    result.avgWaitingTime = avgWaitingtime;
    result.avgTurnaroundTime = avgTurnAround;
    result.avgResponseTime = avgResponseTime;
    stats.push_back(result);

    // Write to file
    file << "First Come First Serve,";
    file << avgWaitingtime << "," << avgTurnAround << "," << avgResponseTime << "\n";

    cout << "First Come First Serve:\n";
    cout << "Average Waiting Time: " << avgWaitingtime << "\n";
    cout << "Average Turnaround Time: " << avgTurnAround << "\n";
    cout << "Average Response Time: " << avgResponseTime << "\n\n";

    return 0;
}



void RoundRobin(int timeQuantum, const vector<Process>& processes, vector<Statistics>& stats, ofstream& file) {
    int number = processes.size();

    vector<int> remainingTime(number);
    vector<int> waitingTime(number, 0);
    vector<int> TurnAround(number, 0);
    vector<int> responseTime(number, -1);  // Initialize response time to -1

    int currentTime = 0;
    int completedProcesses = 0;

    for (int i = 0; i < number; i++) {
        remainingTime[i] = processes[i].burstTime;
    }

    while (completedProcesses < number) {
        bool isProcessExecuted = false;
        for (int i = 0; i < number; i++) {
            if (remainingTime[i] > 0) {
                isProcessExecuted = true;
                if (responseTime[i] == -1) {
                    responseTime[i] = currentTime - processes[i].arrivaltime;
                }

                if (remainingTime[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingTime[i] -= timeQuantum;
                } else {
                    currentTime += remainingTime[i];
                    waitingTime[i] = currentTime - processes[i].burstTime - processes[i].arrivaltime;
                    TurnAround[i] = waitingTime[i] + processes[i].burstTime;
                    remainingTime[i] = 0;
                    completedProcesses++;
                }
            }
        }
        if (!isProcessExecuted) {
            currentTime++;
        }
    }

    // Calculate total and average waiting time, turnaround time, and response time
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;
    for (int i = 0; i < number; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += TurnAround[i];
        totalResponseTime += responseTime[i];
    }
    float averageWaitingTime = (float)(totalWaitingTime) / number;
    float averageTurnaroundTime = (float)(totalTurnaroundTime) / number;
    float averageResponseTime = (float)(totalResponseTime) / number;

    // Add results to the stats vector
    Statistics result;
    result.avgWaitingTime = averageWaitingTime;
    result.avgTurnaroundTime = averageTurnaroundTime;
    result.avgResponseTime = averageResponseTime;
    stats.push_back(result);

    // Write to file

    file << "Round Robin,";
    file << averageWaitingTime << "," << averageTurnaroundTime << "," << averageResponseTime << "\n";

    cout << "Round Robin:\n";
    cout << "Average Waiting Time: " << averageWaitingTime << "\n";
    cout << "Average Turnaround Time: " << averageTurnaroundTime << "\n";
    cout << "Average Response Time: " << averageResponseTime << "\n\n";
}

int ShortestJobFirst(const vector<Process>& processes, vector<Statistics>& stats, ofstream& file) {
    int number = processes.size();
    int total_waitingTime = 0;
    int total_turnaroundTime = 0;
    int total_responseTime = 0;
    float avgTurnAround, AvgWaitingtime, AvgResponseTime;

    vector<Process> sortedProcesses = processes;
    sort(sortedProcesses.begin(), sortedProcesses.end(), [](const Process& a, const Process& b) {
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;

    for (int i = 0; i < number; i++) {
        int waitingtime = currentTime - sortedProcesses[i].arrivaltime;
        if (waitingtime < 0)
            waitingtime = 0;

        total_waitingTime += waitingtime;
        total_responseTime += waitingtime; // Calculate the response time for each process

        int turnaroundtime = waitingtime + sortedProcesses[i].burstTime;
        total_turnaroundTime += turnaroundtime;

        currentTime += sortedProcesses[i].burstTime;
    }

    AvgWaitingtime = (float)(total_waitingTime) / number;
    avgTurnAround = (float)(total_turnaroundTime) / number;
    AvgResponseTime = (float)(total_responseTime) / number; // Calculate the average response time

    // Add results to the stats vector
    Statistics result;
    result.avgWaitingTime = AvgWaitingtime;
    result.avgTurnaroundTime = avgTurnAround;
    result.avgResponseTime = AvgResponseTime; // Add the average response time to the stats vector
    stats.push_back(result);

    // Write to file
    file << "Shortest Job First,";
    file << AvgWaitingtime << "," << avgTurnAround << "," << AvgResponseTime << "\n";

    cout << "Shortest Job First:\n";
    cout << "Average Waiting Time: " << AvgWaitingtime << "\n";
    cout << "Average Turnaround Time: " << avgTurnAround << "\n";
    cout << "Average Response Time: " << AvgResponseTime << "\n\n";

    return 0;
}
void MultilevelFeedbackQueue(const vector<Process>& processes, vector<Statistics>& stats, ofstream& file) {
    int number = processes.size();

    vector<int> remainingTime(number, 0);
    vector<int> waitingTime(number, 0);
    vector<int> turnaroundTime(number, 0);
    vector<int> responseTime(number, -1);

    int currentTime = 0;
    int completedProcesses = 0;

    // Initialize queues for MLFQ
    vector<vector<Process>> queues(3);

    for (int i = 0; i < number; i++) {
        remainingTime[i] = processes[i].burstTime;
        queues[0].push_back(processes[i]);
    }

    while (completedProcesses < number) {
        bool isProcessExecuted = false;

        for (int i = 0; i < queues.size(); i++) {
            if (!queues[i].empty()) {
                Process currentProcess = queues[i].front();
                queues[i].erase(queues[i].begin());

                if (responseTime[currentProcess.PID - 1] == -1) {
                    responseTime[currentProcess.PID - 1] = currentTime - currentProcess.arrivaltime;
                }

                int quantum = 0;
                if (i == 0) {
                    quantum = 2; // Time quantum for the first queue
                } else if (i == 1) {
                    quantum = 4; // Time quantum for the second queue
                } else {
                    quantum = remainingTime[currentProcess.PID - 1]; // No time quantum for the last queue
                }

                currentTime += min(quantum, remainingTime[currentProcess.PID - 1]);
                remainingTime[currentProcess.PID - 1] -= min(quantum, remainingTime[currentProcess.PID - 1]);

                if (remainingTime[currentProcess.PID - 1] == 0) {
                    // Process completed
                    waitingTime[currentProcess.PID - 1] = currentTime - currentProcess.burstTime - currentProcess.arrivaltime;
                    turnaroundTime[currentProcess.PID - 1] = currentTime - currentProcess.arrivaltime;
                    completedProcesses++;
                } else {
                    // Move the process to the next queue
                    queues[i + 1].push_back(currentProcess);
                }

                isProcessExecuted = true;
                break;
            }
        }

        if (!isProcessExecuted) {
            currentTime++;
        }
    }

    // Calculate total and average waiting time, turnaround time, and response time
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;

    for (int i = 0; i < number; i++) {
        totalWaitingTime += waitingTime[i];
        totalTurnaroundTime += turnaroundTime[i];
        totalResponseTime += responseTime[i];
    }

    float averageWaitingTime = (float)(totalWaitingTime) / number;
    float averageTurnaroundTime = (float)(totalTurnaroundTime) / number;
    float averageResponseTime = (float)(totalResponseTime) / number;

    // Add results to the stats vector
    Statistics result;
    result.avgWaitingTime = averageWaitingTime;
    result.avgTurnaroundTime = averageTurnaroundTime;
    result.avgResponseTime = averageResponseTime;
    stats.push_back(result);

    // Write to file
    file << "Multilevel Feedback Queue,";
    file << averageWaitingTime << "," << averageTurnaroundTime << "," << averageResponseTime << "\n";

    cout << "Multilevel Feedback Queue:\n";
    cout << "Average Waiting Time: " << averageWaitingTime << "\n";
    cout << "Average Turnaround Time: " << averageTurnaroundTime << "\n";
    cout << "Average Response Time: " << averageResponseTime << "\n\n";
}




int main() {
    int iterations = 20;
    int initialProcesses = 5;
    int processIncrement = 5;

    int timeQuantum1 = 2;
    int timeQuantum2 = 4;

    vector<vector<Statistics>> allStats;
    vector<Process> processes;

    // Open the files before calling the functions
    ofstream fcfsFile("/Users/muhammadabdelmohsen/Desktop/Operating Systems/OSproject7/OSproject7/fcfs.csv");
    ofstream rrFile("/Users/muhammadabdelmohsen/Desktop/Operating Systems/OSproject7/OSproject7/rr.csv");
    ofstream sjfFile("/Users/muhammadabdelmohsen/Desktop/Operating Systems/OSproject7/OSproject7/sjf.csv");
    ofstream mlfqFile("/Users/muhammadabdelmohsen/Desktop/Operating Systems/OSproject7/OSproject7/mlfq.csv"); // Add a file for MLFQ
    
    fcfsFile << "Algorithm, AvgWaitingTime, AvgTurnaroundTime, AvgResponseTime\n";
    rrFile << "Algorithm, AvgWaitingTime, AvgTurnaroundTime, AvgResponseTime\n";
    sjfFile << "Algorithm, AvgWaitingTime, AvgTurnaroundTime, AvgResponseTime\n";
    mlfqFile << "Algorithm, AvgWaitingTime, AvgTurnaroundTime, AvgResponseTime\n"; // Write header for MLFQ file

    for (int iter = 0; iter < 500; ++iter) {
        processes.clear();

        for (int i = 0; i < initialProcesses + iter * processIncrement; ++i) {
            processes.push_back({i + 1, 0, rand() % 10 + 1, rand() % 5 + 1});
        }

        vector<Statistics> fcfsStats;
        FirstComeFirstServe(processes, fcfsStats, fcfsFile);
        allStats.push_back(fcfsStats);

        vector<Statistics> rrStats;
        RoundRobin(timeQuantum1, processes, rrStats, rrFile);
        allStats.push_back(rrStats);

        vector<Statistics> sjfStats;
        ShortestJobFirst(processes, sjfStats, sjfFile);
        allStats.push_back(sjfStats);

        // Run MLFQ and write the statistics
        vector<Statistics> mlfqStats;
        MultilevelFeedbackQueue(processes, mlfqStats, mlfqFile);
        allStats.push_back(mlfqStats);
    }

    // Close the files after calling the functions
    fcfsFile.close();
    rrFile.close();
    sjfFile.close();
    mlfqFile.close(); // Close MLFQ file

    return 0;
}
