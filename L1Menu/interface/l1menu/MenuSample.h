#ifndef l1menu_MenuSample_h
#define l1menu_MenuSample_h

#include <string>
#include <memory>

// Forward declarations
namespace L1Analysis
{
	class L1AnalysisDataFormat;
}


namespace l1menu
{

	class MenuSample
	{
	public:
		MenuSample();
		virtual ~MenuSample();
		MenuSample( const MenuSample& otherMenuSample );
		MenuSample& operator=( const MenuSample& otherMenuSample );

		void loadFile( const std::string& filename );
		size_t numberOfEvents() const;
		const L1Analysis::L1AnalysisDataFormat& getEvent( size_t eventNumber ) const;
	private:
		class MenuSamplePrivateMembers* pImple_;
	}; // end of class MenuSample

} // end of namespace l1menu

#endif
