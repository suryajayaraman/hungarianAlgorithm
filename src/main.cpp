#include<iostream>
#include "hungarianAlgorithm/utils.h"
#include "hungarianAlgorithm/hungarian.h"



int main()
{
    const matXd testArr {
                    {5,  9, 3,  6},
                    {8,  7, 8,  2},
                    {6, 10, 12, 7},
                    {3, 10, 8,  6}
                };
    std::cout << "Input Array = \n" << testArr;
    std::vector<uint16_t> optimalAssignment = hungarian::solve(testArr);
    std::cout << "---------\nOptimal assignment = \n" << optimalAssignment << "\n";
    return 0;
}