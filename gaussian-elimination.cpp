// GaussianElimination
// reads a comma separated .txt file of arbitrary numbers of rows and columns
// into a square matrix and uses Gaussian Elimination to compute the inverse

// includes
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// function definitions

// indentity_matrix
// make an nxn identity matrix
vector<vector<double> > identity(int n)
{
    vector<vector<double> > identity( n, vector<double>(n, 0) );
    for( int i = 0; i < n; ++i ) {
        identity[i][i] = 1;
    }
    return identity;
}

// main programme starts here
int main() {
    
    // declare variables
    vector < vector<double> > matrix, inverse;
    string s, line;
    
    // read in data to the matrix
    ifstream thefile("matrix.txt");
    
    // check the file is open
    if (thefile.is_open()) {
        
        // and that the end of the file hasn't been reached
        while (!thefile.eof()) {
            
            // read in each line of data and append it to the string 'line'
            getline(thefile, line);
            
            // make a temporary vector
            vector <double> temp;
            // use sstream to split the data in the line up, according to ',' separation
            istringstream linestream(line);
            while (getline(linestream, s, ',')) {
                // put the data in to temporary vector
                temp.push_back(atof(s.c_str()));
            }
            // put the data from the temporary vector in to the vector 'matrix'
            matrix.push_back(temp);
            
        }
        
        // check that 'matrix' is square
        if (matrix.size() != matrix[0].size()) {
            cout << "Error. Matrix is not square." << endl;
            exit(2);
        }
        
        // close the file
        thefile.close();
        
        // make identity matrix, with same dimensions as read in matrix
        // this will be transformed in to inverse matrix
        inverse = identity(matrix.size());
        
        // check identity matrix has been made
        for (int j = 0; j < inverse.size(); j++) {
            for (int i = 0; i < inverse.size(); i++) {
                if (i == j) {
                    if (inverse[j][i] == 1) {
                    }
                    else {
                        cout << "Error.\n";
                        exit(3);
                    }
                }
                else if (i != j) {
                    if (inverse[j][i] == 0) {
                    }
                    else {
                        cout << "Error.\n";
                        exit(3);
                    }
                }
            }
        }
        
        // matrices read in sucessfully
        
        // perform Gaussian Elimination
        // this is done by converting the vector 'matrix' in to an identity matrix
        // and performing the same set of operations to the vector 'inverse'
        
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                
                // make the diagonal = 1
                
                // ensure diagonal elements are non-zero
                // do nothing to elements above the diagonal
                if (i > j) {
                }
                // look at the diagonal elements
                else if (i == j) {
                    // if the element is zero...
                    if (matrix[j][i] == 0) {
                        for (int k = 0; k < matrix.size(); k++) {
                            // find elements below it...
                            if (k > j) {
                                // that are non-zero...
                                if ( matrix[k][i] != 0) {
                                    
                                    // sum the rows...
                                    for (int m = 0; m < matrix.size(); m++) {
                                        matrix[j][m] = matrix[j][m] + matrix[k][m];
                                        inverse[j][m] = inverse[j][m] + inverse[k][m];
                                    }
                                    // and divide the row by itself
                                    double x = matrix[j][i];
                                    for (int m = 0; m < matrix.size(); m++) {
                                        matrix[j][m] = matrix[j][m] / x;
                                        inverse[j][m] = inverse[j][m] / x;
                                    }
                                    
                                }
                            }
                        }
                    }
                    
                    // if the element is non-zero, divide by itself
                    else {
                        double x = matrix[j][i];
                        for (int m = 0; m < matrix.size(); m++) {
                           matrix[j][m] = matrix[j][m] / x;
                           inverse[j][m] = inverse[j][m] / x;
                        }
                    }
                }
                
                else {
                    // if the element is below the leading diagonal, make it 0
                    // do nothing if the element is already 0
                    if ( matrix[j][i] == 0 ) {
                    }
                    else {
                        // if it is non-zero, transform it in to a 1 (divide by itself)...
                        double x = matrix[j][i];
                        for (int m = 0; m < matrix.size(); m++) {
                            matrix[j][m] = matrix[j][m] / x;
                            inverse[j][m] = inverse[j][m] / x;
                        }
                        // and subtract the row with the diagonal element = 1
                        for (int m = 0; m < matrix.size(); m++) {
                            matrix[j][m] = matrix[j][m] - matrix[i][m];
                            inverse[j][m] = inverse[j][m] - inverse[i][m];
                        }
                    }
                    
                }
            }
        }
        
        // use back substitution to transform 'matrix' in to an identity matrix
        
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                // for elements above the diagonal
                if (i > j) {
                    // that are non-zero
                    if (matrix[j][i] != 0) {
                        
                        double x = matrix[j][i];
                        
                        //Divide the row by itself
                        for (int m = 0; m < matrix.size(); m++) {
                            matrix[j][m] = matrix[j][m] / x;
                            inverse[j][m] = inverse[j][m] / x;
                        }
                        // subtract row with diagonal = 1 in the same column
                        for (int m = 0; m < matrix.size(); m++) {
                            matrix[j][m] = matrix[j][m] - matrix[i][m];
                            inverse[j][m] = inverse[j][m] - inverse[i][m];
                        }
                        // multiply by the original row
                        for (int m = 0; m < matrix.size(); m++) {
                            matrix[j][m] = matrix[j][m] * x;
                            inverse[j][m] = inverse[j][m] * x;
                        }
                        
                    }
                }
            }
        }
        
        // remove " - " from 0s
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                if (inverse[i][j] == -0) {
                    inverse[i][j] = +0;
                }
            }
        }
        
        // check that read in matrix has been transformed to identity matrix
        for (int j = 0; j < matrix.size(); j++) {
            for (int i = 0; i < matrix.size(); i++) {
                if (i == j) {
                    if (matrix[j][i] == 1) {}
                    else {
                        cout << "Error.  Matrix cannot be inverted.\n";
                        exit(4);
                    }
                }
                else if (i != j) {
                    if (matrix[j][i] == 0) {}
                    else {
                        cout << "Error.  Matrix cannot be inverted.\n";
                        exit(4);
                    }
                }
            }
        }
    }
    else { // matches if (thefile.is_open()) {
        // write out an error if data not read in
        cout << "Unable to read file.\n";
        exit(1);
    }
    
    
    // write the matrix out to a new file
    // create new .txt file to write out to
    ofstream newfile("inverse.txt");
    // check the write file is open
    if (newfile.is_open()) {
        for (int i = 0; i < inverse.size(); i++) {
            for (int j = 0; j < inverse.size(); j++) {
                // write the data in to the new file
                newfile << setw(3) << inverse[i][j] << ((j == inverse.size() - 1) ? "" : ",");
            }
            // make a new line after each row
            newfile << "\n";
        }
        // close the write file
        newfile.close();
        
    }
    else {
        // unable to write to file, print out message and exit
        cout << "Unable to write to file.\n";
        exit(5);
    }
    
    return 0;
}
