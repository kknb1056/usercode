#ifndef SimDataFormats_TrackingParticle_h
#define SimDataFormats_TrackingParticle_h

/** Concrete TrackingParticle.
 *  All track parameters are passed in the constructor and stored internally.
 */

#include <map>

#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "Rtypes.h"

#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "DataFormats/DetId/interface/DetId.h"

class TrackingVertex;

class TrackingParticle
{
    friend std::ostream& operator<< (std::ostream& s, TrackingParticle const& tp);

public:

    /// electric charge type
    typedef int Charge;
    /// Lorentz vector
    typedef math::XYZTLorentzVectorD LorentzVector;
    /// Lorentz vector
    typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;
    /// point in the space
    typedef math::XYZPointD Point;
    /// point in the space
    typedef math::XYZVectorD Vector;

    /// reference to reco::GenParticle
    typedef reco::GenParticleRefVector::iterator   genp_iterator;
    typedef std::vector<SimTrack>::const_iterator  g4t_iterator;

    /// default constructor
    TrackingParticle() {}

    /// constructor from SimTrack and SimVertex
    TrackingParticle(const SimTrack& simtrk, const TrackingVertexRef& simvtx) {
      addG4Track(simtrk);
      setParentVertex(simvtx);     
    }

    // destructor
    ~TrackingParticle() {}

    /// PDG id, signal source, crossing number
    int pdgId() const
    {
        if( genParticles_.empty() ) return 0;

        reco::GenParticleRefVector::const_iterator it = genParticle_begin();
        return (*it)->pdgId();
    }
    EncodedEventId eventId() const
    {
        return g4Tracks_.at(0).eventId();
    }

    // Setters for G4 and reco::GenParticle
    void addGenParticle( const reco::GenParticleRef& ref) {
        genParticles_.push_back(ref);
    }
    void addG4Track( const SimTrack& t) {
        g4Tracks_.push_back(t);
    }
    /// iterators
    genp_iterator genParticle_begin() const {
        return genParticles_.begin();
    }
    genp_iterator genParticle_end() const {
        return genParticles_.end();
    }
    g4t_iterator g4Track_begin() const {
        return g4Tracks_.begin();
    }
    g4t_iterator g4Track_end() const {
        return g4Tracks_.end();
    }
    void setParentVertex(const TrackingVertexRef& ref) {
        parentVertex_ = ref;
    }
    void addDecayVertex(const TrackingVertexRef& ref) {
        decayVertices_.push_back(ref);
    }
    void clearParentVertex() {
        parentVertex_ = TrackingVertexRef();
    }
    void clearDecayVertices() {
        decayVertices_.clear();
    }
    void setMatchedHit(const int& hitnumb) {
        matchedHit_ = hitnumb;
    }
    // Getters for Embd and Sim Tracks
    const reco::GenParticleRefVector& genParticles() const {
        return genParticles_;
    }
    const std::vector<SimTrack>& g4Tracks() const {
        return g4Tracks_;
    }
    const TrackingVertexRef& parentVertex() const {
        return parentVertex_;
    }

    // Accessors for vector of decay vertices
    const TrackingVertexRefVector& decayVertices() const {
        return decayVertices_;
    }
    tv_iterator decayVertices_begin() const {
        return decayVertices_.begin();
    }
    tv_iterator decayVertices_end() const  {
        return decayVertices_.end();
    }
    int matchedHit() const  {
        return matchedHit_;
    }
    /// electric charge
    int charge() const {
        return g4Tracks_.at(0).charge() / 3;
    }
    /// electric charge 
    int threeCharge() const {
        return g4Tracks_.at(0).charge();
    }
    /// four-momentum Lorentz vector
    const LorentzVector& p4() const  {
        return g4Tracks_.at(0).momentum();
    }
#if 0
    /// four-momentum Lorentz vector
    const PolarLorentzVector& polarP4() const
    {
    }
#endif
    /// spatial momentum vector
    Vector momentum() const {
      return p4().Vect();
    }
    /// boost vector to boost a Lorentz vector
    /// to the particle center of mass system
    Vector boostToCM() const {
        return p4().BoostToCM();
    }
    /// magnitude of momentum vector
    double p() const {
        return p4().P();
    }
    /// energy
    double energy() const {
        return p4().E();
    }
    /// transverse energy
    double et() const {
        return p4().Et();
    }
    /// mass
    double mass() const {
        return p4().M();
    }
    /// mass squared
    double massSqr() const {
        return pow(mass(), 2);
    }
    /// transverse mass
    double mt() const {
        return p4().Mt();
    }
    /// transverse mass squared
    double mtSqr() const {
        return p4().Mt2();
    }
    /// x coordinate of momentum vector
    double px() const {
        return p4().Px();
    }
    /// y coordinate of momentum vector
    double py() const {
        return p4().Py();
    }
    /// z coordinate of momentum vector
    double pz() const {
        return p4().Pz();
    }
    /// transverse momentum
    double pt() const {
        return p4().Pt();
    }
    /// momentum azimuthal angle
    double phi() const {
        return p4().Phi();
    }
    /// momentum polar angle
    double theta() const {
        return p4().Theta();
    }
    /// momentum pseudorapidity
    double eta() const {
        return p4().Eta();
    }
    /// rapidity
    double rapidity() const {
        return p4().Rapidity();
    }
    /// rapidity
    double y() const {
        return rapidity();
    }
    /// vertex position
    Point vertex() const {
        return Point(vx(), vy(), vz());
    }
    /// x coordinate of vertex position
    double vx() const {
        const TrackingVertex& r = (*parentVertex_); 
        return r.position().X();
    }
    /// y coordinate of vertex position
    double vy() const {
        const TrackingVertex& r = (*parentVertex_); 
        return r.position().Y();
    }
    /// z coordinate of vertex position
    double vz() const {
        const TrackingVertex& r = (*parentVertex_); 
        return r.position().Z();
    }
    /// status word
    int status() const {
        reco::GenParticleRefVector::const_iterator it = genParticle_begin();
        return (*it)->status();
    }
    /// long lived flag
    static const unsigned int longLivedTag;
    /// is long lived?
    bool longLived() const {
      return status() & longLivedTag;
    }

private:

    /// production time
    float t_;

    /// Total Number of Hits belonging to the TrackingParticle
    int matchedHit_;

    /// references to G4 and reco::GenParticle tracks
    std::vector<SimTrack> g4Tracks_;
    reco::GenParticleRefVector genParticles_;

    // Source and decay vertices
    TrackingVertexRef parentVertex_;
    TrackingVertexRefVector decayVertices_;
};

#endif // SimDataFormats_TrackingParticle_H
