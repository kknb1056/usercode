#ifndef SimDataFormats_TrackingParticle_h
#define SimDataFormats_TrackingParticle_h

#include <vector>
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"

//
// Forward declarations
//
class TrackingVertex;
class SimTrack;
class EncodedEventId;

class TrackingParticle
{
    friend std::ostream& operator<< (std::ostream& s, TrackingParticle const& tp);
public:
    typedef int Charge; ///< electric charge type
    typedef math::XYZTLorentzVectorD LorentzVector; ///< Lorentz vector
    typedef math::PtEtaPhiMLorentzVector PolarLorentzVector; ///< Lorentz vector
    typedef math::XYZPointD Point; ///< point in the space
    typedef math::XYZVectorD Vector; ///< point in the space

    /// reference to reco::GenParticle
    typedef reco::GenParticleRefVector::iterator   genp_iterator;
    typedef std::vector<SimTrack>::const_iterator  g4t_iterator;

    /// default constructor
    TrackingParticle();

    TrackingParticle( const SimTrack& simtrk, const TrackingVertexRef& simvtx );

    // destructor
    ~TrackingParticle();

    /// PDG id, signal source, crossing number
    int pdgId() const;
    EncodedEventId eventId() const;

    // Setters for G4 and reco::GenParticle
    void addGenParticle( const reco::GenParticleRef& ref);
    void addG4Track( const SimTrack& t);
    /// iterators
    genp_iterator genParticle_begin() const;
    genp_iterator genParticle_end() const;
    g4t_iterator g4Track_begin() const;
    g4t_iterator g4Track_end() const;
    void setParentVertex(const TrackingVertexRef& ref);
    void addDecayVertex(const TrackingVertexRef& ref);
    void clearParentVertex();
    void clearDecayVertices();
    // Getters for Embd and Sim Tracks
    const reco::GenParticleRefVector& genParticles() const;
    const std::vector<SimTrack>& g4Tracks() const;
    const TrackingVertexRef& parentVertex() const;

    // Accessors for vector of decay vertices
    const TrackingVertexRefVector& decayVertices() const;
    tv_iterator decayVertices_begin() const;
    tv_iterator decayVertices_end() const;


    int charge() const; ///< electric charge
    int threeCharge() const; ///< electric charge
    const LorentzVector& p4() const; ///< four-momentum Lorentz vector


    Vector momentum() const; ///< spatial momentum vector

    /// boost vector to boost a Lorentz vector
    /// to the particle center of mass system
    Vector boostToCM() const;

    double p() const; ///< magnitude of momentum vector
    double energy() const; ///< energy
    double et() const; ///< transverse energy
    double mass() const; ///< mass
    double massSqr() const; ///< mass squared
    double mt() const; ///< transverse mass
    double mtSqr() const; ///< transverse mass squared
    double px() const; ///< x coordinate of momentum vector
    double py() const; ///< y coordinate of momentum vector
    double pz() const; ///< z coordinate of momentum vector
    double pt() const; ///< transverse momentum
    double phi() const; ///< momentum azimuthal angle
    double theta() const; ///< momentum polar angle
    double eta() const; ///< momentum pseudorapidity
    double rapidity() const; ///< rapidity
    double y() const; ///< rapidity
    Point vertex() const; ///< vertex position
    double vx() const; ///< x coordinate of vertex position
    double vy() const; ///< y coordinate of vertex position
    double vz() const; ///< z coordinate of vertex position
    int status() const; ///< status word

    static const unsigned int longLivedTag; ///< long lived flag

    bool longLived() const; ///< is long lived?

   /** Gives the total number of hits, including muon hits. Hits on overlaps in the same layer count as two
    * hits. Equivalent to trackPSimHit().size() in the old TrackingParticle implementation. */
   int numberOfHits() const;

   /** The number of hits in the tracker. Hits on overlaps in the same layer count as two hits. Equivalent to
    * trackPSimHit(DetId::Tracker).size() in the old TrackingParticle implementation. */
   int numberOfTrackerHits() const;

   /** @deprecated The number of hits in the tracker but taking account of overlaps.
    * Deprecated in favour of the more aptly named numberOfTrackerLayers(). */
   int matchedHit() const;
   /** The number of tracker layers with a hit. Different from numberOfTrackerHits because
    * this method counts multiple hits on overlaps in the layer as one hit. */
   int numberOfTrackerLayers() const;

   void setNumberOfHits( int numberOfHits );
   void setNumberOfTrackerHits( int numberOfTrackerHits );
   void setNumberOfTrackerLayers( const int numberOfTrackerLayers );
private:
    int numberOfHits_; ///< I'll put this in until I'm certain matchedHits is the same as simHits.size()
    int numberOfTrackerHits_; ///< The number of tracker only hits
    int numberOfTrackerLayers_; ///< Equivalent to the old matchedHit

    /// references to G4 and reco::GenParticle tracks
    std::vector<SimTrack> g4Tracks_;
    reco::GenParticleRefVector genParticles_;

    // Source and decay vertices
    TrackingVertexRef parentVertex_;
    TrackingVertexRefVector decayVertices_;
};

#endif // SimDataFormats_TrackingParticle_H
