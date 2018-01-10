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
cm.block->n++;
block = cm.block;
}

CMatrix::~CMatrix(){
  if(--block->n==0)
    delete block;
}

CMatrix::CMatrix(){
        block = new rcmatrix();
}

CMatrix::CMatrix(unsigned int nrows, unsigned int ncols, double var){
        block = new rcmatrix(nrows,ncols,var);
}

CMatrix::CMatrix(unsigned int nrows, unsigned int ncols, double var1, double var2){
                block = new rcmatrix(nrows,ncols,var1,var2);
}

CMatrix& CMatrix::operator = (const CMatrix& asOp){

asOp.block->n++;
if(--block->n == 0)
delete block;

block=asOp.block;
return *this;
}

CMatrix & CMatrix::operator=(double** co){
  if(block->n==1){
    block->assign(co);
  }
  else
  {
    rcmatrix* t= new rcmatrix(1,1,co);
    this->block->n--;
    this->block = t;
  }
  return *this;
}

std::ostream & operator << (std::ostream & s, const CMatrix & matrix){
	//std::cout << "matrix.cols=" << matrix.block->cols <<std::endl;
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

inline CMatrix operator * (const CMatrix& m1, const CMatrix& m2){

        if(m1.block->cols != m2.block->rows)throw WrongDim();

        CMatrix newMatrix(m1.block->rows, m2.block->cols,0.0);

        for(unsigned int i=0;i<newMatrix.block->rows;i++)
        for(unsigned int j=0;j<newMatrix.block->cols;j++)
        {
                double var=0.0;

                for(unsigned int lol=0;lol<m1.block->cols;lol++)
                var += m1.block->data[i][lol]*m2.block->data[lol][j];

                newMatrix.block->data[i][j] = var;
        }

        return newMatrix;
}

double* CMatrix::read(unsigned int i) const{
	try{
	return block->data[i];
	}
	catch(...){
	throw IndexOutOfRange();
	}
}

void CMatrix::write(unsigned int i, double* c){
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
std::cout << "operator<<(std::ostream& o, const CMatrix::Cref& s1)\\n"<<std::endl;
	o << s1.s.block->data[s1.i][s1.i];
	return o;
}
