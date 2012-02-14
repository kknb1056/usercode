#include "TrkUpgradeAnalysis/VHbb/interface/CutTypes.h"

#include <sstream>

trkupgradeanalysis::cuts::Equals::Equals( double cutValue )
	: cutValue_(cutValue)
{
}

bool trkupgradeanalysis::cuts::Equals::apply( double value ) const
{
	return value==cutValue_;
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::Equals::copy() const
{
	return std::auto_ptr<ICutType>( new Equals(*this) );
}

std::string trkupgradeanalysis::cuts::Equals::name() const
{
	std::stringstream nameStream;
	nameStream << "Equals" << cutValue_;
	return nameStream.str();
}









trkupgradeanalysis::cuts::LessThan::LessThan( double cutValue )
	: cutValue_(cutValue)
{
}

bool trkupgradeanalysis::cuts::LessThan::apply( double value ) const
{
	return value<cutValue_;
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::LessThan::copy() const
{
	return std::auto_ptr<ICutType>( new LessThan(*this) );
}

std::string trkupgradeanalysis::cuts::LessThan::name() const
{
	std::stringstream nameStream;
	nameStream << "LessThan" << cutValue_;
	return nameStream.str();
}









trkupgradeanalysis::cuts::LessThanOrEqual::LessThanOrEqual( double cutValue )
	: cutValue_(cutValue)
{
}

bool trkupgradeanalysis::cuts::LessThanOrEqual::apply( double value ) const
{
	return value<=cutValue_;
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::LessThanOrEqual::copy() const
{
	return std::auto_ptr<ICutType>( new LessThanOrEqual(*this) );
}

std::string trkupgradeanalysis::cuts::LessThanOrEqual::name() const
{
	std::stringstream nameStream;
	nameStream << "LessThanOrEqual" << cutValue_;
	return nameStream.str();
}









trkupgradeanalysis::cuts::GreaterThan::GreaterThan( double cutValue )
	: cutValue_(cutValue)
{
}

bool trkupgradeanalysis::cuts::GreaterThan::apply( double value ) const
{
	return value>cutValue_;
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::GreaterThan::copy() const
{
	return std::auto_ptr<ICutType>( new GreaterThan(*this) );
}

std::string trkupgradeanalysis::cuts::GreaterThan::name() const
{
	std::stringstream nameStream;
	nameStream << "GreaterThan" << cutValue_;
	return nameStream.str();
}









trkupgradeanalysis::cuts::GreaterThanOrEqual::GreaterThanOrEqual( double cutValue )
	: cutValue_(cutValue)
{
}

bool trkupgradeanalysis::cuts::GreaterThanOrEqual::apply( double value ) const
{
	return value>=cutValue_;
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::GreaterThanOrEqual::copy() const
{
	return std::auto_ptr<ICutType>( new GreaterThanOrEqual(*this) );
}

std::string trkupgradeanalysis::cuts::GreaterThanOrEqual::name() const
{
	std::stringstream nameStream;
	nameStream << "GreaterThanOrEqual" << cutValue_;
	return nameStream.str();
}









trkupgradeanalysis::cuts::Within::Within( double lowerCutValue, double upperCutValue )
	: lowerCutValue_(lowerCutValue), upperCutValue_(upperCutValue)
{
	// Make sure they're the correct way around
	if( lowerCutValue_>upperCutValue ) std::swap( lowerCutValue_, upperCutValue );
}

bool trkupgradeanalysis::cuts::Within::apply( double value ) const
{
	return ( lowerCutValue_<value && value<=upperCutValue_ );
}

std::auto_ptr<trkupgradeanalysis::cuts::ICutType> trkupgradeanalysis::cuts::Within::copy() const
{
	return std::auto_ptr<ICutType>( new Within(*this) );
}

std::string trkupgradeanalysis::cuts::Within::name() const
{
	std::stringstream nameStream;
	nameStream << "Within" << lowerCutValue_ << "And" << upperCutValue_;
	return nameStream.str();
}
