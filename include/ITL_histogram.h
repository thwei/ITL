/**
 * Histogram class.
 * The histogram assigns a field value to a bin of the histogram.
 * Created on: Nov 17, 2010.
 * @author Teng-Yok
 * @author Abon
 * @see ITL_histogramconstants
 */

#ifndef ITL_HISTOGRAM_H_
#define ITL_HISTOGRAM_H_

#include "ITL_header.h"
#include "ITL_vectormatrix.h"
// ADD-BY-Tzu-Hsuan-BEGIN-02/13
#include "ITL_util.h"
#include "ITL_field_regular.h"
// ADD-BY-Tzu-Hsuan-END-02/13

class ITL_histogram
{
public:
	// ADD-BY-LEETEN 10/07/2011-BEGIN
	enum {
		DEFAULT_NR_OF_BINS = 360
	};
	// ADD-BY-LEETEN 10/07/2011-END

	float* theta[5];					/**< Angle variable. Angle related to spherical coordinates. */
	float* phi[5];						/**< Angle variable. Angle related to spherical coordinates. */
	float* thetaCenter[5];				/**< Angle variable. Angle related to spherical coordinates. */
	float* phiCenter[5];				/**< Angle variable. Angle related to spherical coordinates. */
	VECTOR3* binCenter[5];


	bool bIsAngleMapInitialized[5];	/**< Boolean flag. Flag is set if the angle maps have been initialized. */
	bool bIsPatchRead;				/**< Boolean flag. Flag is set if the patch file has been read. */
	
	int nResolution;				/**< Number of histogram resolutions available, maximum 5 available */
	int nBin[5];					/**< Histogram parameter. Numer of bins. */
	int iNrOfThetas[5];				/**< Histogram parameter. Number of thetas.  */
	int iNrOfPhis[5];				/**< Histogram parameter. Number of phis.  */
	float fNrOfThetas[5];			/**< Histogram parameter. Number of phis.  */
	float fNrOfPhis[5];				/**< Histogram parameter. Number of phis.  */
	int* piAngleMap[5];			/**< Histogram parameter. Mapping from vector to a region in sperical coordinates.  */

	// ADD-BY-Tzu-Hsuan-BEGIN-02/13
	float histogramLow;
	float histogramHigh;
	ITL_field_regular<int>* binDatas;
	SCALAR histMin;
	SCALAR histMax;
	bool hist_RangeSet;
	// ADD-BY-Tzu-Hsuan-END-02/13

	// ADD-BY-Tzu-Hsuan-BEGIN-10/2013
	int Doanes(ITL_field_regular<SCALAR> *scalarField);
	int Scotts_Normal_Reference_Rule(ITL_field_regular<SCALAR> *scalarField);
	int FreedmanDiaconisRule(ITL_field_regular<SCALAR> *scalarField);
	int ShimazakiBinSelection(ITL_field_regular<SCALAR> *scalarField);
	int QueryBinNum(ITL_field_regular<SCALAR> *scalarField);
	int QueryBinNum(ITL_field_regular<SCALAR> *scalarField, double v_low, double v_high);
	double QueryBinWidth(ITL_field_regular<SCALAR> *scalarField);
	double QueryBinWidth(ITL_field_regular<SCALAR> *scalarField, double v_low, double v_high);
	void CreateRandomSample(ITL_field_regular<SCALAR> *scalarField, float *sample_data, int &number_sample, double percentage);
	// ADD-BY-Tzu-Hsuan-END-10/2013



public:

	/**
	 * Constructor.
	 * @param patchFileName Path of patch file on disc.
	 * @param nBins Desired number of bins in the histogram.
	 */
	// MOD-BY-LEETEN 10/07/2011-FROM:
		// ITL_histogram( const char* patchFileName, int nBins );
	// TO:
	ITL_histogram( const char* patchFileName, int nbin = DEFAULT_NR_OF_BINS );
	// MOD-BY-LEETEN 10/07/2011-END
	
	/**
	 * Constructor.
	 * @param patchfilename Path of patch file on disc.
	 * @param nbin Desired number of bins in the histogram.
	 * @param nresoltion Number of resolutions of the historgam.
	 */	
	ITL_histogram( const char** patchfilename, int *nbin, int nresolution );
	
	/**
	 * Histogram initialization function.
	 * @param patchFileName Path of patch file on disc.
	 * @param nBins Desired number of bins in the histogram.
	 */
	// MOD-BY-LEETEN 10/07/2011-FROM:
		// static void ITL_init_histogram( const char* patchFileName, int nBins );
	// TO:
	void ITL_init_histogram( const char* patchFileName, int iRes = 0 );
	// MOD-BY-LEETEN 10/07/2011-END

	/**
	 * Patch header reader. Reads patch information from the in-built header.
	 * 
	 */
	void readPatches_header();
	/**
	 * Patch file reader.
	 * @param patchFileName Path of patch file on disc.
	 *
	 */
	void readPatches_region( const char* patchFileName, int iRes = 0 );
	/**
	 * Actual angle map computation doen here.
	 * @param nBins Desired number of bins in the histogram.
	 */
	void ITL_compute_table();
	/**
	 * Actual angle map computation doen here.
	 * @param nBins Desired number of bins in the histogram.
	 */
	void ITL_compute_table( int iRes );

	/**
	 * Actual angle map computation done here.
	 * @param mytheta theta corresponding to the local vector.
	 * @param myphi phi corresponding to the local vector.
	 * @param binnum Desired number of bins in the histogram.
	 */
	int get_bin_by_angle(float mytheta, float myphi, int binnum, int iRes = 0 );

	/**
	 * Scala-to-bin conversion Routine.
	 * Function converts the scalar to bin id.
	 * @param v at some Cartesian co-ordinate of the field
	 * @param nBins Desired number of bins in the histogram.
	 */
	int get_bin_number_3D(SCALAR v, int nBins );
	
	/**
	 * Vector-to-bin conversion Routine.
	 * Function converts the vector from Cartesian coodinates to the patch index
	 * via the specified lookup table.
	 * @param v at some Cartesian co-ordinate of the field
	 * @param nBins Desired number of bins in the histogram.
	 */
	int get_bin_number_3D(VECTOR3 v, int iRes = 0 );

	/**
	 * 2D Vector-to-bin conversion Routine.
	 * Function converts the vector from Cartesian coodinates to the patch index
	 * via the specified lookup table.
	 * @param v at some Cartesian co-ordinate of the field
	 * @param nBins Desired number of bins in the histogram.
	 */
	int get_bin_number_2D(VECTOR3 v, int nbin );

	/**
	 * Theta computation.
	 * @param x x-component of vector
	 * @param y y-computation of vector
	 * @return Theta
	 */
	float getAngle(float x, float y);

	/**
	 * Phi computation.
	 * @param x x-component of vector
	 * @param y y-computation of vector
	 * @return Phi
	 */
	float getAngle2(float x, float y);

	/**
	 *
	 */
	VECTOR3 getBinCenter( int binId, int iRes = 0 );

	/**
	 *
	 */
	VECTOR3 getXYZ( float theta, float phi );

	// ADD-BY-Tzu-Hsuan-BEGIN-02/13
	int crossValidateSpeedUp( ITL_field_regular<SCALAR> *scalarField, char *fieldType, int nMax, int start, int step);
	void computeHistogramBinField_h( ITL_field_regular<SCALAR> *scalarField, char *fieldType, int nBin, char* binMapFile);
	void computeHistogramBinField_Scalar_h( ITL_field_regular<SCALAR> *scalarField, int nBin );
	void setHistogramRange_h( SCALAR minR, SCALAR maxR );
	void computeFrequencies_h( int nPoint, int *binIds, double *freqArray );
	// ADD-BY-Tzu-Hsuan-END-02/13

};

#endif
/* ITL_HISTOGRAM_H_ */
