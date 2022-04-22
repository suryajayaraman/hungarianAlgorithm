#include <iostream>
#include "hungarianAlgorithm/hungarian.h"



int main(int argc, char *argv[])
{
    matXd costMtx (4,4);
    costMtx << 5,  9, 3,  6,
               8,  7, 8,  2,
               6, 10, 12, 7,
               3, 10, 8,  6;
    std::cout << "costMtx = \n" << costMtx << "\n";               
    
    double* cost = nullptr;

    hungarian costFinderObj;
    Eigen::VectorXi optimalAssignment = costFinderObj.solve(costMtx, cost);
    std::cout << "---------\nOptimal assignment = \n" << optimalAssignment << "\n";
    return 0;
}