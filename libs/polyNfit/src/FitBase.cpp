/*
 *  FitBase.cpp
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 29/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

///////////////////////////////////////////////////////////////////////////////
//
// 'bases' = the number of bases in the polynomial
// f       = a pointer to a function f(n,x) that returns the value of the
//	     'n'th basis at the point *x
//
///////////////////////////////////////////////////////////////////////////////

#include "FitBase.h"

template<class T>
FitBase<T>::FitBase(int bases, int indim, int outdim) : 
A(bases, bases),
_indim(indim), _outdim(outdim), y(bases), _bases(bases)
{
	for (int iBasis = 0; iBasis < bases; iBasis++)
		y[iBasis].resize(outdim);
	
}


template<class T>
FitBase<T>::~FitBase() {

} 


///////////////////////////////////////////////////////////////////////////////
//
// Solves the equation Mx=y[][I] for x, storing the result in 'x'
// i.e. fit a polynomial to the I'th output dimension and store the
// coefficients in 'x'. where x[n] is the coefficient of the 'n'th
// basis, as given by the basis function.
//
///////////////////////////////////////////////////////////////////////////////

template<class T>
void FitBase<T>::solve(int I, T *x, unsigned int &nCoefficients) {
	
	
	int n,i,j;
	
	n=A.jsize();
	
	for (i=0;i<n;i++) {
		x[i]=y[i][I];
		for (j=0; j<i; j++) {
			x[i]=x[i]-A(i,j)*x[j];
		}
	}
	
	x[n-1]=x[n-1]/A(n-1,n-1) ; 
	for (i=n-2;i>=0;i--) {
		for (j=i+1; j<n; j++) { 
			x[i]=x[i]-A(i,j)*x[j] ;  
		}
		x[i]=x[i]/A(i,i) ;
	}
	
	nCoefficients = _bases;
}

///////////////////////////////////////////////////////////////////////////////
//
// Initialises the matrix A, and the values of y for the coordinates
// in 'points' and the corresponding observed values in 'data',
// then decomposes A into its LU-decomposition.
// 'M' gives the number of datapoints in 'points' and 'data'
//
///////////////////////////////////////////////////////////////////////////////
template<class T>
void FitBase<T>::init(pfitDataSet<T> &dataSet)
{
    const pfitIndex M = dataSet.getActiveCount();
    
	const int N = A.jsize();
	T *bas = new T[N];
	
	int n, k;
	int i, j;
	
	// reset lower corner of this matrix
	for(i = 0; i < N; ++i) {
		for(j =0; j <= i; ++j) {
			A(i,j) = 0.0;
		}
	}
	// reset y
	for(i = 0; i < N; ++i) {
		for(j =0; j < _outdim; ++j) {
			y[i][j] = 0.0;
		}
	}
	
	// Form sums over each datapoint
    T *output;
	for(pfitDataPoint<T> point = dataSet.begin(); point != dataSet.end(); ++point)
    {
        if (!point.getActive())
            continue;
        
		// Form matrix of bases for this data point (x values)
		for(n = 0; n < N; ++n) {
			bas[n] = basis(n, point);
		}
		
		// fill in this matrix
		for(i = 0; i < N; ++i) {
			for(j =0; j <= i; ++j) {
				A(i,j) += bas[i]*bas[j];
			}
		}
		
		// Fill in y values
        output = point.getOutput();
        //
		for(i = 0; i<N; ++i) {
			for(j = 0; j < _outdim; ++j) {
				y[i][j] += bas[i] * output[j];
			}
		}
	}
	
	// copy to upper corner
	for(i = 0; i < N; ++i) {
		for(j =0; j < i; ++j) {
			A(j,i) = A(i,j);
		}
	}
	
	// Decompose A
	A.LUdecomp(); 
	
	delete [] bas;
}

template class FitBase<double>;
template class FitBase<float>;
