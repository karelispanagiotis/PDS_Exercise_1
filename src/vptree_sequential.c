//vptree_sequential.c
#include "vptree.h"
#include "details.h"
#include <stdlib.h>
#include <math.h>

void recursiveBuildTree(
    vptree* node, 
    int start, int end, 
    int n, int d, 
    int idArr[n], 
    double distArr[n], 
    double dataArr[n][d])
{
    //consider X[ idArr[start] ] as vantage point
    node->idx = idArr[start];
    node->vp  = dataArr[ node->idx ]; 
    
    if (start==end)
    {
        node->inner = node->outer = NULL;
        node->md = 0.0;
        return;
    }
    
    for (int i=start; i<end; i++)
        distArr[i] = sqr(node->vp[0]-dataArr[idArr[start]][0]);
    for (int i=start; i<end; i++)
        for (int j=1; j<d; j++)
            distArr[i] += sqr(node->vp[j]-dataArr[idArr[start]][j]);
    
    quickSelect( (end-start)/2, distArr, idArr, start, end );
    // now idArr[start .. (start+end)/2] contains the indexes
    // for the points which belong inside the radius (inner)

    node->md  = distArr[ (start+end)/2 ];
    node->inner = malloc( sizeof(vptree) );
    node->outer = malloc( sizeof(vptree) );
    recursiveBuildTree(node->inner, start, (start+end)/2, n, d, idArr, distArr, dataArr);
    recursiveBuildTree(node->outer, (start+end)/2 +1, end,n, d, idArr, distArr, dataArr);
};

/////////////////////////////////////////////////////////////////////////////

vptree *buildvp(double *X, int n, int d)
{
    vptree *root = malloc( sizeof(vptree) );
    int *idArr = malloc( n*sizeof(int) );
    double *distArr = malloc ( n*sizeof(double) );
    
    for (int i=0; i<n; i++) idArr[i] = i;
    recursiveBuildTree(root, 0, n-1, n, d, idArr, distArr,  (double (*)[d])X );
    
    free(idArr);
    free(distArr);
    return root;
}

/////////////////////////////////////////////////////////////////////////////