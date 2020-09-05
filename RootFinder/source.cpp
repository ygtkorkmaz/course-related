#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <stdio.h> 
#include <stdlib.h>
 
using namespace std;

class poly{
	public:
		int deg;
		double* coef;
		poly();//default constructor
		poly(int,double*);//constructor 1
		poly(int);//constructor 2
		~poly () { //destructor
		}
	double eval(double point) { //evaluating the polynomial for a point
		double sum=0;
		for(int i=deg;i>=0;i--) {
			sum += (pow(point,i)*(coef[i])); //summing of terms of all degrees
		}
		return sum;
	}
	void bisection(double x1,double x2,double tol){//Applying bisection algorithm
		int iterationnum=0;
		double x0;
		double g1=x1;//assigning given guesses to variables in order to keep them unchanged.
		double g2=x2;
			while (fabs(g2-g1) > tol)
    	{
        x0 = g1+(g2-g1)/2; // Finding middle point
        if (eval(x0) == 0)// Checking if middle point is root
            break;
        else if (eval(x0)*eval(g1) < 0)// Decide the side to repeat the steps
            g2 = x0;
        else
            g1 = x0;
    	iterationnum++;//updating the number of iteration.
	}
    cout << "Bisecetion Method - The value of root is : " <<x0<< endl;
    cout << "Bisecetion Method - # of iterations : "<<iterationnum<<endl;
		}
	void secant(double x1, double x2, double tol)//applying secant method
	{
		double g1;
		double g2=x1;
    	double iterationnum = 0;
		double x0=x2;
        do {
        	g1 = g2;//Assigning given guesses to variables in order to keep them unchanged.
            g2 = x0;
            x0=g2-eval(g2)*(g2-g1)/(eval(g2)-eval(g1));// calculating the intermediate value
            iterationnum++;// updating number of iteration
            if (eval(x0) == 0)// if x0 is the root of equation then break the loop
                break;
        } while (fabs(g2 - x0) > tol); // repeat the loop until the convergence
 
        cout << "Secant Method - The value of root is : " << x0 << endl;
        cout << "Secant Method - # of iterations = " << iterationnum << endl;
	}
	void hybrid(double x1, double x2, double tol){//applying hybrid method
		double x0;
		double g1=x1;//assigning given guesses to variables in order to keep them unchanged
		double g2=x2;
		double temp;
		double iterationnum=0;
		for(int i=0; i<2;i++){//applying bisection method for first 2 iterations
			 x0 = g1+(g2-g1)/2;
            if (eval(x0)==0||g2-g1<=tol){//breaking loop if it converges or middle point is a root of equation.
            	cout << "Hybrid Method - The value of root is : " << x0 << endl;
        		cout << "Hybrid Method - # of iterations = " << iterationnum << endl;
        		break;
			}
        // Decide the side to repeat the steps
        	else if (eval(x0)*eval(g1) < 0)
            	g2 = x0;
        	else
            	g1 = x0;
    	iterationnum++;
		}
		while (1){
			x0=g2-eval(g2)*(g2-g1)/(eval(g2)-eval(g1));//finding the intermadiate value
			iterationnum++;
			 if (eval(x0) == 0||fabs(g2-x0)<=tol){//breaking the loop if it converges or x0 is a root of equation.
			 cout << "Hybrid Method - The value of root is : " << x0 << endl;
        	 cout << "Hybrid Method - # of iterations = " << iterationnum << endl;
                break;
    		}
			temp=g2;//using temp variable to update values.
			g2=x0;
			g1=temp;
		}
	}};

poly::poly(int d, double *c){//construct decleration, if we have degree and coefficients both.
	deg=d;
	coef=new double[deg+1];//creating a dynamic array to store coefficients, note that we also need to consider 0th degree.
	coef=c;
}
poly::poly(int d){//construct decleration, if we only have degree.
	deg=d;
	coef=new double[deg+1];
}

int main(int argc, char** argv) {
	if(argc <5) { //error check mechanism for insufficient command line arguments
			cout<<"Error-Insuffient argument!"<<endl;
			return 0;
		}
	if (argc == 5) { //error check mechanism for 0th degree constant polynomials
			cout<<"Error-Polynomial is 0th degree, i.e. have no roots!"<<endl;
			return 0;
		}
	poly x(argc-5);//creating a polynomial with proper degree, -5 is beceause of given guesses, tolerance, and effect of 0th degree coefficient.
	for(int i=0;i<=(argc-5);i++) {
				x.coef[(argc-5)-i]=atof(argv[i+1]);//assigning coefficients to respective degrees.
	}
	double tol=atof(argv[argc-1]);//converting and assigning.
	double g1=atof(argv[argc-3]);//converting and assigning.
	double g2=atof(argv[argc-2]);//converting and assigning.
	x.bisection(g1,g2,tol);
	x.secant(g1,g2,tol);
	x.hybrid(g1,g2,tol);
	
	return 0;
}
