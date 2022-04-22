#include "hungarianAlgorithm/hungarian.h"

/********************************************************/

double hungarian::solve(const matXd &costMtx, Eigen::VectorXi &optimalAssgnVec)
{
    // initialize return variable with -1;
    double cost = -1;

    // check for empty matrix
    if(costMtx.size() > 0)
    	// call solving function
        assignmentOptimal(costMtx, optimalAssgnVec, &cost);
    
    else {} // do nothing
    return cost;
}

/********************************************************/

void hungarian::assignmentOptimal(const matXd &costMtx, 
                                  Eigen::VectorXi &optimalAssgnVec, 
                                  double* cost)
{
    // boolean arrays for indicating which row / column is covered
    // meaning it contains an assignment
    vecXbool coveredRows {vecXbool::Zero(costMtx.rows())};
    vecXbool coveredCols {vecXbool::Zero(costMtx.cols())};

    // placeholders for temporary variables
    matXbool starMtx {matXbool::Zero(costMtx.rows(), costMtx.cols())};
    matXbool primeMtx {matXbool::Zero(costMtx.rows(), costMtx.cols())};

    // create copy of cost matrix
    matXd costMtxTemp {costMtx};

    // subtract row wise minimum
    vecXd rowWiseMin = costMtxTemp.rowwise().minCoeff();
    costMtxTemp.colwise() -= rowWiseMin;

    // iterate through elements and try columnwise assignment
    for(int row =0; row < costMtxTemp.rows(); row++)
        for(int col =0; col < costMtxTemp.cols(); col++)
            // check for zero in uncovered columns
            if((fabs(costMtxTemp(row, col)) < DBL_EPSILON) && (coveredCols(col) == false))
            {
                // star the found zero and enter in coveredColumns
                starMtx(row, col) = true;
                coveredCols(col) = true;
                break;
            }

    // try assigning to rows
    step2b(costMtxTemp, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
    
    // calculate cost of the optimal assignment
    calculateCost(costMtx, optimalAssgnVec, cost);
}

/********************************************************/

void hungarian::buildAssignmentVec(matXbool &starMtx, Eigen::VectorXi &optimalAssgnVec)
{
    for(int row=0; row < starMtx.rows(); row++)
        for(int col=0; col < starMtx.cols(); col++)
            if(starMtx(row,col) == true)
            {
                optimalAssgnVec(row) = col;
                break;
            }
}

/********************************************************/

void hungarian::calculateCost(const matXd &costMtx, 
        Eigen::VectorXi &optimalAssgnVec, double *cost)
{
    // initialze cost to 0
    *cost = 0.0F;

    // temp placeholder
    int col;
    for(int row=0; row < costMtx.rows(); row++)
    {
        col = optimalAssgnVec(row);
        if((col >=0) && (col < costMtx.cols()))
            *cost += costMtx(row, optimalAssgnVec(row));
    }
}

/********************************************************/

void hungarian::step2a(matXd &costMtx, matXbool &starMtx,
                    matXbool &primeMtx, vecXbool &coveredCols, 
                    vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec)
{
    for(int col =0; col < starMtx.cols(); col++)
        if(coveredCols(col) == false)
            for(int row =0; row < starMtx.rows(); row++)
                if(starMtx(row, col) == true)
                {
                    coveredCols(col) = true;
                    break;
                }

    step2b(costMtx, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec); 
}

/********************************************************/

void hungarian::step2b(matXd &costMtx, matXbool &starMtx,
                       matXbool &primeMtx, vecXbool &coveredCols,
                       vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec)
{
    // check for terminating condition of covered columns
    // equalling total number of columns
    int noCoveredCols {coveredCols.cast<int>().sum() };

    // algorithm finished, build the assignment vector
    if(noCoveredCols == costMtx.cols())
        buildAssignmentVec(starMtx, optimalAssgnVec);

    // covered columns < total columns, proceed to next step
    else
        step3(costMtx, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}

/********************************************************/

void hungarian::step3(matXd &costMtx, matXbool &starMtx,
           matXbool &primeMtx, vecXbool &coveredCols, 
           vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec)
{
    // The following code snippet cover all the zeros with
    // minimum number of lines.
    bool zerosFound {true};
    while(zerosFound == true)
    {
        zerosFound = false;
        // iterate through cost matrix, looking for uncovered zeros
        for(int col =0; col < costMtx.cols(); col++)
            if(coveredCols(col) == false)
                for(int row =0; row < costMtx.rows(); row++)
                    if((coveredRows(row) == false) && (fabs(costMtx(row,col)) < DBL_EPSILON))
                    {
                        // keep track of the new uncovered zero found
                        primeMtx(row, col) = true;

                        // identify starred zero in current row, if available
                        int starCol = 0;
                        for(starCol=0; starCol < starMtx.cols(); starCol++)
                            if(starMtx(row, starCol) == true)
                                break;

                        // no starred zeros present, proceed to step4
                        if(starCol == starMtx.cols())
                        {
                           step4(costMtx, starMtx, primeMtx, coveredCols, coveredRows, 
                                 optimalAssgnVec, row, col);
                           return;
                        }
                        
                        // at least 1 starred zero present, demark it in coveredCols
                        // mark the corresponding row as covered
                        else
                        {
                            coveredRows(row) = true;
                            coveredCols(starCol) = false;
                            zerosFound = true;
                            break;
                        }
                    }
    }
    // reconstruct cost matrix
    step5(costMtx, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}

/********************************************************/

void hungarian::step4(matXd &costMtx, matXbool &starMtx,
           matXbool &primeMtx, vecXbool &coveredCols, 
           vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec,
           const int &starRow, const int &starCol)
{
    // create a copy of the existing star matrix and
    // star the found zero
    matXbool newStarMtx {starMtx};
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

    // reassign new star matrix to old matrix
    // reset prime matrix and covered rows
    starMtx = newStarMtx;
    primeMtx = matXbool::Zero(primeMtx.rows(), primeMtx.cols());
    coveredRows = vecXbool::Zero(costMtx.rows());

    // proceed to stepa where we calculate coveredCols
    // for the new star matrix
    step2a(costMtx, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}

/********************************************************/

void hungarian::step5(matXd &costMtx, matXbool &starMtx,
           matXbool &primeMtx, vecXbool &coveredCols,
           vecXbool &coveredRows, Eigen::VectorXi &optimalAssgnVec)
{
    // find minimum of uncovered value in costMatrix
    double minValue = __DBL_MAX__;
    for(int col =0; col < costMtx.cols(); col++)
        if(coveredCols(col) == false)
            for(int row =0; row < costMtx.rows(); row++)
                if(coveredRows(row) == false)
                    if(costMtx(row,col) < minValue)
                        minValue = costMtx(row,col);

    // subtract minValue from uncovered columns
    for(int col=0; col < costMtx.cols(); col++)
        if(coveredCols(col) == false)
            costMtx.col(col).array() -= minValue;

    // add minValue to covered rows
    for(int row=0; row < costMtx.rows(); row++)
        if(coveredRows(row) == true)
            costMtx.row(row).array() += minValue;

    // try covering all zeros on reconstructed matrix
    step3(costMtx, starMtx, primeMtx, coveredCols, coveredRows, optimalAssgnVec);
}