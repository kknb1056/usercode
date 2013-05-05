#ifndef l1menu_RegisterTriggerMacro_h
#define l1menu_RegisterTriggerMacro_h

#include "l1menu/TriggerTable.h"

#define REGISTER_TRIGGER( NAME ) class NAME##Factory \
	{ \
	public: \
		NAME##Factory() \
		{ \
			NAME temporaryInstance; \
			TriggerTable::registerTrigger( temporaryInstance.name(), temporaryInstance.version(), (&this->createTrigger) ); \
		} \
		static std::auto_ptr<l1menu::ITrigger> createTrigger() \
		{ \
			return std::auto_ptr<l1menu::ITrigger>(new NAME); \
		} \
	}; \
	NAME##Factory NAME##FactoryOnlyInstance;

#endif
