/**
 * @file hungarian.h
 * @author J Surya (suryajayaraman97@gmail.com)
 * @brief File contains Hungarian algorithm implementation. The
 * work is majorly a copy of the following amazing  repo :
 * https://github.com/mcximing/hungarian-algorithm-cpp
 * 
 * CHANGES FROM REFERENCE
 * 1. Implemented using `Eigen` library for Array operations
 * 2. Added docstring and comments for better understanding
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _HUNGARIAN_H_
#define _HUNGARIAN_H_

#include <iostream>             // printing to console
#include <cfloat>               // for DBL_MAX
#include <cmath>                // for fabs()
#include "eigen3/Eigen/Dense"   // for matrix operations


// useful typedefs
typedef Eigen::VectorXd  vecXd;
typedef Eigen::MatrixXd matXd;
typedef Eigen::Matrix<bool,-1, 1> vecXbool;
typedef Eigen::Matrix<bool,-1,-1> matXbool;



class hungarian
{
    public:
        /**
         * @brief Construct a new hungarian object
         * 
         */
        hungarian() {};

        /**
         * @brief Destroy the hungarian object
         * 
         */
        ~hungarian() {};


        /**
         * @brief Function solves the assignment problem using Hungarian algorithm
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param optimalAssgnVec column vector where each row entry indicates 
         * column optimal for that row
         * @return double Cost for the optimal assignment
         */
        double solve(const matXd &costMtx, Eigen::VectorXi &optimalAssgnVec);


    private:
        /**
         * @brief Function performs different steps of Hungarian algorithm
         * and calculates optimal assignment vector and cost
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param optimalAssgnVec column vector where each row entry indicates 
         * column optimal for that row
         * @param cost Cost for the optimal assignment
         * @return void
         */
        void assignmentOptimal(const matXd &costMtx, 
                               Eigen::VectorXi &optimalAssgnVec, 
                               double* cost);


        /**
         * @brief Function constructs the final assignment vector 
         * from the input starMtx. Every row in starMtx (same shape
         * as cost matrix) has 1 entry = true. The corresponding 
         * column is the optimal assignment for the row. Every row 
         * and column of star Matrix must sum up to exactly 1
         * for valid assignments. eg: 
         * starMtx = [[0, 0, 1, 0],
         *            [0, 0, 0, 1],
         *            [0, 1, 0, 0],
         *            [1, 0, 0, 0]]
         * corresponding optimalAssgnVec = [2, 3, 1, 0]
         * 
         * @param starMtx optimal assignment matrix as boolean 
         * @param optimalAssgnVec integer vector containing column index
         * of optimal assignment for each row
         * @return void
         */
        void buildAssignmentVec(matXbool &starMtx, Eigen::VectorXi &optimalAssgnVec);


        /**
         * @brief Function calculates the cost for input assignment
         * costMtx = [[5,  9,  3, 6],
         *            [8,  7,  8, 2],
         *            [6, 10, 12, 7],
         *            [3, 10,  8, 6]]
         * optimalAssgnVec = [2, 3, 1, 0]
         * cost = 3(1st row) + 2(2nd row) + 10(3rd row) + 3(4th row) = 18
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param optimalAssgnVec integer vector containing column index
         * of optimal assignment for each row
         * @param cost total cost for given assignment and cost matrix
         * @return void
         */
        void calculateCost(const matXd &costMtx, Eigen::VectorXi &optimalAssgnVec, 
                           double *cost);


        /**
         * @brief Function calculates coveredCols based on entires in starMtx
         * If any of the rows contains a starred zero, then that column is 
         * considered as covered. i.e. assignment for that column is calculated
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param starMtx optimal assignment matrix as boolean 
         * @param primeMtx 
         * @param coveredCols boolean vector indicating if a column is covered / not
         * @param coveredRows boolean vector indicating if a row is covered / not
         * @param optimalAssgnVec integer vector containing column index
         * of optimal assignment for each row
         * @return void
         */
        void step2a(matXd &costMtx, matXbool &starMtx,
                    matXbool &primeMtx, vecXbool &coveredCols, 
                    vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec);


        /**
         * @brief Function checks if assignments has been made for all columns
         * If so, it proceeds to build the assignment vector, else it proceeds
         * to next step in the algorithm
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param starMtx optimal assignment matrix as boolean 
         * @param primeMtx 
         * @param coveredCols boolean vector indicating if a column is covered / not
         * @param coveredRows boolean vector indicating if a row is covered / not
         * @param optimalAssgnVec integer vector containing column index
         * of optimal assignment for each row
         * @return void
         */
        void step2b(matXd &costMtx, matXbool &starMtx,
                    matXbool &primeMtx, vecXbool &coveredCols, 
                    vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec);
        
        /**
         * @brief Function finds any uncovered zeros in costMatrix and 
         * marks their position. If that row contains any previously starred
         * zero, it demarks it. This goes on until all zeros are covered
         * 
         * @param costMtx input cost matrix where c_ij indicates cost associated 
         * with ith row and jth column 
         * @param starMtx optimal assignment matrix as boolean 
         * @param primeMtx 
         * @param coveredCols boolean vector indicating if a column is covered / not
         * @param coveredRows boolean vector indicating if a row is covered / not
         * @param optimalAssgnVec integer vector containing column index
         * of optimal assignment for each row
         * @return void
         */
        void step3(matXd &costMtx, matXbool &starMtx,
                   matXbool &primeMtx, vecXbool &coveredCols, 
                   vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec);

        void step4(matXd &costMtx, matXbool &starMtx,
                   matXbool &primeMtx, vecXbool &coveredCols, 
                   vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec,
                   const int &starRow, const int &starCol);

        void step5(matXd &costMtx, matXbool &starMtx,
                   matXbool &primeMtx, vecXbool &coveredCols, 
                   vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec);
};


#endif // _HUNGARIAN_H_