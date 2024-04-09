// Naive_Solution_Task Scheduling.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <chrono>
#include <numeric>

using namespace std;

void printVector(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

vector<int> initializeVector(int size, int minValue, int maxValue) {
    vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = rand() % (maxValue - minValue + 1) + minValue;
    }
    return result;
}

pair<std::vector<int>, int> generatePermutations(vector<int>& objects, int startIndex, int d) {
    
    std::pair<std::vector<int>, int> results;
    
    if (startIndex == objects.size() - 1) {
        for (int obj : objects) {
            cout << obj << " ";
        }
        cout << endl;
        int TimingCost = 0;
        vector<int> TasksEndTime(objects.size(), 0);
        for (int i = 0; i < objects.size(); ++i) {
            if (i == 0)
                TasksEndTime[i] = objects[i];
            else
                TasksEndTime[i] = objects[i] + TasksEndTime[i - 1];
        }
        for (int i = 0; i< TasksEndTime.size() ;++i)
            TimingCost += abs(TasksEndTime[i] - d);

        return std::make_pair(objects, TimingCost);
    }
    // Recursively generate permutations
    for (int i = startIndex; i < objects.size(); ++i) {
        swap(objects[startIndex], objects[i]);
        auto temp =generatePermutations(objects, startIndex + 1,d);
        if (i == startIndex || temp.second < results.second)
            results = temp;
        
        swap(objects[startIndex], objects[i]);
    }
    return results;
}

int main() {
    // Example usage
    int d = 4;
    int size = 3;
    int minValue = 1;
    int maxValue = 10;

    srand(static_cast<unsigned int>(time(nullptr)));
    
    cout << "Enter due-date: ";
    cin >> d;
    cout << "Enter number of tasks: ";
    cin >> size;
    cout << "Enter min and max value: ";
    cin >> minValue >> maxValue;
     


    vector<int> objects = initializeVector(size, minValue, maxValue);
    //vector<int> objects = { 3, 6, 5 ,4};
    cout << "All possible orders of " << size << " objects:" <<endl;
    auto result = generatePermutations(objects, 0, d);
    
    cout << "The minimal error to the due-date "<<d<< " is: " << result.second <<endl;
    cout << "The Tasks order is" << endl;
    printVector(result.first);

    return 0;
}