/*
 *  basisTransformIndices.cpp
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 26/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "basisTransformIndices.h"

int basisTransformIndices::updatebasisIndices(int nDimensions, int nPowerOrder)
{
	_nDimensions = nDimensions;
	_nPowerOrder = nPowerOrder;
	
	if (_nDimensions != _renderedNDimensions
		|| _nPowerOrder != _renderedNPowerOrder)
	{
		calc();
		_renderedNDimensions = _nDimensions;
		_renderedNPowerOrder = _nPowerOrder;
	}
	
	return _nBases;
}

void basisTransformIndices::calc()
{
	// create temporary variables for each item
	int *iPossibleItemX = new int[_nDimensions];
	bool present;
	
	//
	// clean out bases
	//
	clear();
	
	//
	// iterate through all possible items for square/cubic/etc
	//
	for (int iPossibleItem=0; iPossibleItem < pow(_nPowerOrder+1,(double)_nDimensions); iPossibleItem++)
	{
		
		//
		// calculate Indices in each dimension for this item
		//
		int iOrderSum=0;
		
		for (int iDimension=0; iDimension < _nDimensions; iDimension++)
		{
			iPossibleItemX[iDimension] = iPossibleItem 
			/ (int)pow(_nPowerOrder+1,(double)iDimension)
			% (_nPowerOrder+1);
			
			if (iDimension<_nDimensions-1)
				iOrderSum += iPossibleItemX[iDimension];
		}
		
		
		//
		// determine whether current index should be selected
		//
		present = iOrderSum < (_nPowerOrder+1) - iPossibleItemX[_nDimensions-1];
		
		
		//
		// Fill array with data Indices
		//
		
		int idxbasisIndices;
		if (present)
		{
			vecBasisIndices.push_back(new unsigned int[_nDimensions]);
			idxbasisIndices = vecBasisIndices.size()-1;
			
			for (int iDimension=0; iDimension<_nDimensions; iDimension++)
			{
				vecBasisIndices.at(idxbasisIndices)[iDimension]=iPossibleItemX[iDimension];
				
			}
		}		
	}
	
	_nBases = vecBasisIndices.size();
	
	_renderedNDimensions = _nDimensions;
	_renderedNPowerOrder = _nPowerOrder;

	delete[] iPossibleItemX;
}

void basisTransformIndices::clear()
{
	for (int iBasis=0; iBasis < vecBasisIndices.size(); iBasis++)
		delete[] vecBasisIndices[iBasis];

	vecBasisIndices.clear();
	_nBases=0;
}