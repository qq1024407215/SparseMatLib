
#include "MatLib/TripletStore/TripletMatrix.h"
#include "MatLib/TripletStore/Triplet.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

namespace MatLib
{

namespace TripletStore
{

//constructor
//li
TripletMatrix::TripletMatrix()
{
    matrixWidth = 0; matrixHeight = 0;
    nonZeroNum = 0;
    //std::cout<< "TripleMatrix Constructed!" << '\n';
}


void TripletMatrix::createMatrix(unsigned int width, unsigned int height, unsigned int nonZero)
{
    matrixWidth = width; matrixHeight = height;
    nonZeroNum = nonZero;
}

void TripletMatrix::destroyMatrix()
{
    matrixWidth = 0; matrixHeight = 0;
    nonZeroNum = 0;
    for(int i = 0; i < MAXSIZE && data[i].getRowNum()!= 0; i++)
    {
       data[i].setZero();
    }
}

void TripletMatrix::insertTripletToMatrix(Triplet insertTriplet)
{
    /* TODO: Insert a Triple into the right place of matrix table
     * if the position is not 0, return ERROR
     */
    int i, j;
    for(i = 0; i < MAXSIZE && data[i].getRowNum()!= 0 &&
    ((data[i].getRowNum() < insertTriplet.getRowNum())|| (data[i].getRowNum() == insertTriplet.getRowNum() && data[i].getColNum() <= insertTriplet.getColNum()))\
    ; i++)
    {
        if((data[i].getRowNum() == insertTriplet.getRowNum() && data[i].getColNum() == insertTriplet.getColNum()))
        {
            std::cout << "Element Exists. Insert Failed."<< '\n';
            return;
        }

    }
    while(data[j].getRowNum()!=0) {j++;}
    for(; j> i; j--)
    {
        data[j] = data[j-1];
    }

    data[j] = insertTriplet;
    nonZeroUpdate();
}

void TripletMatrix::nonZeroUpdate()
{
    unsigned int i;
    for(i = 0; i < MAXSIZE && data[i].getRowNum()!=0 ;i++){;}
    nonZeroNum = i;
}

void TripletMatrix::displayTable() const
{
    /*
     * TODO: Display triple table of a matrix
     */
    int i;
    std::cout << "Matrix Table Print:" << '\n';
    for(i = 0; i<MAXSIZE && data[i].getRowNum()!= 0; i++)
    {
        data[i].displayTriplet();
    }
}

void TripletMatrix::printMatrix() const
{
    /*
     * TODO: Print matrix.
     */
    unsigned int i, j, k, flag;
    k = 0;
    std::cout << "TripleMatrix Print:" << '\n';
    for(i = 0; i < matrixWidth; i++)
    {
        for(j = 0; j < matrixHeight; j++)
        {
            for(k = 0; k < MAXSIZE && data[k].getRowNum()!=0; k++)
            {
                flag = 0;
                if(data[k].getRowNum() == i+1 && data[k].getColNum() == j+1 )
                {
                    std::cout << data[k].getValue(); flag = 1;
                    break;
                }
            }
            if(flag == 0) {std::cout << 0;} else {;}
            std::cout << '\t';
        }
        std::cout << '\n';
    }

}

unsigned int TripletMatrix::getMatrixWidth() const
{
    return matrixWidth;
}
unsigned int TripletMatrix::getMatrixHeight() const
{
    return matrixHeight;
}
unsigned int TripletMatrix::getMatrixNonZeroNum() const
{
    return nonZeroNum;
}


// Operators Overload

void TripletMatrix::operator=(const TripletMatrix &M)
{
    matrixWidth = M.getMatrixWidth(); matrixHeight = M.getMatrixHeight();
    nonZeroNum = M.getMatrixNonZeroNum();
    int i;
    for( i = 0; i < MAXSIZE && M.data[i].getRowNum(); i++)
    {
        data[i] = M.data[i];
    }
    while(data[i].getRowNum()!=0)
    {
        data[i].modifyTriplet(0,0,0);
    }

}

bool TripletMatrix::operator==(const TripletMatrix &M) const
{
    if((matrixHeight == M.getMatrixHeight()) && (matrixWidth == M.getMatrixWidth())){;}
    else return false;
    int i;
    for(i = 0; i<MAXSIZE && (data[i].getRowNum() || M.data[i].getRowNum()); i++)
    {
        if(!(data[i] == M.data[i]))
        {
            return false;
        }
    }
    return true;
    //lixie
}

TripletMatrix TripletMatrix::operator+(const TripletMatrix &M)
{
    if(this->matrixWidth != M.matrixWidth || this->matrixHeight != M.matrixHeight)
    {
        std::cout << "Unable To Add" << '\n';
        return *this;
    }
    TripletMatrix Temp; Temp.createMatrix(this->matrixWidth, this->matrixHeight, 0);
    unsigned int i = 0;  unsigned int j = 0; unsigned int k = 0;
    while((this->data[i].getRowNum()!=0) && (M.data[j].getRowNum()!=0))
    {
        if(this->data[i].getRowNum() < M.data[j].getRowNum())
        {
            Temp.data[k] = this->data[i];
            i++; k++;
        }
        else if(this->data[i].getRowNum() == M.data[j].getRowNum())
        {
            if(this->data[i].getColNum() < M.data[j].getColNum() )
            {
                Temp.data[k] = this->data[i];
                i++; k++;
            }
            else if(this->data[i].getColNum() == M.data[j].getColNum())
            {
                if(this->data[i].getValue()+M.data[j].getValue() != 0)
                {
                    Temp.data[k] = this->data[i] + M.data[j];
                    k++;
                }
                i++; j++;
            }
            else if(this->data[i].getColNum() > M.data[j].getColNum())
            {
                Temp.data[k] = M.data[j];
                j++; k++;
            }
            else {std::cout << "ERROR" << '\n';}
        }
        else if(this->data[i].getRowNum() > M.data[j].getRowNum())
        {
            Temp.data[k] = M.data[j];
            j++; k++;
        }
        else {std::cout << "ERROR" << '\n';}

    }
    while((this->data[i].getRowNum()!=0) && i<MAXSIZE)
    {
        Temp.data[k] = this->data[i];
        i++; k++;
    }
    while(M.data[j].getRowNum()!=0 && j<MAXSIZE)
    {

        Temp.data[k] = M.data[j];
        j++; k++;
    }
    Temp.nonZeroUpdate();
    return Temp;

}

TripletMatrix TripletMatrix::getNegMatrix() const
{
    TripletMatrix NegMat;
    NegMat.createMatrix(this->getMatrixWidth(), this->getMatrixHeight(), this->getMatrixNonZeroNum());
    for(int i = 0; i < MAXSIZE && (this->data[i].getColNum()!=0); i++)
    {
        NegMat.data[i].modifyTriplet(this->data[i].getRowNum(),data[i].getColNum(),-(this->data[i].getValue()));
    }
    return NegMat;
}

TripletMatrix TripletMatrix::operator*(const TripletMatrix &M)
{
    TripletMatrix Temp;
    if(this->getMatrixHeight() != M.getMatrixWidth() || this->getMatrixWidth() == 0 || M.getMatrixHeight() == 0)
    {
        std::cout<< "ERROR in Multiplication." << '\n';
        return *this;
    }
    Temp.createMatrix(this->getMatrixWidth(), M.getMatrixHeight(), 0);
    //array used to store intermediate result
    int *heightArray;
    heightArray = (int *)malloc((M.getMatrixHeight()+1)*sizeof(int ));

    int tableRow = 0;
    while(this->data[tableRow].getRowNum()!=0)//Traverse the index of Triple matrix
    {
        for(unsigned int i = 0; i <= M.getMatrixHeight(); i++)
        {
            heightArray[i] = 0;
        }
        unsigned int x = this->data[tableRow].getRowNum();
        while(x == this->data[tableRow].getRowNum())
        {
            for(int j = 0; j < MAXSIZE && M.data[j].getRowNum()!=0; j++)
            {
                if(this->data[tableRow].getColNum() == M.data[j].getRowNum())
                {
                    //Multiply corresponding unit
                    heightArray[M.data[j].getColNum()] += (this->data[tableRow].getValue()*M.data[j].getValue());

                }
                else {;}
            }
            tableRow++;

        }
        for(unsigned int k = 1; k <= M.getMatrixHeight(); k++)
        {

            if(heightArray[k]!= 0)
            {
                Triplet ins; ins.modifyTriplet(x, k, heightArray[k]);
                Temp.insertTripletToMatrix(ins);
            }
        }

    }

    free(heightArray);
    return Temp;
}

TripletMatrix TripletMatrix::operator-(const TripletMatrix &M)
{
    TripletMatrix Temp;
    Temp = *this+M.getNegMatrix();


    return Temp;
}
//destructor
//xie
TripletMatrix::~TripletMatrix()
{
    //std::cout << "TripleMatrix Destructed." << '\n';
}

}//End Namespace Triple

}//End Namespace MatLib