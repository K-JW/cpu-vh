/*
 * FullyDiscreteKurganovTadmorScheme.h
 *
 *  Created on: Oct 23, 2015
 *      Author: bazow
 */

#ifndef FULLYDISCRETEKURGANOVTADMORSCHEME_H_
#define FULLYDISCRETEKURGANOVTADMORSCHEME_H_

//#define SIMD //for acceleration via vector instructions - See FullyDiscreteKurganovTadmorScheme.cpp and DynamicalVariables.cpp
#define TILE //for acceleration via memory tiling (cache blocking) - handled by compiler

#include "DynamicalVariables.h"

void rungeKutta2(PRECISION t, PRECISION dt, CONSERVED_VARIABLES * __restrict__ q, CONSERVED_VARIABLES * __restrict__ Q,
void * latticeParams, void * hydroParams
);

#endif /* FULLYDISCRETEKURGANOVTADMORSCHEME_H_ */
