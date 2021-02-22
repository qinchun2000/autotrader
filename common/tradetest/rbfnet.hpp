#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#ifndef _RBFNET_HPP
#define _RBFNET_HPP

class RbfNet
{

public:
	RbfNet();
	float rbf1(float row,float rad);
	float rbf2(float row,float rad);
	int c2MAT(int i,int j,int ROWS);
	void rbfTrain(int numVect,int numData,int numDelay,float *DataVect,float radius);

};

#endif /* _RBFNET_HPP */
