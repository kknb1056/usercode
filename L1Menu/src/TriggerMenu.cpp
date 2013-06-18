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

    /** @brief Converts a value in absolute eta to the calorimeter region. */
    float convertEtaCutToRegionCut( float etaCut )
    {
    	return 0;
    }

    /** @brief Converts a value in calorimeter region to absolute eta. */
    float convertRegionCutToEtaCut( float regionCut )
    {
    	return 0;
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

l1menu::ITrigger& l1menu::TriggerMenu::addTrigger( const std::string& triggerName )
{
	std::unique_ptr<l1menu::ITrigger> pNewTrigger=triggerTable_.getTrigger( triggerName );
	if( pNewTrigger.get()==NULL ) throw std::range_error( "Trigger requested that does not exist" );

	triggers_.push_back( std::move(pNewTrigger) );

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );
	return *triggers_.back();
}

l1menu::ITrigger& l1menu::TriggerMenu::addTrigger( const std::string& triggerName, unsigned int version )
{
	std::unique_ptr<l1menu::ITrigger> pNewTrigger=triggerTable_.getTrigger( triggerName, version );
	if( pNewTrigger.get()==NULL ) throw std::range_error( "Trigger requested that does not exist" );

	triggers_.push_back( std::move(pNewTrigger) );

	// Make sure triggerResults_ is always the same size as triggers_
	triggerResults_.resize( triggers_.size() );
	return *triggers_.back();
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
				std::string triggerName=tableColumns[0];

				try
				{
					//std::cout << "Added trigger \"" << tableColumns[0] << "\"" << std::endl;
					l1menu::ITrigger& newTrigger=addTrigger( triggerName ); // Try and create a trigger with the name supplied

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

					float etaOrRegionCut=::convertStringToFloat( tableColumns[7] );
					// For most triggers, I can just try and set both the etaCut and regionCut parameters
					// with this value. If it doesn't have either of the parameters just catch the exception
					// and nothing will happen. Some cross triggers however have both, and need to set them
					// both from this value which requires a conversion. Most cross triggers expect this
					// value to be the regionCut, except for L1_SingleMu_CJet which expects it as the etaCut.
					if( triggerName=="L1_SingleMu_CJet" )
					{
						newTrigger.parameter("etaCut")=etaOrRegionCut;
						newTrigger.parameter("regionCut")=convertEtaCutToRegionCut( etaOrRegionCut );
					}
					else if( triggerName=="L1_SingleIsoEG_CJet" )
					{
						newTrigger.parameter("etaCut")=etaOrRegionCut;
						newTrigger.parameter("regionCut")=convertEtaCutToRegionCut( etaOrRegionCut );
					}
					else
					{
						// Any remaining triggers should only have one of these parameters and won't
						// need conversion. I'll just try and set them both, not a problem if one fails.
						try{ newTrigger.parameter("etaCut")=etaOrRegionCut; }
						catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

						try{ newTrigger.parameter("regionCut")=etaOrRegionCut; }
						catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters
					}

					try{ newTrigger.parameter("muonQuality")=::convertStringToFloat( tableColumns[8] ); }
					catch( std::exception& error ) { } // Do nothing, just try and convert the other parameters

				} // end of "if able to add trigger"
				catch( std::exception& error )
				{
					std::cerr << "Unable to add trigger \"" << tableColumns[0] << "\" because: " << error.what() << std::endl;
				}
			} // end of "if( prescale!=0 )"
		} // end of try block
		catch( std::runtime_error& exception )
		{
			std::cout << "Some error occured while processing the line \"" << buffer << "\":" << exception.what() << std::endl;
		}
	}
}
