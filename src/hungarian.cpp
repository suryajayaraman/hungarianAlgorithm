#include "hungarianAlgorithm/hungarian.h"



Eigen::VectorXi hungarian::solve(const matXd &costMtx, double *costHandle)
{
    Eigen::VectorXi optimalAssgnVec { Eigen::VectorXi::Ones(costMtx.rows()) * -1};
    
    // check for empty matrix
    if(costMtx.size() > 0)
    {
        assignmentOptimal(costMtx, optimalAssgnVec, costHandle);
    }

    else
    {
        // do nothing
    }
    
    return optimalAssgnVec;
}


void hungarian::assignmentOptimal(const matXd &costMtx, 
                                  Eigen::VectorXi &optimalAssgnVec, 
                                  double* costHandle)
{
    // placeholders for temporary variables
    vecXbool coveredRows {vecXbool::Zero(costMtx.rows())};
    vecXbool coveredCols {vecXbool::Zero(costMtx.cols())};

    matXbool starMtx {matXbool::Zero(costMtx.rows(), costMtx.cols())};
    matXbool newStarMtx {matXbool::Zero(costMtx.rows(), costMtx.cols())};
    matXbool primeMtx {matXbool::Zero(costMtx.rows(), costMtx.cols())};

    // create copy of cost matrix
    matXd costMtxTemp {costMtx};

    // subtract row wise minimum
    vecXd rowWiseMin = costMtxTemp.rowwise().minCoeff();
    costMtxTemp.colwise() -= rowWiseMin;
    // std::cout << "after rowmin subtraction, costMtx = \n" << costMtxTemp << "\n";

    // iterate through elements and try columnwise assignment
    for(int row =0; row < costMtxTemp.rows(); row++)
    {
        for(int col =0; col < costMtxTemp.cols(); col++)
        {
            // check for zero in uncovered columns
            if((costMtxTemp(row, col) == 0.0) && (coveredCols(col) == false))
            {
                // std::cout << row << "," << col << "\n";
                // star the found zero and enter in coveredColumns
                starMtx(row, col) = true;
                coveredCols(col) = true;
            }
        }
    }    
    // try assigning to rows
    step2b(costMtxTemp, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}



void hungarian::buildAssignmentVec(matXbool &starMtx, vecXbool &coveredCols,
                                   Eigen::VectorXi &optimalAssgnVec)
{
    for(int row=0; row < starMtx.rows(); row++)
    {
        for(int col=0; col < starMtx.cols(); col++)
        {
            if(starMtx(row,col) == true)
            {
                optimalAssgnVec(row) = col;
                break;
            }
        }
    }
}



void hungarian::step2a(matXd &costMtx, matXbool &starMtx,
                    matXbool &newStarMtx, matXbool &primeMtx,
                    vecXbool &coveredCols, vecXbool &coveredRows,
                    Eigen::VectorXi &optimalAssgnVec)
{
    std::cout << "---------\nstep 2a \n------------------\n";
    // std::cout << "costMtx = \n";
    // std::cout << costMtx << "\n";
    // std::cout << "starMtx  = \n";
    // std::cout << starMtx << "\n";
    // std::cout << "coveredCols = \n";
    // std::cout << coveredCols << "\n";
    // std::cout << "coveredRows  = \n";
    // std::cout << coveredRows << "\n";
    for(int col =0; col < starMtx.cols(); col++)
    {
        if(coveredCols(col) == false)
        {
            for(int row =0; row < starMtx.rows(); row++)
            {
                if(starMtx(row, col) == true)
                {
                    coveredCols(col) = true;
                    break;
                }
            }
        }
    }
    // std::cout << "coveredCols = \n";
    // std::cout << coveredCols << "\n";   
    step2b(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec); 
}



void hungarian::step2b(matXd &costMtx, matXbool &starMtx,
                       matXbool &newStarMtx, matXbool &primeMtx,        
                       vecXbool &coveredCols, vecXbool &coveredRows,
                       Eigen::VectorXi &optimalAssgnVec)
{
    // check for terminating condition of covered columns
    // equalling total number of columns
    int noCoveredCols = coveredCols.cast<int>().sum();
    std::cout << "noCoveredCols = " << noCoveredCols << "\n";

    // terminating condition reched, build the assignment vector
    if(noCoveredCols == costMtx.cols())
    {
        buildAssignmentVec(starMtx, coveredCols, optimalAssgnVec);
    }

    // covered columns < total columns
    // proceed to next step
    else
    {
        step3(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
    }
}


void hungarian::step3(matXd &costMtx, matXbool &starMtx,
           matXbool &newStarMtx, matXbool &primeMtx,
           vecXbool &coveredCols, vecXbool &coveredRows,
           Eigen::VectorXi &optimalAssgnVec)
{
    bool zerosFound = true;
    while(zerosFound == true)
    {
        zerosFound = false;
        for(int col =0; col < costMtx.cols(); col++)
        {
            if(coveredCols(col) == false)
            {
                for(int row =0; row < costMtx.rows(); row++)
                {
                    if((coveredRows(row) == false) && (costMtx(row,col) == 0.0))
                    {
                        // std::cout << "found uncovered zero at " << row << "," << col <<"\n";
                        primeMtx(row, col) = true;

                        int starCol = 0;
                        for(starCol=0; starCol < starMtx.cols(); starCol++)
                        {
                            if(starMtx(row, starCol) == true)
                                break;
                        }
                        // std::cout << "found starred zero at " << starCol <<"\n";

                        if(starCol == starMtx.cols())
                        {
                           step4(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, 
                                 optimalAssgnVec, row, col);
                           return;
                        }
                        
                        else
                        {
                            coveredRows(row) = true;
                            coveredCols(starCol) = false;
                            zerosFound = true;
                        }
                    }
                }
            }
        }
        // std::cout << "coveredCols = \n";
        // std::cout << coveredCols << "\n";
        // std::cout << "coveredRows  = \n";
        // std::cout << coveredRows << "\n";
    }
    // reconstruct cost matrix
    step5(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}



void hungarian::step4(matXd &costMtx, matXbool &starMtx,
           matXbool &newStarMtx, matXbool &primeMtx,        
           vecXbool &coveredCols, vecXbool &coveredRows,
           Eigen::VectorXi &optimalAssgnVec,
           const int &starRow, const int &starCol)
{
    // std::cout << "---------\nstep 4 \n------------------\n";
    // std::cout << "costMtx = \n";
    // std::cout << costMtx << "\n";
    // std::cout << "starMtx  = \n";
    // std::cout << starMtx << "\n";
    // std::cout << "newStarMtx = \n";
    // std::cout << newStarMtx << "\n";
    // std::cout << "primeMtx = \n";
    // std::cout << primeMtx << "\n";
    // std::cout << "coveredCols = \n";
    // std::cout << coveredCols << "\n";
    // std::cout << "coveredRows  = \n";
    // std::cout << coveredRows << "\n";
    // std::cout << "starRow = " << starRow << ", starCol = " << starCol << "\n";

    // create a copy of the existing star matrix and
    // star the found zero
    newStarMtx = starMtx;
    newStarMtx(starRow, starCol) = true;

    // find previous starred zero in current column
    int previousStarRow = 0;
    for(; previousStarRow < starMtx.rows(); previousStarRow++)
        if(starMtx(previousStarRow, starCol) == true)
            break;
    
    // unstar old zero in newStarMtx
    newStarMtx(previousStarRow, starCol) = false;

    // find zero of previousStarRow in primMtx
    int primeZeroCol = 0;
    for(; primeZeroCol < primeMtx.cols(); primeZeroCol++)
        if(primeMtx(previousStarRow, primeZeroCol) == true)
            break;
    newStarMtx(previousStarRow, primeZeroCol) = true;
    std::cout << "new star matrix = \n" << newStarMtx << "\n";

    // reassign new star matrix to old matrix
    // reset prime matrix and covered rows
    starMtx = newStarMtx;
    primeMtx = matXbool::Zero(primeMtx.rows(), primeMtx.cols());
    coveredRows = vecXbool::Zero(costMtx.rows());

    step2a(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}



void hungarian::step5(matXd &costMtx, matXbool &starMtx,
           matXbool &newStarMtx, matXbool &primeMtx,
           vecXbool &coveredCols, vecXbool &coveredRows,
           Eigen::VectorXi &optimalAssgnVec)
{
    // std::cout << "---------\nstep 5 \n------------------\n";
    // std::cout << "costMtx = \n";
    // std::cout << costMtx << "\n";
    // std::cout << "starMtx  = \n";
    // std::cout << starMtx << "\n";
    // std::cout << "newStarMtx = \n";
    // std::cout << newStarMtx << "\n";
    // std::cout << "primeMtx = \n";
    // std::cout << primeMtx << "\n";
    // std::cout << "coveredCols = \n";
    // std::cout << coveredCols << "\n";
    // std::cout << "coveredRows  = \n";
    // std::cout << coveredRows << "\n";

    // find minimum of uncovered value in costMatrix
    double minValue = __DBL_MAX__;
    for(int col =0; col < costMtx.cols(); col++)
        if(coveredCols(col) == false)
            for(int row =0; row < costMtx.rows(); row++)
                if(coveredRows(row) == false)
                    if(costMtx(row,col) < minValue)
                        minValue = costMtx(row,col);
    std::cout << "minUncovered value = " << minValue << "\n";

    // subtract minValue from uncovered columns
    for(int col=0; col < costMtx.cols(); col++)
        if(coveredCols(col) == false)
            costMtx.col(col).array() -= minValue;

    // add minValue to covered rows
    for(int row=0; row < costMtx.rows(); row++)
        if(coveredRows(row) == true)
            costMtx.row(row).array() += minValue;

    std::cout << "After reconstruction, costMtx = \n";
    std::cout << costMtx << "\n";

    // try covering all zeros on reconstructed matrix
    step3(costMtx, starMtx, newStarMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}