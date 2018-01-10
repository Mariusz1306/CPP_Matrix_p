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
		CMatrix(unsigned int nrows, unsigned int ncols, double var);
		CMatrix(unsigned int nrows, unsigned int ncols, double var1, double var2);
		void write(unsigned int i, double* c);
		double* read(unsigned int i) const;
		double* operator[](unsigned int i )const;
		CMatrix& operator=(const CMatrix& asOp);
		CMatrix& operator=(double** co);
		~CMatrix();
		friend std::ostream & operator << (std::ostream & s, const CMatrix & matrix);
		friend std::ostream & operator << (std::ostream & s, const CMatrix::Cref& s1);
		friend CMatrix operator* (const CMatrix&, const CMatrix&);
		void check(unsigned int i);
		Cref operator[](unsigned int i);

};



struct CMatrix::rcmatrix{
    double** data;
    unsigned int rows;
    unsigned int cols;
    unsigned int n;

    rcmatrix(){
        this->data=NULL;
        this->cols=0;
        this->rows=0;
        this->n=0;
    }

	rcmatrix(std::fstream& fs){
		fs >> this->rows;
		fs >> this->cols;

		try{
			this->data = new double*[this->rows];
			for(unsigned int i=0;i<this->rows;i++)
			this->data[i]=new double[this->cols];
		}
		catch(...){
			throw OutOfMem();
		}

		for(unsigned int i=0;i<this->rows;i++){
			for(unsigned int j=0;j<this->cols;j++){
				fs >> this->data[i][j];
			}
		}
		this->n=1;
	}





	rcmatrix(unsigned int nrows, unsigned int ncols, double var){
		try{
			this->data = new double*[nrows];
			for(unsigned int i=0;i<nrows;i++)
				this->data[i]=new double[ncols];
		}
		catch(...){
			throw OutOfMem();
		}

		for(unsigned int i=0;i<nrows;i++)
			for(unsigned int j=0;j<ncols;j++)
				if(i==j)this->data[i][j]=var;
				else this->data[i][j]=0.0;

		this->rows=nrows;
		this->cols=ncols;
		this->n=1;
	}

	rcmatrix(unsigned int nrows, unsigned int ncols, double var1, double var2){
		try{
			this->data = new double*[nrows];
			for(unsigned int i=0;i<nrows;i++)
				this->data[i]=new double[ncols];
			}
			catch(...){
				throw OutOfMem();
			}

			for(unsigned int i=0;i<nrows;i++)
				for(unsigned int j=0;j<ncols;j++)
					if(i==j)this->data[i][j]=var1;
					else this->data[i][j]=var2;

			this->rows=nrows;
			this->cols=ncols;
			this->n=1;
			};

	rcmatrix(unsigned int nrows, unsigned int ncols,double** lol){
		this->rows=nrows;
		this->cols=ncols;
		this->n=1;
		this->data = lol;
		}

	rcmatrix(const rcmatrix& lol){
		try{
			this->data = new double*[lol.rows];
			for(unsigned int i=0;i<lol.rows;i++)
				this->data[i]=new double[lol.cols];
		}
		catch(...){
			throw OutOfMem();
		}

		for(unsigned int i=0;i<lol.rows;i++)
			for(unsigned int j=0;j<lol.cols;j++)
				this->data[i][j]=lol.data[i][j];


		this->rows=lol.rows;
		this->cols=lol.cols;
		this->n=1;

	}

    ~rcmatrix(){
    for(unsigned int i=0;i<this->rows;i++)
    	delete[] data[i];
    delete[] data;
    }

    rcmatrix* detach(){
		if(n==1)
			return this;
        rcmatrix* t=new rcmatrix(*this);
        n--;
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

	Cref (CMatrix& ss, unsigned int ii): s(ss), i(ii){
		std::cout << "cref contructor CALLED\\n"<<std::endl;
	};

	public:
		operator double*() const{
			std::cout << "operator double* CALLED\\n"<<std::endl;
			return s.read(i);
		};

		CMatrix::Cref& operator = (double* c){
			std::cout << "operator = (double* c) CALLED\\n" << std::endl;
			s.write(i,c);
			return *this;
		};

		CMatrix::Cref& operator = (const Cref& ref){
			std::cout << "operator = (const Cref& ref) CALLED\\n" << std::endl;
			return operator = ((double*)ref);
		};

		friend std::ostream& operator<<(std::ostream&, const CMatrix::Cref&);
};


