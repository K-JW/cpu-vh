#include "../rhic/include/DynamicalVariables.h"
#include "../rhic/include/EquationOfState.h"
#include "../rhic/include/FreezeOut.h"
#include "cornelius-c++-1.3/cornelius.cpp"
#include <stdio.h>
#include <iostream>
#include <fstream>

//return a 4 dimensional linear interpolation inside the hypercube, given the values
//on the corners (a0000 through a1111) and edge lengths x0 through x3
double linearInterp4D(double x0, double x1, double x2, double x3,
                      double a0000, double a1000, double a0100, double a0010, double a0001,
                      double a1100, double a1010, double a1001,
                      double a0110, double a0101, double a0011,
                      double a1110, double a1101, double a0111, double a1011, double a1111)
{
  double result = 0;
  result = ((1-x0) * (1-x1) * (1-x2) * (1-x3) * a0000)
            + ((x0) * (1-x1) * (1-x2) * (1-x3) * a1000)
            + ((1-x0) * (x1) * (1-x2) * (1-x3) * a0100)
            + ((1-x0) * (1-x1) * (x2) * (1-x3) * a0010)
            + ((1-x0) * (1-x1) * (1-x2) * (x3) * a0001)
            + ((x0) * (x1) * (1-x2) * (1-x3) * a1100)
            + ((x0) * (1-x1) * (x2) * (1-x3) * a1010)
            + ((x0) * (1-x1) * (1-x2) * (x3) * a1001)
            + ((1-x0) * (x1) * (x2) * (1-x3) * a0110)
            + ((1-x0) * (x1) * (1-x2) * (x3) * a0101)
            + ((1-x0) * (1-x1) * (x2) * (x3) * a0011)
            + ((x0) * (x1) * (x2) * (1-x3) * a1110)
            + ((x0) * (x1) * (1-x2) * (x3) * a1101)
            + ((x0) * (1-x1) * (x2) * (x3) * a1011)
            + ((1-x0) * (x1) * (x2) * (x3) * a0111)
            + ((x0) * (x1) * (x2) * (x3) * a1111);

  return result;
}

double linearInterp3D(double x0, double x1, double x2,
                      double a000, double a100, double a010, double a001,
                      double a110, double a101, double a011, double a111)
{
  double result = 0;
  result = ((1-x0) * (1-x1) * (1-x2) * a000)
            + ((x0) * (1-x1) * (1-x2) * a100)
            + ((1-x0) * (x1) * (1-x2) * a010)
            + ((1-x0) * (1-x1) * (x2) * a001)
            + ((x0) * (x1) * (1-x2) * a110)
            + ((x0) * (1-x1) * (x2) * a101)
            + ((1-x0) * (x1) * (x2) * a011)
            + ((x0) * (x1) * (x2)  * a111);

  return result;
}

void swapAndSetHydroVariables(double ****energy_density_evoution, double *****hydrodynamic_evoution,
                              CONSERVED_VARIABLES * const __restrict__ q, PRECISION * const __restrict__ e,
                              FLUID_VELOCITY * const __restrict__ u, int nx, int ny, int nz, int FOFREQ)
{
  #pragma omp parallel for collapse(3)
  for (int ix = 2; ix < nx+2; ix++)
  {
    for (int iy = 2; iy < ny+2; iy++)
    {
      for (int iz = 2; iz < nz+2; iz++)
      {
        int s = columnMajorLinearIndex(ix, iy, iz, nx+4, ny+4, nz+4);
        //previous hydro variable values written to zeroth index
        energy_density_evoution[0][ix-2][iy-2][iz-2] = energy_density_evoution[FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[0][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[0][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[1][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[1][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[2][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[2][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[3][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[3][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[4][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[4][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[5][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[5][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[6][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[6][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[7][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[7][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[8][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[8][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[9][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[9][FOFREQ][ix-2][iy-2][iz-2];

        //current hydro variable values written to first index
        energy_density_evoution[1][ix-2][iy-2][iz-2] = (double)e[s];
        hydrodynamic_evoution[0][1][ix-2][iy-2][iz-2] = (double)(u->ux[s]);
        hydrodynamic_evoution[1][1][ix-2][iy-2][iz-2] = (double)(u->uy[s]);
        hydrodynamic_evoution[2][1][ix-2][iy-2][iz-2] = (double)(u->un[s]);
        hydrodynamic_evoution[3][1][ix-2][iy-2][iz-2] = (double)(e[s]);
	#ifdef PIMUNU
        hydrodynamic_evoution[4][1][ix-2][iy-2][iz-2] = (double)(q->pixx[s]);
        hydrodynamic_evoution[5][1][ix-2][iy-2][iz-2] = (double)(q->pixy[s]);
        hydrodynamic_evoution[6][1][ix-2][iy-2][iz-2] = (double)(q->pixn[s]);
        hydrodynamic_evoution[7][1][ix-2][iy-2][iz-2] = (double)(q->piyy[s]);
        hydrodynamic_evoution[8][1][ix-2][iy-2][iz-2] = (double)(q->piyn[s]);
	#endif
	#ifdef PI
        hydrodynamic_evoution[9][1][ix-2][iy-2][iz-2] = (double)(q->Pi[s]);
	#endif
      }
    } //for (int iy = 2; iy < ny+2; iy++)
  } //for (int ix = 2; ix < nx+2; ix++)
}

void setHydroVariables(double ****energy_density_evoution, double *****hydrodynamic_evoution,
                              CONSERVED_VARIABLES * const __restrict__ q, PRECISION * const __restrict__ e,
                              FLUID_VELOCITY * const __restrict__ u, int nx, int ny, int nz, int FOFREQ, int n)
{
  int nFO = n % FOFREQ;
  #pragma omp parallel for collapse(3)
  for (int ix = 2; ix < nx+2; ix++)
  {
    for (int iy = 2; iy < ny+2; iy++)
    {
      for (int iz = 2; iz < nz+2; iz++)
      {
        int s = columnMajorLinearIndex(ix, iy, iz, nx+4, ny+4, nz+4);
        energy_density_evoution[nFO+1][ix-2][iy-2][iz-2] = (double)e[s];
        hydrodynamic_evoution[0][nFO+1][ix-2][iy-2][iz-2] = (double)(u->ux[s]);
        hydrodynamic_evoution[1][nFO+1][ix-2][iy-2][iz-2] = (double)(u->uy[s]);
        hydrodynamic_evoution[2][nFO+1][ix-2][iy-2][iz-2] = (double)(u->un[s]);
        hydrodynamic_evoution[3][nFO+1][ix-2][iy-2][iz-2] = (double)(e[s]);
	#ifdef PIMUNU
        hydrodynamic_evoution[4][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixx[s]);
        hydrodynamic_evoution[5][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixy[s]);
        hydrodynamic_evoution[6][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixn[s]);
        hydrodynamic_evoution[7][nFO+1][ix-2][iy-2][iz-2] = (double)(q->piyy[s]);
        hydrodynamic_evoution[8][nFO+1][ix-2][iy-2][iz-2] = (double)(q->piyn[s]);
	#endif
	#ifdef PI
        hydrodynamic_evoution[9][nFO+1][ix-2][iy-2][iz-2] = (double)(q->Pi[s]);
	#endif
      }
    } //for (int iy = 2; iy < ny+2; iy++)
  } // for (int ix = 2; ix < nx+2; ix++)
}

//freezeout functions for vorticity and polarization studies

void swapAndSetHydroVariables_Vorticity(double ****energy_density_evoution, double *****hydrodynamic_evoution,
                              CONSERVED_VARIABLES * const __restrict__ q, PRECISION * const __restrict__ e,
                              FLUID_VELOCITY * const __restrict__ u, VORTICITY * const __restrict__ wmunu,
                              int nx, int ny, int nz, int FOFREQ)
{
  #pragma omp parallel for collapse(3)
  for (int ix = 2; ix < nx+2; ix++)
  {
    for (int iy = 2; iy < ny+2; iy++)
    {
      for (int iz = 2; iz < nz+2; iz++)
      {
        int s = columnMajorLinearIndex(ix, iy, iz, nx+4, ny+4, nz+4);
        //previous hydro variable values written to zeroth index
        energy_density_evoution[0][ix-2][iy-2][iz-2] = energy_density_evoution[FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[0][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[0][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[1][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[1][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[2][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[2][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[3][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[3][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[4][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[4][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[5][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[5][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[6][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[6][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[7][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[7][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[8][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[8][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[9][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[9][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[10][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[10][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[11][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[11][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[12][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[12][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[13][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[13][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[14][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[14][FOFREQ][ix-2][iy-2][iz-2];
        hydrodynamic_evoution[15][0][ix-2][iy-2][iz-2] = hydrodynamic_evoution[15][FOFREQ][ix-2][iy-2][iz-2];

        //current hydro variable values written to first index
        energy_density_evoution[1][ix-2][iy-2][iz-2] = (double)e[s];
        hydrodynamic_evoution[0][1][ix-2][iy-2][iz-2] = (double)(u->ux[s]);
        hydrodynamic_evoution[1][1][ix-2][iy-2][iz-2] = (double)(u->uy[s]);
        hydrodynamic_evoution[2][1][ix-2][iy-2][iz-2] = (double)(u->un[s]);
        hydrodynamic_evoution[3][1][ix-2][iy-2][iz-2] = (double)(e[s]);
	#ifdef PIMUNU
        hydrodynamic_evoution[4][1][ix-2][iy-2][iz-2] = (double)(q->pixx[s]);
        hydrodynamic_evoution[5][1][ix-2][iy-2][iz-2] = (double)(q->pixy[s]);
        hydrodynamic_evoution[6][1][ix-2][iy-2][iz-2] = (double)(q->pixn[s]);
        hydrodynamic_evoution[7][1][ix-2][iy-2][iz-2] = (double)(q->piyy[s]);
        hydrodynamic_evoution[8][1][ix-2][iy-2][iz-2] = (double)(q->piyn[s]);
	#endif
	#ifdef PI
        hydrodynamic_evoution[9][1][ix-2][iy-2][iz-2] = (double)(q->Pi[s]);
	#endif
  #ifdef THERMAL_VORTICITY
        hydrodynamic_evoution[10][1][ix-2][iy-2][iz-2] = (double)(wmunu->wtx[s]);
        hydrodynamic_evoution[11][1][ix-2][iy-2][iz-2] = (double)(wmunu->wty[s]);
        hydrodynamic_evoution[12][1][ix-2][iy-2][iz-2] = (double)(wmunu->wtn[s]);
        hydrodynamic_evoution[13][1][ix-2][iy-2][iz-2] = (double)(wmunu->wxy[s]);
        hydrodynamic_evoution[14][1][ix-2][iy-2][iz-2] = (double)(wmunu->wxn[s]);
        hydrodynamic_evoution[15][1][ix-2][iy-2][iz-2] = (double)(wmunu->wyn[s]);
  #endif
      }
    } //for (int iy = 2; iy < ny+2; iy++)
  } //for (int ix = 2; ix < nx+2; ix++)
}

void setHydroVariables_Vorticity(double ****energy_density_evoution, double *****hydrodynamic_evoution,
                              CONSERVED_VARIABLES * const __restrict__ q, PRECISION * const __restrict__ e,
                              FLUID_VELOCITY * const __restrict__ u, VORTICITY * const __restrict__ wmunu,
                              int nx, int ny, int nz, int FOFREQ, int n)
{
  int nFO = n % FOFREQ;
  #pragma omp parallel for collapse(3)
  for (int ix = 2; ix < nx+2; ix++)
  {
    for (int iy = 2; iy < ny+2; iy++)
    {
      for (int iz = 2; iz < nz+2; iz++)
      {
        int s = columnMajorLinearIndex(ix, iy, iz, nx+4, ny+4, nz+4);
        energy_density_evoution[nFO+1][ix-2][iy-2][iz-2] = (double)e[s];
        hydrodynamic_evoution[0][nFO+1][ix-2][iy-2][iz-2] = (double)(u->ux[s]);
        hydrodynamic_evoution[1][nFO+1][ix-2][iy-2][iz-2] = (double)(u->uy[s]);
        hydrodynamic_evoution[2][nFO+1][ix-2][iy-2][iz-2] = (double)(u->un[s]);
        hydrodynamic_evoution[3][nFO+1][ix-2][iy-2][iz-2] = (double)(e[s]);
	#ifdef PIMUNU
        hydrodynamic_evoution[4][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixx[s]);
        hydrodynamic_evoution[5][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixy[s]);
        hydrodynamic_evoution[6][nFO+1][ix-2][iy-2][iz-2] = (double)(q->pixn[s]);
        hydrodynamic_evoution[7][nFO+1][ix-2][iy-2][iz-2] = (double)(q->piyy[s]);
        hydrodynamic_evoution[8][nFO+1][ix-2][iy-2][iz-2] = (double)(q->piyn[s]);
	#endif
	#ifdef PI
        hydrodynamic_evoution[9][nFO+1][ix-2][iy-2][iz-2] = (double)(q->Pi[s]);
	#endif
  #ifdef THERMAL_VORTICITY
        hydrodynamic_evoution[10][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wtx[s]);
        hydrodynamic_evoution[11][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wty[s]);
        hydrodynamic_evoution[12][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wtn[s]);
        hydrodynamic_evoution[13][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wxy[s]);
        hydrodynamic_evoution[14][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wxn[s]);
        hydrodynamic_evoution[15][nFO+1][ix-2][iy-2][iz-2] = (double)(wmunu->wyn[s]);
  #endif
      }
    } //for (int iy = 2; iy < ny+2; iy++)
  } // for (int ix = 2; ix < nx+2; ix++)
}


void writeEnergyDensityToHypercube4D(double ****hyperCube, double ****energy_density_evoution, int it, int ix, int iy, int iz)
{
  hyperCube[0][0][0][0] = energy_density_evoution[it][ix][iy][iz];
  hyperCube[1][0][0][0] = energy_density_evoution[it+1][ix][iy][iz];
  hyperCube[0][1][0][0] = energy_density_evoution[it][ix+1][iy][iz];
  hyperCube[0][0][1][0] = energy_density_evoution[it][ix][iy+1][iz];
  hyperCube[0][0][0][1] = energy_density_evoution[it][ix][iy][iz+1];
  hyperCube[1][1][0][0] = energy_density_evoution[it+1][ix+1][iy][iz];
  hyperCube[1][0][1][0] = energy_density_evoution[it+1][ix][iy+1][iz];
  hyperCube[1][0][0][1] = energy_density_evoution[it+1][ix][iy][iz+1];
  hyperCube[0][1][1][0] = energy_density_evoution[it][ix+1][iy+1][iz];
  hyperCube[0][1][0][1] = energy_density_evoution[it][ix+1][iy][iz+1];
  hyperCube[0][0][1][1] = energy_density_evoution[it][ix][iy+1][iz+1];
  hyperCube[1][1][1][0] = energy_density_evoution[it+1][ix+1][iy+1][iz];
  hyperCube[1][1][0][1] = energy_density_evoution[it+1][ix+1][iy][iz+1];
  hyperCube[1][0][1][1] = energy_density_evoution[it+1][ix][iy+1][iz+1];
  hyperCube[0][1][1][1] = energy_density_evoution[it][ix+1][iy+1][iz+1];
  hyperCube[1][1][1][1] = energy_density_evoution[it+1][ix+1][iy+1][iz+1];
}
void writeEnergyDensityToHypercube3D(double ***hyperCube, double ****energy_density_evoution, int it, int ix, int iy)
{
  hyperCube[0][0][0] = energy_density_evoution[it][ix][iy][0];
  hyperCube[1][0][0] = energy_density_evoution[it+1][ix][iy][0];
  hyperCube[0][1][0] = energy_density_evoution[it][ix+1][iy][0];
  hyperCube[0][0][1] = energy_density_evoution[it][ix][iy+1][0];
  hyperCube[1][1][0] = energy_density_evoution[it+1][ix+1][iy][0];
  hyperCube[1][0][1] = energy_density_evoution[it+1][ix][iy+1][0];
  hyperCube[0][1][1] = energy_density_evoution[it][ix+1][iy+1][0];
  hyperCube[1][1][1] = energy_density_evoution[it+1][ix+1][iy+1][0];
}
double interpolateVariable4D(double *****hydrodynamic_evoution, int ivar, int it, int ix, int iy, int iz, double tau_frac, double x_frac, double y_frac, double z_frac)
{
  double result = linearInterp4D(tau_frac, x_frac, y_frac, z_frac,
    hydrodynamic_evoution[ivar][it][ix][iy][iz], hydrodynamic_evoution[ivar][it+1][ix][iy][iz], hydrodynamic_evoution[ivar][it][ix+1][iy][iz], hydrodynamic_evoution[ivar][it][ix][iy+1][iz], hydrodynamic_evoution[ivar][it][ix][iy][iz+1],
    hydrodynamic_evoution[ivar][it+1][ix+1][iy][iz], hydrodynamic_evoution[ivar][it+1][ix][iy+1][iz], hydrodynamic_evoution[ivar][it+1][ix][iy][iz+1],
    hydrodynamic_evoution[ivar][it][ix+1][iy+1][iz], hydrodynamic_evoution[ivar][it][ix+1][iy][iz+1], hydrodynamic_evoution[ivar][it][ix][iy+1][iz+1],
    hydrodynamic_evoution[ivar][it+1][ix+1][iy+1][iz], hydrodynamic_evoution[ivar][it+1][ix+1][iy][iz+1], hydrodynamic_evoution[ivar][it][ix+1][iy+1][iz+1], hydrodynamic_evoution[ivar][it+1][ix][iy+1][iz+1], hydrodynamic_evoution[ivar][it+1][ix+1][iy+1][iz+1]);
    return result;
}

double interpolateVariable3D(double *****hydrodynamic_evoution, int ivar, int it, int ix, int iy, double tau_frac, double x_frac, double y_frac)
{
  double result = linearInterp3D(tau_frac, x_frac, y_frac,
    hydrodynamic_evoution[ivar][it][ix][iy][0], hydrodynamic_evoution[ivar][it+1][ix][iy][0], hydrodynamic_evoution[ivar][it][ix+1][iy][0], hydrodynamic_evoution[ivar][it][ix][iy+1][0],
    hydrodynamic_evoution[ivar][it+1][ix+1][iy][0], hydrodynamic_evoution[ivar][it+1][ix][iy+1][0], hydrodynamic_evoution[ivar][it][ix+1][iy+1][0], hydrodynamic_evoution[ivar][it+1][ix+1][iy+1][0]);
    return result;
}

void callFOFinder(int dim, int start, int nx, int ny, int nz, int n, double t0, double dt, double t, double dx, double dy, double dz, double *lattice_spacing, double freezeoutEnergyDensity,
  double ****hyperCube4D, double ***hyperCube3D, double ****energy_density_evoution, double *****hydrodynamic_evoution,
  std::ofstream& freezeoutSurfaceFile, std::vector<FO_Element>& fo_surf)
  {
    int FOFREQ = 10;

    Cornelius cor;
    cor.init(dim, freezeoutEnergyDensity, lattice_spacing);

    //besides writing centroid and normal to file, write all the hydro variables
    int dimZ;
    if (dim == 4) dimZ = nz-1; //enter the loop over iz, and avoid problems at boundary
    else if (dim == 3) dimZ = 1; //we need to enter the 'loop' over iz rather than skipping it
    for (int it = start; it < FOFREQ; it++) //note* avoiding boundary problems (reading outside array)
    {
      for (int ix = 0; ix < nx-1; ix++)
      {
        for (int iy = 0; iy < ny-1; iy++)
        {
          for (int iz = 0; iz < dimZ; iz++)
          {
            //write the values of energy density to all corners of the hyperCube
            if (dim == 4) writeEnergyDensityToHypercube4D(hyperCube4D, energy_density_evoution, it, ix, iy, iz);
            else if (dim == 3) writeEnergyDensityToHypercube3D(hyperCube3D, energy_density_evoution, it, ix, iy);

            //use cornelius to find the centroid and normal vector of each hyperCube
            if (dim == 4) cor.find_surface_4d(hyperCube4D);
            else if (dim == 3) cor.find_surface_3d(hyperCube3D);
            //write centroid and normal of each surface element to file
            for (int i = 0; i < cor.get_Nelements(); i++)
            {
              //declare a new fo cell to hold info, later push back to vector
              //FO_Element fo_cell;

              double temp = 0.0; //temporary variable
              //first write the position of the centroid of surface element
              double cell_tau;
              if (n <= FOFREQ) cell_tau = t0 + ( (double)(n - FOFREQ + (it-1) ) )* dt; //check if this is the correct time!
              else cell_tau = t0 + ((double)(n - FOFREQ + it)) * dt; //check if this is the correct time!
              double cell_x = (double)ix * dx  - (((double)(nx-1)) / 2.0 * dx);
              double cell_y = (double)iy * dy  - (((double)(ny-1)) / 2.0 * dy);
              double cell_z = (double)iz * dz  - (((double)(nz-1)) / 2.0 * dz);

              double tau_frac = cor.get_centroid_elem(i,0) / lattice_spacing[0];
              double x_frac = cor.get_centroid_elem(i,1) / lattice_spacing[1];
              double y_frac = cor.get_centroid_elem(i,2) / lattice_spacing[2];
              double z_frac;
              if (dim == 4) z_frac = cor.get_centroid_elem(i,3) / lattice_spacing[3];
              else z_frac = 0.0;

              freezeoutSurfaceFile << cor.get_centroid_elem(i,0) + cell_tau << " ";
              //fo_cell.tau = (cor.get_centroid_elem(i,0) + cell_tau);

              freezeoutSurfaceFile << cor.get_centroid_elem(i,1) + cell_x << " ";
              //fo_cell.x = cor.get_centroid_elem(i,1) + cell_x;
              freezeoutSurfaceFile << cor.get_centroid_elem(i,2) + cell_y << " ";
              //fo_cell.y = cor.get_centroid_elem(i,2) + cell_y;
              if (dim == 4)
              {
                freezeoutSurfaceFile << cor.get_centroid_elem(i,3) + cell_z << " ";
                //fo_cell.eta = cor.get_centroid_elem(i,3) + cell_z;
              }
              else
              {
                freezeoutSurfaceFile << cell_z << " ";
                //fo_cell.eta = cell_z;
              }
              //acording to cornelius user guide, corenelius returns covariant components of normal vector without jacobian factors
              freezeoutSurfaceFile << t * cor.get_normal_elem(i,0) << " ";
              //fo_cell.dat = t * cor.get_normal_elem(i,0);
              freezeoutSurfaceFile << t * cor.get_normal_elem(i,1) << " ";
              //fo_cell.dax = t * cor.get_normal_elem(i,1);
              freezeoutSurfaceFile << t * cor.get_normal_elem(i,2) << " ";
              //fo_cell.day = t * cor.get_normal_elem(i,2);
              if (dim == 4)
              {
                freezeoutSurfaceFile << t * cor.get_normal_elem(i,3) << " ";
                //fo_cell.dan = t * cor.get_normal_elem(i,3);
              }
              else
              {
                freezeoutSurfaceFile << 0.0 << " ";
                //fo_cell.dan = 0.0;
              }
              //write all necessary variables first performing linear interpolation from values at corners of hypercube

              if (dim == 4) // for 3+1D
              {
                //first write the contravariant flow velocity
                temp = interpolateVariable4D(hydrodynamic_evoution, 0, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.ux = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 1, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.uy = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 2, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.un = temp;

                //write the energy density
                temp = interpolateVariable4D(hydrodynamic_evoution, 3, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " "; //note : iSpectra reads in file in fm^x units e.g. energy density should be written in fm^-4
                //fo_cell.E = temp;
                //the temperature
                freezeoutSurfaceFile << effectiveTemperature(temp) << " ";
                //fo_cell.T = effectiveTemperature(temp);
                //the thermal pressure
                freezeoutSurfaceFile << equilibriumPressure(temp) << " ";
                //fo_cell.P = equilibriumPressure(temp);

                //write five indep components of pi^(mu,nu) shear viscous tensor
                temp = interpolateVariable4D(hydrodynamic_evoution, 4, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixx = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 5, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixy = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 6, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixn = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 7, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.piyy = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 8, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.piyn = temp;

                //write the bulk pressure Pi
                temp = interpolateVariable4D(hydrodynamic_evoution, 9, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp;
                //fo_cell.Pi = temp;

                //write 6 components of w^\mu\nu thermal vorticity tensor
                #ifdef THERMAL_VORTICITY
                for (int ivar = 10; ivar < 16; ivar++)
                temp = interpolateVariable4D(hydrodynamic_evoution, 10, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wtx = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 11, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wty = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 12, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wtn = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 13, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wxy = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 14, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wxn = temp;
                temp = interpolateVariable4D(hydrodynamic_evoution, 15, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.wyn = temp;
                #endif

                freezeoutSurfaceFile << endl;
              }

              else //for 2+1D
              {
                //first write the flow velocity
                for (int ivar = 0; ivar < 3; ivar++)
                temp = interpolateVariable3D(hydrodynamic_evoution, 0, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.ux = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 1, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.uy = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 2, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.un = temp;

                //write the energy density
                temp = interpolateVariable3D(hydrodynamic_evoution, 3, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " "; //note units of fm^-4 appropriate for iSpectra reading
                //fo_cell.E = temp;
                //the temperature
                freezeoutSurfaceFile << effectiveTemperature(temp) << " ";
                //fo_cell.T = effectiveTemperature(temp);
                //the thermal pressure
                freezeoutSurfaceFile << equilibriumPressure(temp) << " ";
                //fo_cell.P = equilibriumPressure(temp);

                //write five indep components of pi_(mu,nu) shear viscous tensor
                temp = interpolateVariable3D(hydrodynamic_evoution, 4, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixx = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 5, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixy = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 6, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.pixn = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 7, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.piyy = temp;
                temp = interpolateVariable3D(hydrodynamic_evoution, 8, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp << " ";
                //fo_cell.piyn = temp;

                //write the bulk pressure Pi
                temp = interpolateVariable3D(hydrodynamic_evoution, 9, it, ix, iy, tau_frac, x_frac, y_frac);
                freezeoutSurfaceFile << temp;
                #ifdef THERMAL_VORTICITY
                for (int ivar = 10; ivar < 16; ivar++)
                {
                  temp = interpolateVariable4D(hydrodynamic_evoution, ivar, it, ix, iy, iz, tau_frac, x_frac, y_frac, z_frac);
                  freezeoutSurfaceFile << temp << " ";
                }
                #endif
                freezeoutSurfaceFile << endl;
              }

              //add the fo cell to fo surface
              //#pragma omp critical
              //fo_surf.push_back(fo_cell);

            } //for (int i = 0; i < cor.get_Nelements(); i++)
          } // for (int iz = 0; iz < dimZ; iz++)
        } // for (int iy = 0; iy < ny-1; iy++)
      } // for (int ix = 0; ix < nx-1; ix++)
    } //for (int it = start; it < FOFREQ; it++)
  }

  //returns the number of cells with T > T_c
  int checkForCellsAboveTc(int nx, int ny, int nz, double freezeoutEnergyDensity, PRECISION *e)
  {
    int accumulator = 0;

    for (int ix = 2; ix < nx+2; ix++)
    {
      for (int iy = 2; iy < ny+2; iy++)
      {
        for (int iz = 2; iz < nz+2; iz++)
        {
          int s = columnMajorLinearIndex(ix, iy, iz, nx+4, ny+4, nz+4);
          if (e[s] > freezeoutEnergyDensity) accumulator += 1;
        }
      }
    }

    return accumulator;
  }
