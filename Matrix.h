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
		CMatrix(const CMatrix&);
		CMatrix(std::fstream&);
		CMatrix(unsigned int, unsigned int, double);
		~CMatrix();
		CMatrix& operator=(const CMatrix&);
		friend CMatrix operator* (const CMatrix&, const CMatrix&);
		friend CMatrix operator+ (const CMatrix&, const CMatrix&);
		friend CMatrix operator- (const CMatrix&, const CMatrix&);
		CMatrix& operator*= (const CMatrix&);
		CMatrix& operator+= (const CMatrix&);
		CMatrix& operator-= (const CMatrix&);
        bool operator== (const CMatrix&);
		friend std::ostream & operator << (std::ostream&, const CMatrix&);
		friend std::ostream & operator << (std::ostream&, const CMatrix::Cref&);
        void write(unsigned int, unsigned int, double);
		double read(unsigned int i, unsigned int) const;
		double operator()(unsigned int i, unsigned int) const;
		void check_row(unsigned int);
		void check_col(unsigned int);
		Cref operator()(unsigned int, unsigned int);
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
		if(ref_count==1)
			return this;
        rcmatrix* t=new rcmatrix(*this);
        ref_count--;
        return t;
    }

};

class CMatrix::Cref{
	friend class CMatrix;
	CMatrix& Cref_matrix;
	unsigned int Cref_rows;
	unsigned int Cref_cols;

	Cref (CMatrix& mat, unsigned int i, unsigned j): Cref_matrix(mat), Cref_rows(i), Cref_cols(j){
	};

    public:
        operator double() const{
            return Cref_matrix.read(Cref_rows, Cref_cols);
        };

        CMatrix::Cref& operator = (double value){
            Cref_matrix.write(Cref_rows, Cref_cols, value);
            return *this;
        };

        CMatrix::Cref& operator = (const Cref& ref){
            return operator = (ref);
        };

        friend std::ostream& operator<<(std::ostream&, const CMatrix::Cref&);
};
