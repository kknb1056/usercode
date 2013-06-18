#ifndef l1menu_TriggerRates_h
#define l1menu_TriggerRates_h

#include <vector>

namespace l1menu
{
	struct TriggerRates
	{
	public:
		/** @brief The fraction of events that passed all triggers, 1 being all events and 0 no events. */
		float totalFraction() const { return totalFraction_; }
		void setTotalFraction( float totalFraction ) { totalFraction_=totalFraction; }

		/** @brief The fraction of events that passed a particular trigger. */
		std::vector<float>& fractions() { return fractions_; }
		const std::vector<float>& fractions() const { return fractions_; }
	private:
		float totalFraction_;
		std::vector<float> fractions_;
	};

} // end of namespace l1menu

#endif
