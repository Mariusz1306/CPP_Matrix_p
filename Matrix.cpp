#include <math.h>
#include <fstream>
#include "Matrix.h"

CMatrix::CMatrix(){
	block = new rcmatrix();
}

CMatrix::CMatrix(const CMatrix& matrix){
	matrix.block->ref_count++;
	block = matrix.block;
}

CMatrix::CMatrix(std::fstream& file){
	block = new rcmatrix(file);
}

CMatrix::CMatrix(unsigned int nrOfRows, unsigned int nrOfCols, double value){
    block = new rcmatrix(nrOfRows,nrOfCols,value);
}

CMatrix::~CMatrix(){
	if(--block->ref_count==0)
		delete block;
}

CMatrix& CMatrix::operator = (const CMatrix& m2){
	m2.block->ref_count++;
	if(--block->ref_count == 0)
		delete block;
	block=m2.block;
	return *this;
}

CMatrix operator * (const CMatrix& m1, const CMatrix& m2){
	if((m1.block->cols != m2.block->rows) && (m1.block->rows != m2.block->cols))
		throw WrongDim();

	CMatrix newMatrix(m1.block->rows, m2.block->cols,0.0);

	for(unsigned int currRow=0;currRow<newMatrix.block->rows;currRow++)
		for(unsigned int currCol=0;currCol<newMatrix.block->cols;currCol++){
			double var=0.0;
			for(unsigned int value=0;value<m1.block->cols;value++)
				var += m1.block->data[currRow][value]*m2.block->data[value][currCol];
			newMatrix.block->data[currRow][currCol] = var;
		}
	return newMatrix;
}

CMatrix operator + (const CMatrix& m1, const CMatrix& m2){
    if((m1.block->rows != m2.block->rows) && (m1.block->cols != m2.block->cols))
		throw WrongDim();

    CMatrix newMatrix(m1);
    newMatrix.block = newMatrix.block->detach();

	for(unsigned int i = 0; i < newMatrix.block->rows; i++){
		for(unsigned int j = 0; j < newMatrix.block->cols; j++){
			newMatrix.block->data[i][j] += m2.block->data[i][j];
		}
	}
    return newMatrix;
}

CMatrix operator - (const CMatrix& m1, const CMatrix& m2){
    if((m1.block->rows != m2.block->rows) && (m1.block->cols != m2.block->cols))
		throw WrongDim();

    CMatrix newMatrix(m1);
    newMatrix.block = newMatrix.block->detach();

	for(unsigned int i = 0; i < newMatrix.block->rows; i++){
		for(unsigned int j = 0; j < newMatrix.block->cols; j++){
			newMatrix.block->data[i][j] -= m2.block->data[i][j];
		}
	}
    return newMatrix;
}

CMatrix& CMatrix::operator *= (const CMatrix& m2){
	if(this->block->cols != m2.block->rows)
		throw WrongDim();

    this->block = this->block->detach();

    CMatrix newMatrix(this->block->rows, m2.block->cols,0.0);

	for(unsigned int i = 0; i < this->block->rows; i++){
		for(unsigned int j = 0; j < this->block->cols; j++){
			double var = 0.0;
			for(unsigned int k = 0; k < this->block->cols; k++)
				var += this->block->data[i][k] * m2.block->data[k][j];
			newMatrix.block->data[i][j] = var;
		}
	}

	for(unsigned int i = 0; i < this->block->rows; i++){
		for(unsigned int j = 0; j < this->block->cols; j++){
            this->block->data[i][j] = newMatrix.block->data[i][j];
		}
	}
	return *this;
}

CMatrix& CMatrix::operator += (const CMatrix& m2){
    if((this->block->rows != m2.block->rows) || (this->block->cols != m2.block->cols))
		throw WrongDim();

    this->block = this->block->detach();

	for(unsigned int i = 0; i < this->block->rows; i++){
		for(unsigned int j = 0; j < this->block->cols; j++){
			this->block->data[i][j] += m2.block->data[i][j];
		}
	}
    return *this;
}

CMatrix& CMatrix::operator -= (const CMatrix& m2){
    if((this->block->rows != m2.block->rows) && (this->block->cols != m2.block->cols))
		throw WrongDim();

    this->block = this->block->detach();

	for(unsigned int i = 0; i < this->block->rows; i++){
		for(unsigned int j = 0; j < this->block->cols; j++){
			this->block->data[i][j] -= m2.block->data[i][j];
		}
	}
    return *this;
}

bool CMatrix::operator == (const CMatrix& m2){
    if((this->block->rows != m2.block->rows) && (this->block->cols != m2.block->cols))
		return false;

    for(unsigned int i = 0; i < this->block->rows; i++){
		for(unsigned int j = 0; j < this->block->cols; j++){
			if(this->block->data[i][j] != m2.block->data[i][j])
                return false;
		}
	}
	return true;
}

std::ostream & operator << (std::ostream & s, const CMatrix& matrix){
	s << "[";
	for(unsigned int i=0;i<matrix.block->rows;i++){
		for(unsigned int j=0;j<matrix.block->cols;j++){
			s << matrix.block->data[i][j];
			if(((j+1) % matrix.block->cols) == 0 && j!=0 && i!=matrix.block->rows-1)
				s << "\n ";
			if(!(i==matrix.block->rows-1 && j==matrix.block->cols-1) && j!=matrix.block->cols-1)
				s << ", ";
		}
	}
	s << "]";
	return s;
}

double CMatrix::read(unsigned int row, unsigned int col) const{
    std::cout << "read" << std::endl;
	try{
        return block->data[row][col];
	}
	catch(...){
        throw IndexOutOfRange();
	}
}

void CMatrix::write(unsigned int row, unsigned col, double value){
    std::cout << "write" << std::endl;
	block = block->detach();
	try{
        block->data[row][col] = value;
	}
	catch(...){
        throw IndexOutOfRange();
	}
}

CMatrix::Cref CMatrix::operator()(unsigned int row, unsigned int col){
  check_row(row);
  check_col(col);
  return Cref(*this,row,col);
}

std::ostream& operator<<(std::ostream& o, const CMatrix::Cref& matrix){
	o << matrix.Cref_matrix.block->data[matrix.Cref_rows][matrix.Cref_cols];
	return o;
}

void CMatrix::check_row(unsigned int rowToCheck){
	if(block->rows < rowToCheck)
		throw IndexOutOfRange();
}

void CMatrix::check_col(unsigned int colToCheck){
	if(block->cols < colToCheck)
		throw IndexOutOfRange();
}
