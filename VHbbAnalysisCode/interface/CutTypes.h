#ifndef trkupgradeanalysis_cuts_CutTypes_h
#define trkupgradeanalysis_cuts_CutTypes_h

#include "TrkUpgradeAnalysis/VHbb/interface/ICutType.h"
#include <memory>
#include <string>

namespace trkupgradeanalysis
{
	namespace cuts
	{
		class Equals : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			Equals( double cutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double cutValue_;
		};

		class LessThan : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			LessThan( double cutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double cutValue_;
		};

		class LessThanOrEqual : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			LessThanOrEqual( double cutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double cutValue_;
		};

		class GreaterThan : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			GreaterThan( double cutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double cutValue_;
		};

		class GreaterThanOrEqual : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			GreaterThanOrEqual( double cutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double cutValue_;
		};

		/** @brief Only returns true if a variable is between the two values supplied in the constructor.
		 *
		 * Note that the lower cut is "greater than" but the upper is "less than or equal to", i.e. returns
		 * true if
		 * 		lowerCutValue < variable <= upperCutVariable
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 30/Jan/2012
		 */
		class Within : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			Within( double lowerCutValue, double upperCutValue );
			bool apply( double value ) const;
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		private:
			double lowerCutValue_;
			double upperCutValue_;
		};


		/** @brief Dummy cut so that variables relation to other cuts can be tracked without actually cutting on them.
		 *
		 * Always returns true whatever value is passed to it.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 14/Feb/2012
		 */
		class AlwaysPasses : public trkupgradeanalysis::cuts::ICutType
		{
		public:
			AlwaysPasses();
			bool apply( double value ) const; ///< @brief Always returns true
			std::auto_ptr<ICutType> copy() const;
			std::string name() const;
		};

	} // end of namespace cuts

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_cuts_CutTypes_h"
