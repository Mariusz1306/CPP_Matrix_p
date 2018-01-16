#include <math.h>
#include <fstream>
#include "Matrix.h"

void CMatrix::check(unsigned int i){
	if(block->rows<i)
		throw IndexOutOfRange();
}

CMatrix::CMatrix(std::fstream& fs){
	block = new rcmatrix(fs);
}

CMatrix::CMatrix(const CMatrix& cm){
	cm.block->ref_count++;
	block = cm.block;
}

CMatrix::~CMatrix(){
	if(--block->ref_count==0)
		delete block;
}

CMatrix::CMatrix(){
	block = new rcmatrix();
}

CMatrix::CMatrix(unsigned int nrows, unsigned int ncols, double var){
    block = new rcmatrix(nrows,ncols,var);
}

CMatrix& CMatrix::operator = (const CMatrix& asOp){
	asOp.block->ref_count++;
	if(--block->ref_count == 0)
		delete block;

	block=asOp.block;
	return *this;
}

CMatrix & CMatrix::operator=(double** co){
	if(block->ref_count==1){
	block->assign(co);
	}
	else{
		rcmatrix* t= new rcmatrix(1,1,co);
	this->block->ref_count--;
	this->block = t;
	}
	return *this;
}

std::ostream & operator << (std::ostream & s, const CMatrix & matrix){
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

CMatrix operator * (const CMatrix& m1, const CMatrix& m2){
	if(m1.block->cols != m2.block->rows)
		throw WrongDim();

	CMatrix newMatrix(m1.block->rows, m2.block->cols,0.0);

	for(unsigned int i=0;i<newMatrix.block->rows;i++)
		for(unsigned int j=0;j<newMatrix.block->cols;j++){
			double var=0.0;
			for(unsigned int lol=0;lol<m1.block->cols;lol++)
				var += m1.block->data[i][lol]*m2.block->data[lol][j];
			newMatrix.block->data[i][j] = var;
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

double* CMatrix::read(unsigned int i) const{
    std::cout << "read" << std::endl;
    std::cout << "i: " << i << std::endl;
	try{
        return block->data[i];
	}
	catch(...){
        throw IndexOutOfRange();
	}
}

void CMatrix::write(unsigned int i, double* c){
    std::cout << "write" << std::endl;
	block = block->detach();
	try{
        block->data[i] = c;
	}
	catch(...){
        throw IndexOutOfRange();
	}
}



CMatrix::Cref CMatrix::operator[](unsigned int i){
  std::cout << "Cref rcstring::operator[](unsigned int i) CALLED"<<std::endl;
  check(i);
  return Cref(*this,i);
}

std::ostream& operator<<(std::ostream& o, const CMatrix::Cref& s1){
std::cout << "operator<<(std::ostream& o, const CMatrix::Cref& s1)"<<std::endl;
	o << s1.s.block->data[s1.i][s1.i];
	return o;
}
