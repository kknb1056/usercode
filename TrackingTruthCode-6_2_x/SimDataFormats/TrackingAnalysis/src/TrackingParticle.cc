#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

const unsigned int TrackingParticle::longLivedTag = 65536;

int TrackingParticle::numberOfHits() const
{
    return numberOfHits_;
}

int TrackingParticle::numberOfTrackerHits() const
{
    return numberOfTrackerHits_;
}

void TrackingParticle::setNumberOfHits( int numberOfHits )
{
    numberOfHits_=numberOfHits;
}

void TrackingParticle::setNumberOfTrackerHits( int numberOfTrackerHits )
{
    numberOfTrackerHits_=numberOfTrackerHits;
}

std::ostream& operator<< (std::ostream& s, TrackingParticle const & tp)
{
    s << "TP momentum, q, ID, & Event #: "
    << tp.p4()                      << " " << tp.charge() << " "   << tp.pdgId() << " "
    << tp.eventId().bunchCrossing() << "." << tp.eventId().event() << std::endl;

    for (TrackingParticle::genp_iterator hepT = tp.genParticle_begin(); hepT !=  tp.genParticle_end(); ++hepT)
    {
        s << " HepMC Track Momentum " << (*hepT)->momentum().rho() << std::endl;
    }

    for (TrackingParticle::g4t_iterator g4T = tp.g4Track_begin(); g4T !=  tp.g4Track_end(); ++g4T)
    {
        s << " Geant Track Momentum  " << g4T->momentum() << std::endl;
        s << " Geant Track ID & type " << g4T->trackId() << " " << g4T->type() << std::endl;
        if (g4T->type() !=  tp.pdgId())
        {
            s << " Mismatch b/t TrackingParticle and Geant types" << std::endl;
        }
    }
    // Loop over decay vertices
    s << " TP Vertex " << tp.vertex() << std::endl;
    s << " Source vertex: " << tp.parentVertex()->position() << std::endl;
    s << " " << tp.decayVertices().size() << " Decay vertices" << std::endl;
    for (tv_iterator iTV = tp.decayVertices_begin(); iTV != tp.decayVertices_end(); ++iTV)
    {
        s << " Decay vertices:      " << (**iTV).position() << std::endl;
    }

    return s;
}
