// Naive_Solution_Task Scheduling.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "xlsxwriter.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <chrono>
#include <numeric>
#include <algorithm>


using namespace std;

std::vector<int> accumulateValues(const std::vector<int>& input) {
    std::vector<int> accumulatedValues;
    int sum = 0;

    for (int i = 0; i < input.size(); ++i) {
        sum += input[i];
        accumulatedValues.push_back(sum);
    }

    return accumulatedValues;
}
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

bool isEqualToAny(const std::vector<int>& vec, int value) {
    // Using std::find algorithm to search for the value in the vector
    auto it = std::find(vec.begin(), vec.end(), value);
    // If iterator points to end of vector, value is not found
    // Otherwise, it found the value
    return it != vec.end();
}

pair<std::vector<int>, int> generatePermutations(vector<int>& objects, int startIndex, int d) {
    
    std::pair<std::vector<int>, int> results;
    
    if (startIndex == objects.size() - 1) {
        
        //for (int obj : objects) {
        //    cout << obj << " ";
        //}

        //cout << endl;
        int TimingCost = 0;
        vector<int> TasksEndTime(objects.size(), 0);
        TasksEndTime = accumulateValues(objects);

        /*for (int i = 0; i < objects.size(); ++i) {
            if (i == 0)
                TasksEndTime[i] = objects[i];
            else
                TasksEndTime[i] = objects[i] + TasksEndTime[i - 1];
        }*/

        for (int i = 0; i< TasksEndTime.size() ;++i)
            TimingCost += abs(TasksEndTime[i] - d);

        return std::make_pair(objects, TimingCost);
    }
    // Recursively generate permutations
    for (int i = startIndex; i < objects.size(); ++i) {
        swap(objects[startIndex], objects[i]);
        auto temp = generatePermutations(objects, startIndex + 1,d);
        if (i == startIndex || temp.second < results.second)
            results = temp;
        
        swap(objects[startIndex], objects[i]);
    }
    return results;
}
std::string concatenateIntegers(const std::vector<int>& integers) {
    std::string concatenatedIntegers;
    for (size_t i = 0; i < integers.size(); ++i) {
        concatenatedIntegers += to_string(integers[i]);
        if (i != integers.size() - 1) {
            concatenatedIntegers += ", ";
        }
    }
    return concatenatedIntegers;
}

void addLineToExcel(lxw_worksheet* worksheet, const std::string& name, const std::string& age, const std::string& gender, const std::string& occupation) {
    // Write the data to the Excel file.
    static int row = 1; // static variable to keep track of row number
    worksheet_write_string(worksheet, row, 0, name.c_str(), NULL);
    worksheet_write_string(worksheet, row, 1, age.c_str(), NULL);
    worksheet_write_string(worksheet, row, 2, gender.c_str(), NULL);
    worksheet_write_string(worksheet, row, 3, occupation.c_str(), NULL);
    row++; // increment the row number for the next call
}

int main() {
    // Example usage
    int d;
    int size = 4;
    int minValue = 1;
    int maxValue = 10;

    srand(static_cast<unsigned int>(time(nullptr)));
    
    //cout << "Enter due-date: ";
    //cin >> d;
    //cout << "Enter number of tasks: ";
    //cin >> size;
    //cout << "Enter min and max value: ";
    //cin >> minValue >> maxValue;
    int i = 0;
     
    lxw_workbook* workbook = workbook_new("Examples.xlsx");
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);

    // Data to be written into the Excel file.
    std::string column_names[4] = { "Due-Date", "Tasks Durations", "Minimal Error", "Tasks order" };
    
    lxw_format* bold_format = workbook_add_format(workbook);
    format_set_bold(bold_format);
    
    for (int col = 0; col < 4; ++col) {
        worksheet_write_string(worksheet, 0, col, column_names[col].c_str(), bold_format);
    }


    while (i < 6)
    {
        vector<int> objects = initializeVector(size, minValue, maxValue);
        d = rand() % accumulateValues(objects).back() + 1;

        //vector<int> objects = { 3, 6, 5 ,4};
        //cout << "All possible orders of " << size << " objects:" << endl;
        auto result = generatePermutations(objects, 0, d);
        if (!isEqualToAny(accumulateValues(result.first), d)) {
            
            cout << "The Tasks durations is: ";
            printVector(objects);
            string TaskDuration = concatenateIntegers(objects); // 
            cout << "The minimal error of the due-date " << d << " is: " << result.second << endl;
            
            string Due_Date = to_string(d); //
            string MinimalError = to_string(result.second); //

            cout << "The Tasks order is" << endl;
            printVector(result.first);
            string TaskOrder = concatenateIntegers(result.first); //
            cout << endl;
            addLineToExcel(worksheet,Due_Date, TaskDuration, MinimalError, TaskOrder);
            ++i;
        }
    }
        
    //cout << "The minimal error of the due-date " << d << " is: " << result.second << endl;
    //cout << "The Tasks order is" << endl;
    //printVector(result.first);


    workbook_close(workbook);
    cout << "Excel file created successfully!" << std::endl;


    return 0;
}