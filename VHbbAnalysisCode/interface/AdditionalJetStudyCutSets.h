#ifndef trkupgradeanalysis_additionaljetstudy_AdditionalJetStudyCutSets_h
#define trkupgradeanalysis_additionaljetstudy_AdditionalJetStudyCutSets_h

#include <string>
#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"

// Forward declarations
class VHbbCandidate;

namespace trkupgradeanalysis
{
	/** @namespace additionaljetstudy
	 *
	 * @brief Namespace to keep the code I'm using to check the number of additional jets variable separate.
	 *
	 * This namespace probably won't be used once I've validated this variable scales as expected for a given
	 * level of pileup. Has a load of cut sets that are used to see the distribution of the number of additional
	 * jets variable with different types of cuts applied to the collection.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 14/Feb/2012
	 */
	namespace additionaljetstudy
	{
		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * Sent on 09/Feb/2012 to the cms-ku-unl-hbbanalysis mailing list, with the subject line "Naj plots".
		 * This cut set is to compare against slide 2, where the cuts are just
		 *    ( candidateType==Zmumu || candidateType==Zee )
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide2 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide2();
			std::string name() const;
		};

		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide3Plot1 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide3Plot1();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide3Plot2 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide3Plot2();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide3Plot3 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide3Plot3();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide4Plot2 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide4Plot2();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide5Plot1 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide5Plot1();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide5Plot2 : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide5Plot2();
			std::string name() const;
		};


		/** @brief Cut set to check against some slides Rachel Wilken sent to me.
		 *
		 * More info in the documentation for WilkenSlide2.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class WilkenSlide5Plot2AssumingTypo : public VHbbCandidateCutSet
		{
		public:
			WilkenSlide5Plot2AssumingTypo();
			std::string name() const;
		};

	} // end of namespace additionaljetstudy

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_additionaljetstudy_AdditionalJetStudyCutSets_h"

