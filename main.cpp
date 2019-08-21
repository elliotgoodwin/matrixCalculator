// matrixCalculator
// A definition and example of a class that performs addition, subtraction, multiplication and finding the determinant of matrices of arbitrary size
// OOP Assignment 6
// Elliot Goodwin
// 04/03/19

// includes and headers
#include<iostream>
#include<stdlib.h> // for c style exit
#include<iomanip>  // for setw()
#include<sstream>  // for string stream
#include<cmath>    // for pow()
// declare namespace
using namespace std;


//--------------------- Class variable definitions ------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//          *mdata - pointer to matrix elements
//            rows - number of rows in matrix
//         columns - number of columns in matrix
//    matrix(i, j) - matrix element at ith row and jth column


// define a matrix class
class matrix
{
    // Friends
    friend ostream & operator<<(ostream &os, const matrix &mat);
    friend istream & operator>>(istream &is, matrix &mat);
    
private:
    double *mdata;
    int rows, columns;
    double compute_Det() const; // compute a 2x2 det
    
public:
    
    matrix(){ mdata = 0; rows = columns = 0; }   // Default constructor
    matrix(const int n, const int m);	         // Parameterized constructor
    matrix(matrix&);                             // Copy constructor
    matrix(matrix&&);                            // Move constructor
    ~matrix() {                                  // Destructor
        delete[] mdata; mdata = 0; rows = columns = 0;
        //cout << "Destructor called." << endl;
    };
    
    // Access functions
    int getRows() const { return rows; }    // Return number of rows
    int getCols() const { return columns; } // Return number of columns
    int index(int m, int n) const;          // Return position in array of element (m,n)
    double & operator()(int m, int n) const { return mdata[index(m, n)]; }  // Return element (m, n)
    void setElement(int m, int n, double element) const; // Set element (m, n)
    bool isEmpty();
    
    matrix& operator= (matrix&);     // Copy Assignment operator
    matrix& operator= (matrix&&);    // Move Assignment operator
    
    // Overload addition, subtraction and multiplication operators for matrices
    matrix operator+ (const matrix &m) const;    // addition
    matrix operator- (const matrix &m) const;    // subtraction
    matrix operator* (const matrix &m) const;    // multiplication
    
    matrix getMinor(const int n, const int m) const;   // minor
    double getDet() const;                             // determinant
};



//--------------------- Member functions defined outside class ------------------------------------------
//-------------------------------------------------------------------------------------------------------
// Parameterised contructor (builds nxm matrix with all elements set to 0)
matrix::matrix(const int m, const int n)
{
    mdata = new double[n*m]; rows = m; columns = n;
    for (int i = 0; i < n*m; i++) { mdata[i] = 0; }
}

// Copy constructor for deep copying
matrix::matrix(matrix &mat)
{
    // Construct matrix of same size as that being copied
    mdata = 0;
    rows = mat.rows;
    columns = mat.columns;
    if (rows > 0 && columns > 0)
    {
        mdata = new double[rows*columns];
        // Copy values into new matrix
        for (int j = 1; j <= rows; j++) {
            for (int i = 1; i <= columns; i++){
                mdata[index(i, j)] = mat(i, j);
            }
        }
    }
    else { cout << "Error. Matrices must have same dimensions." << endl; }
}

// Move copy constructor
matrix::matrix(matrix &&mat)
{   // steal the data
    rows = mat.rows;
    columns = mat.columns;
    mdata = mat.mdata;
    mat.rows = 0; mat.columns = 0; // delete[] mat.mdata; for older versions of C++
    mat.mdata = nullptr;
}

// Assignment operator for deep copying
matrix & matrix::operator=(matrix &mat)
{
    if (&mat == this) return *this;   // if copying to self, do nothing
    // Delete this object's array
    delete[] mdata; mdata = 0; rows = columns = 0;
    // Copy dimensions of matrix to be copied
    rows = mat.rows;
    columns = mat.columns;
    if (rows > 0 && columns > 0)
    {
        // Declare new matrix with same dimensions
        mdata = new double[rows*columns];
        // Copy values into new matrix
        for (int i = 1; i <= mat.rows; i++) {
            for (int j = 1; j <= mat.columns; j++){
                // set element (i, j) to (i, j) of matrix to be copied;
                setElement(i, j, mat.mdata[index(i, j)]);
            }
        }
    }
    return *this;
}

// Move assignment operator
matrix & matrix::operator=(matrix&& mat)
{
    // swap contents of one matrix with another
    swap(rows, mat.rows);
    swap(columns, mat.columns);
    swap(mdata, mat.mdata);
    return *this;
}

// returns position in array of element (m, n)
int matrix::index(int m, int n) const
{
    // print out element of mdata corresponding to matrix(i, j)
    if (m > 0 && m <= rows && n > 0 && n <= columns) { return (n - 1) + (m - 1)*columns; }
    // return error message if indices run over size of matrix
    else { cout << "Error: out of range." << endl; exit(1); }
}

// set value of element (m, n) in array
void matrix::setElement(int m, int n, double element) const
{
    if (m > rows || n > columns) { cout << "Error: out of range." << endl; }
    else { mdata[index(m, n)] = element; }
}

// return true if matrix is empty
bool matrix::isEmpty()
{
    if (rows == 0 && columns == 0) { return true; }
    else { return false; }
}

// overload addition operator for matrix addition
matrix matrix::operator+ (const matrix &mat) const
{
    // construct matrix of same dimensions
    matrix m(rows, columns);
    // sum (i, j) element of each matrix
    if (rows == mat.rows && columns == mat.columns)
    {
        for (int j = 1; j <= columns; j++) {
            for (int i = 1; i <= columns; i++) {
                m.setElement(i, j, mdata[index(i, j)] + mat(i, j));
            }
        }
    }
    // error message if matrices have different dimensions
    else { cout << "Error. Matrices do not have same dimensions." << endl; }
    return m;
}

// Overload subtraction operator for matrix subtraction
matrix matrix::operator- (const matrix &mat) const
{
    // construct matrix of same dimensions
    matrix m(rows, columns);
    // subtract (i, j) elements of each matrix
    if (rows == mat.rows && columns == mat.columns)
    {
        for (int j = 1; j <= columns; j++) {
            for (int i = 1; i <= columns; i++) {
                m.setElement(i, j, mdata[index(i, j)] - mat(i, j));
            }
        }
    }
    // error message if matrices have different dimensions
    else { cout << "Error. Matrices do not have same dimensions." << endl; }
    return m;
}

// overload multiplication operator for matrix multiplication
matrix matrix::operator* (const matrix &mat) const
{
    // construct matrix of same dimensions
    matrix m(rows, mat.getCols());
    
    // perform matrix multiplication operation
    if (columns == mat.getRows())
    {
        for (int i = 1; i <= mat.getRows(); i++) {
            for (int j = 1; j <= mat.getCols(); j++){
                double sum{ 0 };
                for (int k = 1; k <= columns; k++) {
                    sum += mdata[index(i, k)] * mat(k, j);
                }
                m.setElement(i, j, sum);
            }
        }
    }
    // error message if matrices are of incorrect dimensions
    else { cout << "Error. Can only multiply matrices with dimensions ixk * kxj." << endl; }
    return m;
}

// get matrix of minors
matrix matrix::getMinor(const int m, const int n) const
{
    // construct a matrix with n-1 length in each dimension
    matrix mat(rows - 1, columns - 1);
    
    // extract relevant elements for minor matrix
    for (int j = 1; j < n; j++) {
        for (int i = 1; i < m; i++) {
            mat.setElement(i, j, mdata[index(i, j)]);
        }
    }
    for (int j = n + 1; j <= rows; j++) {
        for (int i = 1; i < m; i++) {
            mat.setElement(i, j - 1, mdata[index(i, j)]);
        }
    }
    for (int j = 1; j < n; j++) {
        for (int i = m + 1; i <= columns; i++) {
            mat.setElement(i - 1, j, mdata[index(i, j)]);
        }
    }
    for (int j = n + 1; j <= rows; j++) {
        for (int i = m + 1; i <= columns; i++) {
            mat.setElement(i - 1, j - 1, mdata[index(i, j)]);
        }
    }
    return mat;
}

// private member function to calculate determinant of a 2x2 matrix
double matrix::compute_Det() const
{
    double det;
    // exit program if matrix is not 2x2
    if (rows != 2 && columns != 2) { exit(2); }
    else {
        // find determinant of 2x2 matrix
        det = mdata[index(1, 1)]*mdata[index(2, 2)];
        det -= mdata[index(1, 2)]*mdata[index(2, 1)];
    }
    return det;
}

// public member function to calculate determinant of an nxm matrix
double matrix::getDet() const
{
    // initialise det as zero for each loop in recursive calling of getDet()
    double det{0};
    // construct matrix of length n-1 in each dimension
    matrix temp(rows-1, columns-1);
    
    // exit program if matrix isn't square
    if (rows != columns) { cout << "Error. Matrix must be square." << endl; exit(3); }
    
    // if matrix is 2x2 then use private member function to compute determinant
    if (rows == 2 && columns == 2) {
        det = compute_Det();
        return det;
    }
    // if matrix has dimension nxn (n>2) then perform laplace expansion
    else {
        // loop over number of columns
        for (int j = 1; j <= columns; j++){
            // find minor matrix for each element in row
            temp = getMinor(1, j);
            // compute laplace expansion
            det = det + pow(-1, j-1) * temp.getDet() * mdata[index(1, j)];
        }
    }
    return det;
}

// Overload insertion operator for matrices
ostream & operator<< (ostream &os, const matrix &mat)
{
    // loop over all matrix elements
    for (int i = 1; i <= mat.rows; i++) {
        for (int j = 1; j <= mat.columns; j++){
            // print each element with formatting
            if (j == 1 && i == 1 ) { os << mat(i, j) << setw(6); }
            else if (j == 1 && i!=j ) { os << setw(15) << mat(i, j) << setw(6); }
            else { os << mat(i, j) << "     "; }
        }
        os << endl;
    }
    return os;
}

// Overload extraction operator for matrices
istream & operator>> (istream &is, matrix &mat){

    // declare variables
    string line;
    istringstream ss;
    int row_index{0};
    int column_index{0};
    
    // loop over number of rows in matrix
    while(row_index < mat.rows) {
        // iterate row index
        row_index++;
        // put contents of input stream in a string
        getline(is, line);
        // put contents of string in string stream
        ss.str(line);
    
        // extract each element inside ss
        while(!ss.eof()){
            // iterate column index
            column_index++;
            // ignore whitespace before and after the matrix element
            ss >> ws;
            // put elements from ss in to elements of mdata
            ss >> mat.mdata[mat.index(row_index, column_index)];
            ss >> ws;
        }
    
        // reset column_index, ss and line for next iteration
        column_index = 0;
        ss.clear();
        line.clear();
    }
    return is;
}

// bad_istream
// a function that checks for bad istream
bool bad_istream(istream &is){
    if(is.fail()) {
        is.clear();
        is.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error. Bad input.\n" << endl;
        return true;
    }
    else { return false; }
}

//--------------------- Main program --------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
int main() {
    
    // First part of assignment: constructing and deep copying matrices
    
    // Demonstrate default constructor
    cout << "Constructing matrix a1 with default constructor." << endl;
    matrix a1;
    cout << "Print a1:" << endl;
    
    if (a1.isEmpty()) {
        cout << "Error. Matrix is empty." << endl;
    }
    else {
        cout << "a1 =" << setw(11) << a1 << endl;
    }
    cout << endl;
    
    // Parameterized constructor
    cout << "Constructing matrix a2 with parameterised constructor." << endl;
    const int m(2), n(2);
    cout << "a2 has dimensions (" << n << "x" << m << ")." << endl;
    cout << endl;
    matrix a2(m, n);
    
    // Set values for a2
    cout << "Setting elements of a2." << endl;
    a2.setElement(1, 1, 1);
    a2.setElement(1, 2, 2);
    a2.setElement(2, 1, 3);
    a2.setElement(2, 2, 4);
    
    // Print matrix a2
    if (a2.isEmpty()) {
        cout << "Error. Matrix is empty.\n" << endl;
    }
    else {
        cout << "a2 =" << setw(11) << a2 << endl;
    }
    
    // Deep copy by assignment
    cout << "Initialise empty matrix a3." << endl;
    matrix a3(m,n); // define new matrix a3
    cout << "a3 =" << setw(11) << a3 << endl;
    
    cout << "Deep copy matrix a2 to matrix a3 by assignment." << endl;
    a3 = a2;        // copy a2 data to a3
    cout << "a3 =" << setw(11) << a3 << endl;
    
    // Modify contents of original matrix
    cout << "Modify contents of a2 and compare to a3." << endl;
    a2.setElement(1, 1, 9);
    a2.setElement(1, 2, 9);
    a2.setElement(2, 1, 9);
    a2.setElement(2, 2, 9);
    
    // Print matrices to show that elements are unchanged
    cout << "a2 =" << setw(11) << a2 << endl;
    cout << "a3 =" << setw(11) << a3 << endl;
    
    // Reset a2
    cout << "Reset a2:" << endl;
    a2.setElement(1, 1, 1);
    a2.setElement(1, 2, 2);
    a2.setElement(2, 1, 3);
    a2.setElement(2, 2, 4);
    cout << "a2 =" << setw(11) << a2 << endl;
    
    // Deep copy using copy constructor
    cout << "Deep copy matrix a2 to matrix a4 using copy constructor." << endl;
    matrix a4(a2);
    cout << "a4 =" << setw(11) << a4 << endl;
    
    // Modify contents of original matrix and show copied matrix is unchanged
    cout << "Modify contents of a2 and compare to a4:" << endl;
    a2.setElement(1, 1, 9);
    a2.setElement(1, 2, 9);
    a2.setElement(2, 1, 9);
    a2.setElement(2, 2, 9);
    cout << "a2 =" << setw(11) << a2 << endl;
    cout << "a4 =" << setw(11) << a4 << endl;
    
    // Reset a2
    cout << "Reset a2:" << endl;
    a2.setElement(1, 1, 1);
    a2.setElement(1, 2, 2);
    a2.setElement(2, 1, 3);
    a2.setElement(2, 2, 4);
    cout << "a2 =" << setw(11) << a2 << endl;
    
    
    // Move copy construction demonstration
    cout << "Example of using move copy constructor." << endl;
    matrix a5(move(a2));
    cout << "Show that contents of a2 have been moved to a5:" << endl;
    if (a2.isEmpty()) {
        cout << "a2 is empty" << endl;
    }
    else {
        cout << "a2 =" << setw(11) << a2 << endl;
    }
    cout << "a5 =" << setw(11) << a5 << endl;
    
    // Move assignment demonstration
    matrix a6 = move(a3);
    if(a3.isEmpty()) {
        cout << "a3 is empty." << endl;
    }
    else { cout << "a3 =" << setw(11) << a3 << endl; }
    cout << "a6 =" << setw(11) << a6 << endl;

    
    // Second part of assignment: matrix operations
    // Addition of 2 matrices
    // Example of adding matrices with different dimensions
    cout << "Examples of failed matrix addition." << endl;
    cout << "a1 + a6 = " << setw(5) << a1+a6 << endl;
    
    // Example of adding matrices with equal dimensions
    cout << "Example of successful matrix addition." << endl;
    cout << "a5 + a6 = " << setw(5) << a5+a6 << endl;
    
    // Subtraction of 2 matrices
    cout << "Example of matrix subtraction." << endl;
    cout << "a5 - a6 = " << setw(5) << a5-a6 << endl;
    
    // Multiplication of 2 matrices
    cout << "Example of successful matrix multiplication." << endl;
    cout << "a5*a6 = " << setw(7) << a5*a6 << endl;
    
    // Example of multiplication error
    cout << "Example of failed matrix multiplication." << endl;
    cout << a1 * a5 << endl;
    
    // Example of inputting a matrix using overloaded >> operator
    int row_index{4};
    int column_index{4};
    
    matrix M(row_index, column_index);
    cout << "Enter " << row_index << "x" << column_index << " matrix:" << endl;
    cin >> M;
    cout << "\nM =" << setw(12) << M << endl;
    cout << endl;

    // Example of finding minor
    cout << "Example of finding minor:" << endl;
    cout << "Minor(3, 2) = " << M.getMinor(3, 2) << endl;
    
    // Example of finding determinant
    cout << "Example of finding determinant:" << endl;
    double det = M.getDet();
    cout << "|M| = " << det << endl;
    
    cout << M.index(4, 1) << endl;

    
    return 0;
}
