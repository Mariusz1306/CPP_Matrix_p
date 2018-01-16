#include <math.h>
#include <fstream>
#include <iostream>

class OutOfMem{};
class IndexOutOfRange{};
class WrongDim{};

class CMatrix{
	private:
		struct rcmatrix;
		rcmatrix* block;
	public:
		class Cref;

		CMatrix();
		CMatrix(const CMatrix& cm);
		CMatrix(std::fstream& fs);
		CMatrix(unsigned int, unsigned int, double);
		void write(unsigned int i, unsigned int j, double d);
		double read(unsigned int i, unsigned int j) const;
		double operator()(unsigned int i, unsigned int j) const;
		CMatrix& operator=(const CMatrix& asOp);
		CMatrix& operator=(double** co);
		~CMatrix();
		friend std::ostream & operator << (std::ostream & s, const CMatrix & matrix);
		friend std::ostream & operator << (std::ostream & s, const CMatrix::Cref& s1);
		friend CMatrix operator* (const CMatrix&, const CMatrix&);
		friend CMatrix operator+ (const CMatrix&, const CMatrix&);
		friend CMatrix operator- (const CMatrix&, const CMatrix&);
		CMatrix& operator*= (const CMatrix&);
		CMatrix& operator+= (const CMatrix&);
		CMatrix& operator-= (const CMatrix&);
        bool operator== (const CMatrix&);

		void check(unsigned int i);
		Cref operator()(unsigned int i, unsigned int j);

};

struct CMatrix::rcmatrix{
    double** data;
    unsigned int rows;
    unsigned int cols;
    unsigned int ref_count;
//RCMATRIX void constructor
    rcmatrix(){
        this->data = NULL;
        this->cols = 0;
        this->rows = 0;
        this->ref_count = 0;
    }
//RCMATRIX file constructor
	rcmatrix(std::fstream& fs){
		fs >> this->rows;
		fs >> this->cols;
		try{
			this->data = new double*[this->rows];
			for(unsigned int i = 0; i < this->rows; i++)
			this->data[i] = new double[this->cols];
		}
		catch(...){
			throw OutOfMem();
		}
		for(unsigned int i = 0; i < this->rows; i++){
			for(unsigned int j = 0; j < this->cols; j++){
				fs >> this->data[i][j];
			}
		}
		this->ref_count=1;
	}
//RCMATRIX filled with value constructor
	rcmatrix(unsigned int new_rows, unsigned int new_cols, double value){
		try{
			this->data = new double*[new_rows];
			for(unsigned int i = 0; i < new_rows; i++)
				this->data[i] = new double[new_cols];
		}
		catch(...){
			throw OutOfMem();
		}

		for(unsigned int i = 0; i < new_rows; i++)
			for(unsigned int j = 0; j < new_cols; j++)
				this->data[i][j]=value;

		this->rows=new_rows;
		this->cols=new_cols;
		this->ref_count=1;
	}
//RCMATRIX copying constructor
	rcmatrix(unsigned int new_rows, unsigned int new_cols,double** old_matrix){
		this->rows=new_rows;
		this->cols=new_cols;
		this->ref_count=1;
		this->data = old_matrix;
		}

	rcmatrix(const rcmatrix& old_matrix){
	    //std::cout << "TEST" << std::endl;
		try{
			this->data = new double*[old_matrix.rows];
			for(unsigned int i=0;i<old_matrix.rows;i++)
				this->data[i]=new double[old_matrix.cols];
		}
		catch(...){
			throw OutOfMem();
		}

		for(unsigned int i=0;i<old_matrix.rows;i++)
			for(unsigned int j=0;j<old_matrix.cols;j++)
				this->data[i][j]=old_matrix.data[i][j];


		this->rows=old_matrix.rows;
		this->cols=old_matrix.cols;
		this->ref_count=1;

	}

    ~rcmatrix(){
    for(unsigned int i=0;i<this->rows;i++)
    	delete[] data[i];
    delete[] data;
    }

    rcmatrix* detach(){
        //std::cout<<"Detach"<<std::endl;
		if(ref_count==1)
			return this;
        rcmatrix* t=new rcmatrix(*this);
        ref_count--;
        return t;
    }

	void assign(double** p){
		data = p;
	}
};

class CMatrix::Cref{
	friend class CMatrix;
	CMatrix& s;
	unsigned int i;
	unsigned int j;

	Cref (CMatrix& ss, unsigned int ii, unsigned jj): s(ss), i(ii), j(jj){
		//std::cout << "cref contructor CALLED"<<std::endl;
	};

	public:
		operator double() const{
			//std::cout << "operator double CALLED"<<std::endl;
			return s.read(i, j);
		};

		CMatrix::Cref& operator = (double d){
			//std::cout << "operator = (double c) CALLED" << std::endl;
			s.write(i, j, d);
			return *this;
		};

		CMatrix::Cref& operator = (const Cref& ref){
			//std::cout << "operator = (const Cref& ref) CALLED" << std::endl;
			return operator = (ref);
		};

		friend std::ostream& operator<<(std::ostream&, const CMatrix::Cref&);
};
