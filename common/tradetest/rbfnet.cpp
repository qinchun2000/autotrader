#include "rbfnet.hpp"

RbfNet::RbfNet()
{

}

float RbfNet::rbf1(float row,float rad)
{
	float r=exp(-row/rad);

	//if(r<=0.0000005) r=0.0000005;
	return r;
}

float RbfNet::rbf2(float row,float rad)
{
	float r;
	if(row>=rad)
		r=0;
	else
		r=1.0-row/rad;
	return r;
}

//define
int RbfNet::c2MAT(int i,int j,int ROWS)
{
return (i+j*ROWS);
}

void RbfNet::rbfTrain(int numVect,int numData,int numDelay,float *DataVect,float radius)
{
	int numInp,numHid,numOut,numVects;
	// num of input,hidden output layers and number fo input vects
	float *AL,*BL,*C1L,*W2L;
	float *InpX,*TargY;
	float tmp,*rad;

	//Tranfer sample sata
	numInp = numVect*numDelay;
	numOut = numVect;

	numVects =numData-numDelay;
	numHid = numVects;

	//Memory allocate
	// the memory of input training data
	InpX =(float*) malloc(numInp*numVects*sizeof(float));
	//the memory of Target training data
	TargY =(float*) malloc(numOut*numVects*sizeof(float));
	//creating C1
	C1L =(float*) malloc(numHid*numInp*sizeof(float));
	//creating W2
	W2L =(float*) malloc(numHid*numOut*sizeof(float));

	//creating AL
	AL =(float*) malloc(numHid*numOut*sizeof(float));
	//creating BL
	BL =(float*) malloc(numHid*sizeof(float));
	//creating rad
	rad =(float*) malloc(numHid*numOut*sizeof(float));



	//save testing data

	FILE * OutFile;
	std::string FileName1;
	OutFile =fopen(FileName1.c_str(),"w+");

	for(int i=0;i<numData;i++)	{

		for (int j=0;j<numVect;j++){
			fprintf(OutFile,"%f",DataVect[c2MAT(j,i,numVect)]);

		}
		fprintf(OutFile,"\n");
	}

	fclose(OutFile);


	for(int i=0;i<numVects;i++)	{
		for(int j=0;j<numInp;j++)	{
			InpX[j+i*numInp]= DataVect[j+i*numVect];
		}
		for(int j=0;i<numOut;j++)	{
			TargY[j+i*numOut] = DataVect[j+i*numVect+numInp];
		}
	}


	//begin train
	for(int i=0;i<numInp;i++) C1L[i]=InpX[i];
	for(int i=0;i<numOut;i++) AL[i]=TargY[i];
	BL[0]=1;
	// for(i=0;i<numOut;i++) W2L[i]=AL[i]/BL[0];
	// radius=0.2;
	numHid=1;

	for(int k=1;k<numVects;k++){

		for(int i=0;i<numHid;i++){
			rad[i]=0;
			for(int j=0;j<numInp;j++){
				tmp = InpX[c2MAT(j,k,numInp)]-C1L[c2MAT(j,i,numInp)];
				rad[i]+=tmp*tmp;
			}

		}

		tmp=rad[0];
		int l=0;
		for(int i=0;i<numHid;i++){
			if(rad[i]<tmp){
				tmp=rad[i];
				l=i;
			}
		}

		if(rad[l]<radius){
			for(int i=0;i<numOut;i++){
				AL[c2MAT(i,l,numOut)]=AL[c2MAT(i,l,numOut)]+TargY[c2MAT(i,k,numOut)];
			}
			BL[l]=BL[l]+1;
			//
			//
		}
		else
		{
			numHid=numHid+1;
			for(int i=0;i<numInp;i++){
				C1L[c2MAT(i,numHid-1,numInp)]=InpX[c2MAT(i,k,numInp)];
			}
			for(int i=0;i<numOut;i++){
				AL[c2MAT(i,numHid-1,numOut)]=TargY[c2MAT(i,k,numOut)];
			}
			BL[numHid-l]=l;
			//for(int i==0;i<numOut;i++)
			//W2L[c2MAT(i,numHid-l,numOut)]=AL[c2MAT(i,numHid-l,numOut)]/BL[numHid-l];
		}

	}
	//save net
	std::string FileName;
	OutFile =fopen(FileName.c_str(),"w+");
}




