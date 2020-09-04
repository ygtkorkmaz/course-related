#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h> 
#include <stdlib.h> 

using namespace std;

class Matrix{
	public:
	int row;
	int column;
	double** data;
	Matrix();//default constructor
	Matrix(int,int,double**);//constructor 1
	Matrix(int,int);//constructor 2
	Matrix operator*=(double mult) { //multiplying matrix by a scalar number, operator is *=
		for (int i=0;i<row;i++) {
    	    for (int j=0;j<column;j++) {
     	       data[i][j] *= mult;
     	   }
    	}
		return *this;	
	}
	Matrix operator/=(double div) { //dividing matrix by a scalar number, operator is /=
		for (int i=0;i<row;i++) {
    		for (int j=0;j<column;j++) {
     		    data[i][j] /= div;
     		}
    	}
		return *this;	
	}
	void print() {//printing data of matrix
		for(int i=0;i<row;i++) {
			for(int j=0;j<column;j++) {
				cout<<data[i][j]<<" ";
			}
			cout<<"\n";
		}	
		};
 	~Matrix () {//destructor
	 }
};

Matrix::Matrix(int r,int c,double** b){//constructor 1, when we have a multidimensional array.
	row=r;
	column=c;
	data = new double*[row];
	for(int t = 0; t < row; ++t)
		data[t] = new double [column];//creates a dynamic memory in order to store given multidimensional array
	data=b; //stores given multidimensional array
	}
Matrix::Matrix(int r,int c){//constructor 2, when we only have number of rows and columns
	row=r;
	column=c;
	data = new double*[row];
	for(int i = 0; i < row; ++i) //only creates dynamic memory with given number of rows and columns
		data[i] = new double [column];
}

Matrix addsub( Matrix a,  Matrix b, int c) { //Function for both adding and substracting matricies, decides with integer c, when c=1 adding, c=0 substracting
		Matrix res(a.row,a.column);
		if(c==1){		
    	for (int i=0;i<a.row;i++) {
        	for (int j=0;j<a.column;j++) {
            	res.data[i][j] = (a.data[i][j] + b.data[i][j]); //adding component by component
        	}	
    	}
    	}
    	else if(c==0){
    	for (int i=0;i<a.row;i++) {
        	for (int j=0;j<a.column;j++) {
            	res.data[i][j] = (a.data[i][j] - b.data[i][j]); //substracting component by component
        	}	
    	}	
		}
		return res;
	}
	
Matrix mult(Matrix a, Matrix b){//multiplication of matricies
	Matrix res(a.row,b.column); //to store multiplication properly, matrix with a-row x b-column is created.
	for(int j=0;j<res.row;j++) { //fills matrix with zeros in order to make consecutive summings more securely.
				for(int k=0;k<res.column;k++) {
					res.data[j][k]=0;
			}
		}
	 for(int i = 0; i < a.row; ++i){
        for(int j = 0; j < b.column; ++j){
            for(int k = 0; k < a.column; ++k)
            {
                res.data[i][j] += a.data[i][k] * b.data[k][j];
            }}}
	return res;
}

double norminf(Matrix b){//infinity norm finder
	double norm=0;
	for(int i=0; i<b.row; i++){
		if(fabs(b.data[i][0])>=norm){//since we only need this for vectors, column number is taken as 1;
			norm=fabs(b.data[i][0]); //finds the biggest element 
		}
	}
	return norm;
}

		
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char* argv[]) {
	int error=0;//error checker in order to detect errors.
	int l=0;
	if(argc<4){ // when insufficient args are given, makes error checker = 1, quits program.
		error=1;
		cout<<"Error detected.";
		return 0;
	}
	string line;
	ifstream xfile (argv[1]);
	if(xfile.is_open()) {
		while(!xfile.eof())
		{
			getline (xfile, line);
			l=l+1; //getting the number of rows and columns
		}
	}
	else{ 
		error=1;
		cout<<"Error detected.";
		return 0;} // if file is not opened successfully, makes error checker=1, quits program.
	xfile.clear();
	xfile.seekg (0, ios::beg);
	//Creating dynamic arrays for storing value of given matrix
	double** A1 = new double*[l];
	for(int t = 0; t < l; ++t)
		A1[t] = new double [l];	

	for(int i=0; i<l;i++){
		for(int j=0;j<l;j++){
		xfile >> A1[i][j];
		}
	}
	xfile.close();
	Matrix x(l,1); //creates the starting vector in order to apply power iteration
	for(int i=0;i<x.row;i++) {
		x.data[i][0]=1; //makes all of its elements = 1
	}
	double norm=2; //for algorithm to work properly, a starting value of 2 is selected.
	double pastnorm=1; //for algorithm to work properly, a starting value of 1 is selected.
	double dif=norm-pastnorm;
	Matrix A(l,l,A1); //passes given matrix to an object.
	while(norm!=pastnorm){
		if(dif< 0) {
    	dif *= (-1); //keeps difference always positive
		}
	if(dif<atof(argv[2])) { //when difference between old and present norm is smaller than tolerance, breaks the loop.
		break;
		}
	Matrix temp(l,1); //creates a temporary object to apply iteration properly.
	temp=mult(A,x);
	pastnorm=norm;
	norm=norminf(temp);
	x=temp;
	x/=norm; //normalizes the vector.
	dif=norm-pastnorm;
	if(norm==0){// if norm=0 makes error check=1, quits program.
		error=1;
		cout<<"Error detected.";
		return 0;
		break;}
	}
	//
	Matrix dum(l,1); //creates dummy object to check if eigenvalue is negative or not.
	for(int i=0;i<dum.row;i++) {
		dum.data[i][0]=1; //fills it with ones.
	}
	for(int k=0; k<l;k++){
		if(mult(A,dum).data[k][0]/(dum*=norm).data[k][0]<0){ // if first nonzero terms of A*dum and eigenvalue*dum don't have same sign, takes negative of eigenvalue found.
		norm=-norm;
		}
	}
	//
	Matrix t1(1,l);//creates objects for deflation
	Matrix B(l,l);
	for(int i=0;i<l;i++){
		if(x.data[i][0]!=0){ //finds the row of first non-zero term of eigenvector 
			for(int j=0;j<l;j++){
				t1.data[0][j]=A.data[i][j]; //stores that row of given matrix in a vector.
			}
		B=addsub(A,(mult(x,t1))/=x.data[i][0],0); //applies deflation, ie. B=A-(x*t1)/(first non zero term of eigenvector)
		break;
		}
	}
	Matrix x2(l,1); //creates an object to apply power iteration to matrix B.
	for(int i=0;i<x.row;i++) {
		x2.data[i][0]=1; //fills it with ones
	}
	double norm2=1; //for algorithm to work properly starting value of 1 is selected.
	double pastnorm2=0; //for algorithm to work properly starting value of 0 is selected.
	double dif2=norm2-pastnorm2;
	while(norm2!=pastnorm2){
		if(dif2< 0) {
    	dif2 *= (-1); //keeps difference positive
		}
	if(dif2<atof(argv[2])) { //if difference is less than tolerance, stops the loop.
		break;
		}
	Matrix temp2(l,1); //creates temporary object to apply iteration algorithm.
	temp2=mult(B,x2);
	pastnorm2=norm2;
	norm2=norminf(temp2);
	x2=temp2;
	x2/=norm2; //normalizes the vector.
	dif2=norm2-pastnorm2;
		if(norm2==0){
		error=1;
		cout<<"Error detected.";
		return 0;
		break;}
	}
	//
	Matrix dum2(l,1); //creates a dummy object to check if eigenvalue is negative or not.
	for(int i=0;i<dum2.row;i++) {
		dum2.data[i][0]=1; //fills it with ones
	}
	for(int k=0; k<l;k++){
		if(mult(A,dum2).data[k][0]/(dum2*=norm2).data[k][0]<0){ //if first non zero terms of A*dum and eigenvalue2*dum don't have same signs, takes negative of eigenvalue found.
		norm2=-norm2;
		}
	}
	//
	if(error==0){//if error checker= 0, prints out 2 eigenvalues and eigenvector.
	cout<<"Eigenvalue#1: "<<norm<<endl;
	x.print();
	cout<<"Eigenvalue#2: "<<norm2<<endl;
	ofstream myfile; //writes outputs to a file whose name is given as argument.
  		myfile.open (argv[3]);
  		myfile<<"Eigenvalue#1: "<<norm<<endl;
  		for(int i=0; i<l;i++){
		myfile << x.data[i][0];
		myfile << "\n";
		}
		myfile<<"Eigenvalue#2: "<<norm2<<endl;
  		myfile.close();
		delete[] A1;//deleting dynamic array to free memory.
	return 0;
	}
	else{
		cout<<"Error detected.";
			return 0;
	}

}
