/*
* LatticeParameters.c
*
*  Created on: Oct 22, 2015
*      Author: bazow
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>

#include "../include/LatticeParameters.h"
#include "../include/Properties.h"

int numLatticePointsX;
int numLatticePointsY;
int numLatticePointsRapidity;
int numProperTimePoints;
int coarseProperTimeFOFactor;

double latticeSpacingX;
double latticeSpacingY;
double latticeSpacingRapidity;
double latticeSpacingProperTime;

double fluxLimiterTheta;

//requires libconfig
/*
void loadLatticeParameters(config_t *cfg, const char* configDirectory, void * params) {
	// Read the file
	char fname[255];
	sprintf(fname, "%s/%s", configDirectory, "lattice.properties");
	if (!config_read_file(cfg, fname)) {
		fprintf(stderr, "No configuration file  %s found for lattice parameters - %s.\n", fname, config_error_text(cfg));
		fprintf(stderr, "Using default lattice configuration parameters.\n");
	}

	getIntegerProperty(cfg, "numLatticePointsX", &numLatticePointsX, 128);
	//	fprintf(stderr,"numLatticePointsX = %d\n",numLatticePointsX);
	getIntegerProperty(cfg, "numLatticePointsY", &numLatticePointsY, 128);
	getIntegerProperty(cfg, "numLatticePointsRapidity", &numLatticePointsRapidity, 64);
	getIntegerProperty(cfg, "numProperTimePoints", &numProperTimePoints, 10);

	getDoubleProperty(cfg, "latticeSpacingX", &latticeSpacingX, 0.08);
	getDoubleProperty(cfg, "latticeSpacingY", &latticeSpacingY, 0.08);
	getDoubleProperty(cfg, "latticeSpacingRapidity", &latticeSpacingRapidity, 0.3);
	getDoubleProperty(cfg, "latticeSpacingProperTime", &latticeSpacingProperTime, 0.01);

	struct LatticeParameters * lattice = (struct LatticeParameters *) params;
	lattice->numLatticePointsX = numLatticePointsX;
	lattice->numLatticePointsY = numLatticePointsY;
	lattice->numLatticePointsRapidity = numLatticePointsRapidity;
	lattice->numComputationalLatticePointsX = numLatticePointsX+N_GHOST_CELLS;
	lattice->numComputationalLatticePointsY = numLatticePointsY+N_GHOST_CELLS;
	lattice->numComputationalLatticePointsRapidity = numLatticePointsRapidity+N_GHOST_CELLS;
	lattice->numProperTimePoints = numProperTimePoints;
	lattice->latticeSpacingX = latticeSpacingX;
	lattice->latticeSpacingY = latticeSpacingY;
	lattice->latticeSpacingRapidity = latticeSpacingRapidity;
	lattice->latticeSpacingProperTime = latticeSpacingProperTime;
}
*/

//does not require libconfig
void readLatticeParameters(const char* configDirectory, void * params) {
	// Read the file
	char fname[255];
	sprintf(fname, "%s/%s", configDirectory, "lattice.properties");
	// std::ifstream is RAII, i.e. no need to call close
	std::ifstream cFile (fname);
	if (cFile.is_open())
	{
		std::string line;

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		auto delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		numLatticePointsX = atoi(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		numLatticePointsY = atoi(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		numLatticePointsRapidity = atoi(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		numProperTimePoints = atoi(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		latticeSpacingX = atof(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		latticeSpacingY = atof(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		latticeSpacingRapidity = atof(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		latticeSpacingProperTime = atof(line.c_str());

		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		coarseProperTimeFOFactor = atoi(line.c_str());

		//for flux limiter theta
		/*
		getline(cFile, line);
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		delimiterPos = line.find("=");
		line = line.substr(delimiterPos + 1);
		fluxLimiterTheta = atof(line.c_str());
		*/
	}
	else std::cerr << "No configuration file found for lattice parameters\n";

	struct LatticeParameters * lattice = (struct LatticeParameters *) params;
	lattice->numLatticePointsX = numLatticePointsX;
	lattice->numLatticePointsY = numLatticePointsY;
	lattice->numLatticePointsRapidity = numLatticePointsRapidity;
	lattice->numComputationalLatticePointsX = numLatticePointsX+N_GHOST_CELLS;
	lattice->numComputationalLatticePointsY = numLatticePointsY+N_GHOST_CELLS;
	lattice->numComputationalLatticePointsRapidity = numLatticePointsRapidity+N_GHOST_CELLS;
	lattice->numProperTimePoints = numProperTimePoints;
	lattice->latticeSpacingX = latticeSpacingX;
	lattice->latticeSpacingY = latticeSpacingY;
	lattice->latticeSpacingRapidity = latticeSpacingRapidity;
	lattice->latticeSpacingProperTime = latticeSpacingProperTime;
	lattice->coarseProperTimeFOFactor = coarseProperTimeFOFactor;
	lattice->fluxLimiterTheta = fluxLimiterTheta;
}
