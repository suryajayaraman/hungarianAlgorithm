#ifndef _HUNGARIAN_H_
#define _HUNGARIAN_H_

#include "eigen3/Eigen/Dense"
#include <stdint.h>
#include <iostream>
// #include "hungarianAlgorithm/utils.h"


typedef Eigen::VectorXd  vecXd;
typedef Eigen::MatrixXd matXd;


class hungarian
{
    public:
        hungarian(void);
        ~hungarian(void);

        static Eigen::VectorXi solve(const matXd &costMtx);

    private:
        /**
         * @brief utility function to check if input matrix is empty
         * @param costMtx 
         * @return true if costMtx is empty
         * @return false if costMtx.size() > 0 and costMtx[0].size() > 0
         */
        static bool checkEmptyMtx(const matXd &costMtx);


        /**
         * @brief function does row min subtraction - step1 of hungarian algorithm
         * @param costMtx (matXd) input Matrix
         * @param nRows (uint16_t) number of rows
         * @param nCols (uint16_t) number of columns
         * @return NA
         */
        static void step1(matXd &costMtx, const uint16_t &nRows, const uint16_t &nCols);


        /**
         * @brief function creates masked matrix based on presence of
         * starred zeros in every row, column combination
         * @param costMtx input Matrix
         * @param nRows (uint16_t) number of rows
         * @param nCols (uint16_t) number of columns
         * @return mtxMask (std::vector<std::vector<bool>>) 
         */
        static std::vector<std::vector<bool>> step2(matXd &costMtx, const uint16_t &nRows, const uint16_t &nCols);


        /**
         * @brief function checks if all columns of mask matrix have starred zero
         * If true, we have found optimal assignment, else we proceed to step4
         * @param costMtx input Matrix
         * @param nRows (uint16_t) number of rows
         * @param nCols (uint16_t) number of columns
         * @return NA
         */
        static void step3(std::vector<std::vector<bool>> &mtxMask, const uint16_t &nRows, const uint16_t &nCols);

};


#endif // _HUNGARIAN_H_