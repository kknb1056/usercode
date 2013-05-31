#include "l1menu/TriggerMenu.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include "l1menu/ITrigger.h"

namespace // Use the unnamed namespace for things only used in this file
{
	/// ASCII codes of characters that are considered whitespace (space, tab, carriage return, line feed).
    const char* whitespace="\x20\x09\x0D\x0A";

    /** @brief Converts the entire string to a float or throws an exception. */
    float convertStringToFloat( const std::string& string )
    {
    	float returnValue;
        std::stringstream stringConverter( string );
        stringConverter >> returnValue;
        if( stringConverter.fail() || !stringConverter.eof() ) throw std::runtime_error( "Unable to convert \""+string+"\" to a float" );
        return returnValue;
    }

    /** @brief Splits a string into individual parts delimited by whitespace. */
    std::vector<std::string> splitByWhitespace( const std::string& stringToSplit )
    {
    	std::vector<std::string> returnValue;

        size_t currentPosition=0;
        size_t nextDelimeter=0;
        do
        {
            // Skip over any leading whitespace
            size_t nextElementStart=stringToSplit.find_first_not_of( whitespace, currentPosition );
            if( nextElementStart!=std::string::npos ) currentPosition=nextElementStart;

            // Find the next whitespace and subtract everything up to that point
            nextDelimeter=stringToSplit.find_first_of( whitespace, currentPosition );
            std::string element=stringToSplit.substr( currentPosition, nextDelimeter-currentPosition );
            returnValue.push_back(element);

            // skip over any trailing whitespace
            nextElementStart=stringToSplit.find_first_not_of( whitespace, nextDelimeter );
            if( nextElementStart!=std::string::npos ) currentPosition=nextElementStart;
            else nextDelimeter=std::string::npos;

        } while( nextDelimeter!=std::string::npos );

        return returnValue;
    }

} // end of the unnamed namespace


l1menu::TriggerMenu::TriggerMenu() : triggerTable_( l1menu::TriggerTable::instance() )
{
	// No operation besides the initialiser list
}

l1menu::TriggerMenu::~TriggerMenu()
{
	// No operation since I switched from raw pointers to unique_ptr.
}

l1menu::TriggerMenu::TriggerMenu( const TriggerMenu& otherTriggerMenu )
	: triggerTable_(otherTriggerMenu.triggerTable_)
{
	for( std::vector< std::unique_ptr<l1menu::ITrigger> >::const_iterator iTrigger=otherTriggerMenu.triggers_.begin(); iTrigger!=otherTriggerMenu.triggers_.end(); ++iTrigger )
	{
		l1menu::ITrigger& sourceTrigger=**iTrigger;

		triggers_.push_back( std::move(triggerTable_.copyTrigger(sourceTrigger)) );
	}

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );
}

l1menu::TriggerMenu::TriggerMenu( TriggerMenu&& otherTriggerMenu ) noexcept
	: triggerTable_(otherTriggerMenu.triggerTable_), triggers_( std::move(otherTriggerMenu.triggers_) ),
	  triggerResults_( std::move(otherTriggerMenu.triggerResults_) )
{
	// No operation besides the initialiser list
}

l1menu::TriggerMenu& l1menu::TriggerMenu::operator=( const l1menu::TriggerMenu& otherTriggerMenu )
{
	//
	// First clean up whatever triggers I had before
	//
	triggers_.clear();

	//
	// Now copy the triggers from the other menu
	//
	for( std::vector< std::unique_ptr<l1menu::ITrigger> >::const_iterator iTrigger=otherTriggerMenu.triggers_.begin(); iTrigger!=otherTriggerMenu.triggers_.end(); ++iTrigger )
	{
		l1menu::ITrigger& sourceTrigger=**iTrigger;

		triggers_.push_back( std::move(triggerTable_.copyTrigger(sourceTrigger)) );
	}

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );

	return *this;
}

l1menu::TriggerMenu& l1menu::TriggerMenu::operator=( l1menu::TriggerMenu&& otherTriggerMenu ) noexcept
{
	triggers_=std::move( otherTriggerMenu.triggers_ );
	triggerResults_=std::move(otherTriggerMenu.triggerResults_);

	return *this;
}

bool l1menu::TriggerMenu::addTrigger( const std::string& triggerName )
{
	std::unique_ptr<l1menu::ITrigger> pNewTrigger=triggerTable_.getTrigger( triggerName );
	if( pNewTrigger.get()==NULL ) return false;

	triggers_.push_back( std::move(pNewTrigger) );

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );
	return true;
}

bool l1menu::TriggerMenu::addTrigger( const std::string& triggerName, unsigned int version )
{
	std::unique_ptr<l1menu::ITrigger> pNewTrigger=triggerTable_.getTrigger( triggerName, version );
	if( pNewTrigger.get()==NULL ) return false;

	triggers_.push_back( std::move(pNewTrigger) );

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );
	return true;
}

size_t l1menu::TriggerMenu::numberOfTriggers() const
{
	return triggers_.size();
}

l1menu::ITrigger& l1menu::TriggerMenu::getTrigger( size_t position )
{
	if( position>triggers_.size() ) throw std::range_error( "Trigger requested that does not exist in the menu" );

	return *triggers_[position];
}

const l1menu::ITrigger& l1menu::TriggerMenu::getTrigger( size_t position ) const
{
	if( position>triggers_.size() ) throw std::range_error( "Trigger requested that does not exist in the menu" );

	return *triggers_[position];
}

std::unique_ptr<l1menu::ITrigger> l1menu::TriggerMenu::getTriggerCopy( size_t position ) const
{
	if( position>triggers_.size() ) throw std::range_error( "Trigger requested that does not exist in the menu" );

	return triggerTable_.copyTrigger(*triggers_[position]);
}

bool l1menu::TriggerMenu::apply( const l1menu::IEvent& event ) const
{
	bool atLeastOneTriggerHasFired=false;

	for( size_t triggerNumber=0; triggerNumber<triggers_.size(); ++triggerNumber )
	{
		bool result=triggers_[triggerNumber]->apply(event);
//		triggerResults_[triggerNumber]=result;
		if( result ) atLeastOneTriggerHasFired=true;
	}

	return atLeastOneTriggerHasFired;
}

void l1menu::TriggerMenu::loadMenuFromFile( const std::string& filename )
{
	std::ifstream file( filename.c_str() );
	if( !file.is_open() ) throw std::runtime_error( "Unable to open file "+filename );

	loadMenuInOldFormat( file );
}

void l1menu::TriggerMenu::loadMenuInOldFormat( std::ifstream& file )
{
	const size_t bufferSize=200;
	char buffer[bufferSize];

	while( file.good() )
	{
		try
		{
			// Get one line at a time
			file.getline( buffer, bufferSize );

			// split the line by whitespace into columns
			std::vector<std::string> tableColumns=::splitByWhitespace( buffer );

			if( tableColumns.size()==1 && tableColumns[0].empty() ) continue; // Allow blank lines without giving a warning
			if( tableColumns.size()!=12 ) throw std::runtime_error( "The line does not have the correct number of columns" );

			float prescale=::convertStringToFloat( tableColumns[2] );
			if( prescale!=0 )
			{
				if( addTrigger( tableColumns[0] ) ) // Try and create a trigger with the name supplied
				{
					//std::cout << "Added trigger \"" << tableColumns[0] << "\"" << std::endl;

					l1menu::ITrigger& newTrigger=*(triggers_.back());

					// Try and set all of the relevant parameters. I know not all triggers have these parameters
					// so wrap in individual try/catch blocks.
					try{ newTrigger.parameter("threshold1")=::convertStringToFloat( tableColumns[3] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

					try{ newTrigger.parameter("threshold2")=::convertStringToFloat( tableColumns[4] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

					try{ newTrigger.parameter("threshold3")=::convertStringToFloat( tableColumns[5] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

					try{ newTrigger.parameter("threshold4")=::convertStringToFloat( tableColumns[6] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

					try{ newTrigger.parameter("etaCut")=::convertStringToFloat( tableColumns[7] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

					try{ newTrigger.parameter("muonQuality")=::convertStringToFloat( tableColumns[8] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

				} // end of "if able to add trigger"
				else std::cout << "The trigger \"" << tableColumns[0] << "\" is not registered in the trigger table" << std::endl;
			} // end of "if( prescale!=0 )"
		} // end of try block
		catch( std::runtime_error& exception )
		{
			std::cout << "Some error occured while processing the line \"" << buffer << "\":" << exception.what() << std::endl;
		}
	}
}
