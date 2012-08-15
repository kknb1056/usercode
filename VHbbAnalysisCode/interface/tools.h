#ifndef trkupgradeanalysis_tools_h
#define trkupgradeanalysis_tools_h

#include <memory> // required for std::auto_ptr
#include <string>
#include <utility> // required for std::pair
#include <map>

// Forward declarations
class TFile;
class TH1F;
class TTree;
class TDirectory;

namespace trkupgradeanalysis
{
	namespace tools
	{

		/** @brief Class to automatically clean up a TFile when it goes out of scope.
		 *
		 * Since I'm using exceptions I need something to keep track of the open TFile
		 * that is exception safe. I can't use auto_ptr class directly because I want
		 * the "Close" method to be called before deletion.
		 *
		 * I'm not sure if inheriting from auto_ptr is a good idea. I'll give it a go
		 * and see what happens.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 27/Apr/2012
		 */
		class TFile_auto_ptr : public std::auto_ptr<TFile>
		{
		public:
			TFile_auto_ptr( const std::string& filename );
			~TFile_auto_ptr();
			bool operator==( const TFile* pOtherFile );
		};



		/** @brief Class to simplify getting entries from a TTree.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 07/Jul/2012
		 */
		class NTupleRow
		{
		public:
			NTupleRow( TTree* pTree );
			const double& getDouble( const std::string& name ) const;
			bool nextRow();
			void returnToStart();
		protected:
			std::map<std::string,double> branchAddresses_;
			TTree* pTree_;
			int maxCandidateNumber_;
			int candidateNumber_;
		};



		/** @brief Uses linear interpolation of two points to find and x value that corresponds to the supplied requiredY.
		 *
		 * @param[in] point1     The first point to interpolate between, with x as the first member and y as the second.
		 * @param[in] point2     The second point to interpolate between, with x as the first member and y as the second.
		 * @param[in] requiredY  The y value for the interpolated point you want to find the x value for.
		 * @return               The x value for the point with y=requiredY on the straight line between point1 and point2
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 30/Apr/2012
		 */
		float linearInterpolate( std::pair<float,float> point1, std::pair<float,float> point2, float requiredY );



		/** @brief Calculates the btag operating point that would give the requested tag efficiency.
		 *
		 * Finds the two histogram bins either side of the requested efficiency and then uses linear
		 * interpolation between the two to get the operating point.
		 *
		 * @param[in] pEventsVersusDiscriminatorHistogram  The histogram of number of events versus btag discriminator to calculate from.
		 * @param[in] requiredEfficiency                   The efficiency that you want an operating point for.
		 * @param[in] verbose                              (Defaults to true) Whether to print extra information to standard output.
		 * @return                      The discriminator value that would give an efficiency of requiredEfficiency for the supplied histogram.
		 * @throw  std::runtime_error   If something goes wrong.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 30/Apr/2012
		 */
		float findDiscriminator( const TH1F* pEventsVersusDiscriminatorHistogram, const float requiredEfficiency, const bool verbose=true );



		/** @brief Calculates the efficiency for the supplied btag operating point.
		 *
		 * Finds the two histogram bins either side of the requested operating point and then uses linear
		 * interpolation between the two to get the efficiency.
		 *
		 * @param[in] pEventsVersusDiscriminatorHistogram  The histogram of number of events versus btag discriminator to calculate from.
		 * @param[in] requiredDiscriminator                The operating point that you want to know the efficiency for.
		 * @param[in] verbose                              (Defaults to true) Whether to print extra information to standard output.
		 * @return                      The efficiency that the supplied discriminator would give for the supplied histogram.
		 * @throw  std::runtime_error   If something goes wrong.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 30/Apr/2012
		 */
		float findEfficiency( const TH1F* pEventsVersusDiscriminatorHistogram, const float requiredDiscriminator, const bool verbose=true );


		/** @brief Creates a directory structure in the root TDirectory, with parent directories separated by slashes also created.
		 *
		 * @param[in] fullPath          The directory name to be created. Slashes will create a series of directories inside each other.
		 * @param[in] pParent           The already existing directory to create the new directories in.
		 * @return                      The TDirectory created.
		 * @throw  std::runtime_error   If the pParent is NULL or one of the directories couldn't be created.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 08/Jul/2012
		 */
		TDirectory* createDirectory( const std::string& fullPath, TDirectory* pParent );

	} // end of namespace tools

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_tools_h"
