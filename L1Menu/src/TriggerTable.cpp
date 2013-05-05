#include "l1menu/TriggerTable.h"

#include "l1menu/ITrigger.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<l1menu::TriggerTable::TriggerRegistryEntry> l1menu::TriggerTable::registeredTriggers;

bool l1menu::TriggerTable::TriggerDetails::operator==( const l1menu::TriggerTable::TriggerDetails& otherTriggerDetails ) const
{
	return name==otherTriggerDetails.name && version==otherTriggerDetails.version;
}

std::auto_ptr<l1menu::ITrigger> l1menu::TriggerTable::getTrigger( const std::string& name ) const
{
//	std::cout << "Looking for latest version of " << name << std::endl;

	std::auto_ptr<l1menu::ITrigger> returnValue;
	unsigned int highestVersionNumber=0; // The highest version of the trigger encountered in the list so far

	for( std::vector<TriggerRegistryEntry>::const_iterator iRegistryEntry=registeredTriggers.begin(); iRegistryEntry!=registeredTriggers.end(); ++iRegistryEntry )
	{
		if( iRegistryEntry->details.name==name && iRegistryEntry->details.version>=highestVersionNumber )
		{
			returnValue=(*iRegistryEntry->creationFunctionPointer)();
			highestVersionNumber=iRegistryEntry->details.version;
		}
	}

	return returnValue;
}

std::auto_ptr<l1menu::ITrigger> l1menu::TriggerTable::getTrigger( const std::string& name, unsigned int version ) const
{
	TriggerDetails requestedTriggerDetails{ name, version };

	// Delegate to the other overload
	return getTrigger( requestedTriggerDetails );
}

std::auto_ptr<l1menu::ITrigger> l1menu::TriggerTable::getTrigger( const TriggerDetails& details ) const
{
//	std::cout << "Looking for version " << details.version << " of " << details.name << std::endl;

	for( std::vector<TriggerRegistryEntry>::const_iterator iRegistryEntry=registeredTriggers.begin(); iRegistryEntry!=registeredTriggers.end(); ++iRegistryEntry )
	{
		if( iRegistryEntry->details==details )
		{
			return (*iRegistryEntry->creationFunctionPointer)();
		}
	}

	// If program flow has reached this point then there are no triggers registered that
	// match the criteria. Return an empty pointer.
	return std::auto_ptr<l1menu::ITrigger>();
}

std::auto_ptr<l1menu::ITrigger> l1menu::TriggerTable::copyTrigger( const l1menu::ITrigger& triggerToCopy ) const
{
	// First create a trigger with the matching name and version
	std::auto_ptr<l1menu::ITrigger> newTrigger=getTrigger( triggerToCopy.name(), triggerToCopy.version() );

	if( newTrigger.get()==NULL ) throw std::runtime_error( "Unable to copy trigger "+triggerToCopy.name() );

	//
	// Now copy all of the parameters over.
	//
	// Get the parameter names
	std::vector<std::string> parameterNames=triggerToCopy.parameterNames();
	// Then run through and copy the value of each one
	for( std::vector<std::string>::const_iterator iName=parameterNames.begin(); iName!=parameterNames.end(); ++iName )
	{
		newTrigger->parameter(*iName)=triggerToCopy.parameter(*iName);
	}

	return newTrigger;
}

std::vector<l1menu::TriggerTable::TriggerDetails> l1menu::TriggerTable::listTriggers() const
{
	std::vector<TriggerDetails> returnValue;

	// Copy the relevant parts from the registered triggers into the return value
	for( std::vector<TriggerRegistryEntry>::const_iterator iRegistryEntry=registeredTriggers.begin(); iRegistryEntry!=registeredTriggers.end(); ++iRegistryEntry )
	{
		returnValue.push_back( iRegistryEntry->details );
	}

	return returnValue;
}

void l1menu::TriggerTable::registerTrigger( const std::string name, unsigned int version, std::auto_ptr<l1menu::ITrigger> (*creationFunctionPointer)() )
{
	std::cout << "Registering trigger " << name << " with version " << version << std::endl;

	TriggerDetails newTriggerDetails{ name, version };

	// First make sure there is not a trigger with the same name and version already registered
	for( std::vector<TriggerRegistryEntry>::const_iterator iRegistryEntry=registeredTriggers.begin(); iRegistryEntry!=registeredTriggers.end(); ++iRegistryEntry )
	{
		if( iRegistryEntry->details==newTriggerDetails )
		{
			std::stringstream errorMessage;
			errorMessage << "A trigger called \"" << newTriggerDetails.name << "\" with version " << newTriggerDetails.version << " has already been registered in the trigger table.";
			throw std::logic_error( errorMessage.str() );
		}
	}

	// If program flow has reached this point then there are no triggers with the same name
	// and version already registered, so it's okay to add the trigger as requested.
	registeredTriggers.push_back( TriggerRegistryEntry{newTriggerDetails,creationFunctionPointer} );
}
