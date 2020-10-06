
#ifndef _LCANALYSISTOOLS_PDGHELPER_H
#define _LCANALYSISTOOLS_PDGHELPER_H

// -- std headers
#include <array>
#include <optional>

// -- LCAnalysisTools headers
#include <LCAnalysisTools/Utilities.h>

namespace lc_analysis {
  
  namespace pdg {
    
    
    
    static constexpr std::size_t MaxDigits = 10 ;    
    using Digits = std::array<int8_t, MaxDigits> ;
    class PDGHelper ;
    
    ///  @brief  PDGDigit enum
    /// Represent the nth digit of a pdg code
    enum class Digit {
      Nj = 1,
      Nq3 = 2,
      Nq2 = 3,
      Nq1 = 4,
      N1 = 5,
      Nr = 6,
      N = 7,
      N8 = 8,
      N9 = 9,
      N10 = 10
    };
    
    //----------------------------------------------------------------------------
    
    /// A simple enum for quarks
    enum Quark {
      u,    ///< u quark flavor
      d,    ///< d quark flavor
      s,    ///< s quark flavor
      c,    ///< c quark flavor
      b,    ///< b quark flavor
      t
    };
    
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    
    /**
     *  @brief  ParticleData class.
     *
     *  Handles simple data for a single 
     *  particle (elementary or composite).
     *  Some of the fields may be applicable or not.
     *  Property getters might throw an std::bad_optional_access
     *  exception if the field is not meaningful for the particle
     *  or has not yet been set in the PDG table.
     */
    class ParticleData {
      friend class PDGHelper ;
      
      /// The particle data structure
      struct Data {
        int                        _pdgid {0} ;
        std::optional<float>       _mass {0.} ;
        std::optional<float>       _massUpper {0.} ;
        std::optional<float>       _massLower {0.} ;
        std::optional<float>       _width {0.} ;
        std::optional<float>       _widthUpper {0.} ;
        std::optional<float>       _widthLower {0.} ;
        std::optional<float>       _isospin {0} ;
        std::optional<int>         _gParity {0} ;
        float                      _charge {0.} ;
        Digits                     _digits {} ;
        std::string                _name {} ;
      };
      
    public:
      /// Constructor with particle data
      ParticleData( const Data &d ) : _data(d) {} 
      
      /// Get the particle PDG id
      inline int pdg() const ;
      
      /// Get the particle mass (if applicable)
      inline float mass() const ;
      
      /// Get the mass upper error (if applicable)
      inline float massUpper() const ;
      
      /// Get the mass lower error (if applicable)
      inline float massLower() const ;
      
      /// Get the particle mass width (if applicable)
      inline float width() const ;
      
      /// Get the width upper error (if applicable)
      inline float widthUpper() const ;
      
      /// Get the width lower error (if applicable)
      inline float widthLower() const ;
      
      /// Get the isospin (if applicable)
      inline float isospin() const ;
      
      /// Get the G parity (if applicable)
      inline int gParity() const ;
      
      /// Get the charge
      inline float charge() const ;
      
      /// Get the particle name
      inline const std::string &name() const ;
      
      /// Get the particle PDG digit.
      /// Particle PDG are encoded on 10 digit
      /// each having a meaning.
      /// See https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf
      template <Digit d>
      inline auto digit() const ;
      
    private:
      /// The particle data
      Data             _data {} ;
    };
    
    /// The particle data table defined in PDGTable.cc
    extern const std::vector<ParticleData> pdgTable ;
    
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    
    /**
     *  @brief  DigitsHelper class
     *
     *  Simple helper for particle PDG digits  
     */
    class DigitsHelper {
    public:
      /// Convert digit id to index in the array
      template <Digit d>
      static std::size_t digitIndex() {
        return MaxDigits-static_cast<std::size_t>(d) ;
      }
      
      /// Whether a digit in the array is set (not -1)
      template <Digit d>
      static constexpr bool digitSet( const Digits &digits ) {
        return ( digits[digitIndex<d>()] >= 0 ) ;
      }
      
      /// Get the digit value. Returns -1 if not set
      template <Digit d>
      static constexpr auto digit( const Digits &digits ) {
        return digits[digitIndex<d>()] ;
      }
    };
    
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    
    /**
     *  @brief  PDGHelper class
     *
     *  Helper class providing utility functions based 
     *  on particle pdg and more.
     *  See https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf
     *  for more information on helper functions calculations. Most of the 
     *  caculations rely on the PDG id digits, thus on rules edicted in the 
     *  document above. Use this class together with the table provided in 
     *  this document.
     */
    class PDGHelper {
    public:
      /// Get the particle with the given pdg id
      static const ParticleData &particle( int pdg ) ;
      
      /// Whether the pdg is from a quark ( 1 -> 6 )
      static bool isQuark( const ParticleData &p ) ;
      
      /// Whether the pdg is from a lepton particle
      static bool isLepton( const ParticleData &p ) ;
      
      /// Whether the pdg is from a lepton particle
      static bool isHadron( const ParticleData &p ) ;
      
      /// Whether the pdg is from a meson
      static bool isMeson( const ParticleData &p ) ;
      
      /// Whether the pdg is from a baryon
      static bool isBaryon( const ParticleData &p ) ;
      
      /// Whether the pdg is from a di-quark particle
      static bool isDiQuark( const ParticleData &p ) ;
      
      /// Whether this particle is nucleus
      /// Ion numbers are +/- 10LZZZAAAI.
      /// AAA is A - total baryon number
      /// ZZZ is Z - total charge
      /// L is the total number of strange quarks.
      /// I is the isomer number, with I=0 corresponding to the ground state.
      static bool isNucleus( const ParticleData &p ) ;
      
      /// Whether the pdg is from a penta quark
      /// Pentaquark IDs are of the form +/- 9 Nr Nl Nq1 Nq2 Nq3 Nj, where Nj = 2J + 1 gives the spin
      /// and Nr Nl Nq1 Nq2 Nq3 denote the quark numbers in order Nr >= Nl >= Nq1 >= Nq2
      /// and Nq3 gives the antiquark number.
      static bool isPentaQuark( const ParticleData &p ) ;
      
      /// Whether this pdg is from a gauge boson or a Higgs
      /// Codes 21-30 are reserved for the Standard Model gauge bosons and the Higgs.
      /// The graviton and the boson content of a two-Higgs-doublet scenario
      /// and of additional SU(2)xU(1) groups are found in the range 31-40.
      static bool isGaugeBosonOrHiggs( const ParticleData &p ) ;
      
      /// Whether this pdg is from a standard model gauge boson or Higgs
      /// Codes 21-30 are reserved for the Standard Model gauge bosons and the Higgs,
      /// but only the codes 21-25 actually correspond to SM particles.
      static bool isSMGaugeBosonOrHiggs( const ParticleData &p ) ;
      
      /// Whether this pdg is generator specific
      /// Codes 81-100 are reserved for generator-specific pseudoparticles and concepts.
      /// Codes 901-930, 1901-1930, 2901-2930, and 3901-3930 are for
      /// additional components of Standard Model parton distribution functions,
      /// where the latter three ranges are intended to distinguish
      /// left/right/longitudinal components.
      /// Codes 998 and 999 are reserved for GEANT tracking purposes.
      static bool isGeneratorSpecific( const ParticleData &p ) ;
      
      /// Whether the pdg is from a special particle
      /// Special particle in the sense of the classification in the PDG MC particle numbering scheme document,
      /// hence the graviton, the DM (S = 0, 1/2, 1) particles, the reggeons (reggeon, pomeron and odderon),
      /// and all generator-specific pseudo-particles and concepts, see `is_generator_specific`.
      static bool isSpecialParticle( const ParticleData &p ) ;
      
      /// Whether the pdg is from a lepton particle
      /// An R-hadron is of the form 10abcdj, 100abcj, or 1000abj,
      /// where j = 2J + 1 gives the spin; b, c, and d are quarks or gluons;
      /// and a (the digit following the zero's) is a SUSY particle.
      static bool isRHadron( const ParticleData &p ) ;
      
      /// Whether the pdg is a Q ball or any exotic particle beyond the qqq scheme
      /// Ad-hoc numbering for such particles is +/- 100XXXY0, where XXX.Y is the charge.
      static bool isQBall( const ParticleData &p ) ;
      
      /// Whether the pdg is Dyon, a magnetic monopole
      /// Magnetic monopoles and Dyons are assumed to have one unit of Dirac monopole charge
      /// and a variable integer number xyz units of electric charge,
      /// where xyz stands for Nq1 Nq2 Nq3.
      /// Codes 411xyz0 are used when the magnetic and electrical charge sign agree and 412xyz0 when they disagree,
      /// with the overall sign of the particle set by the magnetic charge.
      /// For now, no spin information is provided.
      static bool isDyon( const ParticleData &p ) ;
      
      /// Whether the pdg is a SUSY particle
      /// Fundamental SUSY particles have N = 1 or 2.
      static bool isSUSY( const ParticleData &p ) ;
      
      /// Whether the pdg is a technicolor state
      /// Technicolor states have N = 3.
      static bool isTechnicolor( const ParticleData &p ) ;
      
      /// Whether the pdg is a excited composite quark or lepton
      /// Excited (composite) quarks and leptons have N = 4 and Nr = 0.
      static bool isCompositeQuarkOrLepton( const ParticleData &p ) ;
      
      /// Whether this particle contains a specific quark
      template <Quark q>
      static bool hasQuark( const ParticleData &p ) ;
            
      /// Whether this pdg is a fundamental particle and has an anti-particle.
      /// Based on the current list of defined particles/concepts
      static bool hasFundamentalAnti( const ParticleData &p ) ;
      
    private:
      // private helper methods
      static int extraBits( int pdgid ) ;
      static int fundamentalId( const ParticleData &p ) ;
      static std::optional<int> atomicA( const ParticleData &p ) ;
      static std::optional<int> atomicZ( const ParticleData &p ) ;
    };
    
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    
    inline int ParticleData::pdg() const { 
      return _data._pdgid ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::mass() const { 
      return _data._mass.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::massUpper() const { 
      return _data._massUpper.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::massLower() const { 
      return _data._massLower.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::width() const { 
      return _data._width.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::widthUpper() const { 
      return _data._widthUpper.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::widthLower() const { 
      return _data._widthLower.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::isospin() const { 
      return _data._isospin.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline int ParticleData::gParity() const { 
      return _data._gParity.value() ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline float ParticleData::charge() const { 
      return _data._charge ; 
    }
    
    //----------------------------------------------------------------------------
    
    inline const std::string &ParticleData::name() const { 
      return _data._name ; 
    }
    
    //----------------------------------------------------------------------------
    
    template <Digit d>
    inline auto ParticleData::digit() const { 
      return DigitsHelper::digit<d>( _data._digits ) ; 
    }

    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------

    template <Quark q>
    inline bool PDGHelper::hasQuark( const ParticleData &p ) {
      if ( isNucleus( p ) ) {
        if( Quark::u == q || Quark::d == q ) {
          return true ;
        }
        else if ( Quark::s == q && contains( {2112, 2212}, p.pdg() ) ) {
          return ( p.digit<Digit::N8>() > 0 ) ; 
        }
      }
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( fundamentalId( p ) > 0 ) {
        return false ;
      }
      if ( isDyon( p ) ) {
        return false ;
      }
      const auto qint = static_cast<int>( q ) ;
      if ( isRHadron( p ) ) {        
        auto iz = 7 ;
        for ( auto loc : {6, 5, 4, 3, 2, 1} ) {
          if ( p._data._digits[loc] == 0 ) {
            iz = loc ;
          }
          else if ( loc == (iz -1) ) {
            continue ;
          }
          else if ( p._data._digits[loc] == qint ) {
            return true ;
          }
        }
        return false ;
      }
      if ( p.digit<Digit::Nq3>() == qint 
        || p.digit<Digit::Nq2>() == qint 
        || p.digit<Digit::Nq1>() == qint ) {
        return true ;
      }
      if ( isPentaQuark( p ) ) {
        if ( p.digit<Digit::N1>() == qint || p.digit<Digit::Nr>() == qint ) {
          return true ;
        }
      }
      return false ;
    }
    
  }
}

#endif
