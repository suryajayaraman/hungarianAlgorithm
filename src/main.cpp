#include <iostream>
#include "hungarianAlgorithm/hungarian.h"



int main(int argc, char *argv[])
{
    // test inputs
    matXd costMtx (4,4);
    costMtx << 5,  9, 3,  6,
               8,  7, 8,  2,
               6, 10, 12, 7,
               3, 10, 8,  6;
    std::cout << "costMtx = \n" << costMtx << "\n";               
    Eigen::VectorXi optimalAssgnVec { Eigen::VectorXi::Ones(costMtx.rows()) * -1};

    // create object to find optimal assignment
    hungarian costFinderObj;
    double cost = costFinderObj.solve(costMtx, optimalAssgnVec);
    
    // print result
    std::cout << "---------\nOptimal assignment = \n" << optimalAssgnVec << "\n";
    std::cout << "Assignment cost = " << cost << "\n";
    return 0;
}