# EigenvectorFinder

In order to execute this project, you should have a txt document(whose name is given as argument to program), which has an nxn matrix in the form of;

3.14 1.59 2.65 3.58
9.79 3.23 8.46 2.64
3.38 3.27 9.50 2.88
4.19 7.16 9.39 9.37

In the same folder with source.cpp file!

After this ensured, you can give arguments to program, and compile and run it. You should be able to see the results(or it can be an error message depending on arguments and matrix) If there aren't any errors, you can see first 2 dominant eigenvalues and eigenvector belonging to most dominant one both as output, and in the output file whose name is also given as argument to program. For example for given matrix above you should be able to get output like below:

Eigenvalue#1: 20.488

0.348061

0.622149

0.554319

1

Eigenvalue#2: 3.91405

## Important Notes:

*When filling in txt file, please only enter matrix. Do not add another line after matrix. That will cause program to crash.

*Note that, if you come across with an error, error message will not be written into txt file, it will only be printed out.

*Please consider limitations of power iteration method when giving inputs, which are:

--Starting vector may have no component in dominant eigenvector.

--There may be more than one eigenvalue having same maximum modulus.

--For real matrix and starting vector, iteration can never converge to complex vector.

Those things can cause iteration to fail.

- Note that, this program only finds real eigenvalues.  
- Dominant eigenvalues is always the one with maximum absolute value.     
- This program also finds whether the eigenvalues are positive or negative, by 
checking it with A*v=lambda*v.
- If you enter less than 3 arguments you will get an error message, however if you enter 3 arguments you should enter them in proper configuration,ie.

argv[1]=inputfile's name(including .txt)

argv[2]=tolerance value (you should enter a number!)

argv[3]=outputfile's name(including .txt)

