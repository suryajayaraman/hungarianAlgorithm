#ifndef _HUNGARIAN_H_
#define _HUNGARIAN_H_

#include <iostream>
#include "eigen3/Eigen/Dense"


typedef Eigen::VectorXd  vecXd;
typedef Eigen::MatrixXd matXd;
typedef Eigen::Matrix<bool,-1, 1> vecXbool;
typedef Eigen::Matrix<bool,-1,-1> matXbool;


class hungarian
{
    public:
        hungarian() {};
        ~hungarian() {};

        Eigen::VectorXi solve(const matXd &costMtx, double *cost);

    private:
        void assignmentOptimal(const matXd &costMtx, 
                               Eigen::VectorXi &optimalAssgnVec, 
                               double* costHandle);
        
        void buildAssignmentVec(matXbool &starMtx, vecXbool &coveredCols,
                    Eigen::VectorXi &optimalAssgnVec);

        void step2a(matXd &costMtx, matXbool &starMtx,
                    matXbool &newStarMtx, matXbool &primeMtx,
                    vecXbool &coveredCols, vecXbool &coveredRows,
                    Eigen::VectorXi &optimalAssgnVec);

        void step2b(matXd &costMtx, matXbool &starMtx,
                    matXbool &newStarMtx, matXbool &primeMtx,        
                    vecXbool &coveredCols, vecXbool &coveredRows,
                    Eigen::VectorXi &optimalAssgnVec);
        
        void step3(matXd &costMtx, matXbool &starMtx,
                   matXbool &newStarMtx, matXbool &primeMtx,        
                   vecXbool &coveredCols, vecXbool &coveredRows,
                   Eigen::VectorXi &optimalAssgnVec);

        void step4(matXd &costMtx, matXbool &starMtx,
                   matXbool &newStarMtx, matXbool &primeMtx,        
                   vecXbool &coveredCols, vecXbool &coveredRows,
                   Eigen::VectorXi &optimalAssgnVec,
                   const int &starRow, const int &starCol);

        void step5(matXd &costMtx, matXbool &starMtx,
                   matXbool &newStarMtx, matXbool &primeMtx,        
                   vecXbool &coveredCols, vecXbool &coveredRows,
                   Eigen::VectorXi &optimalAssgnVec);
};


#endif // _HUNGARIAN_H_