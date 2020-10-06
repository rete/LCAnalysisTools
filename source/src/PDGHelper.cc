
// -- LCAnalysisTools headers
#include <LCAnalysisTools/PDGHelper.h>
#include <LCAnalysisTools/PDGTable.h>

// -- std headers
#include <cmath>
#include <set>
#include <stdexcept>
#include <sstream>
#include <algorithm>

namespace lc_analysis {
  
  namespace pdg {
    
    const ParticleData &PDGHelper::particle( int pdg ) {
      auto iter = std::find_if( pdgTable.begin(), pdgTable.end(), [&]( auto part ) { return part._data._pdgid == pdg ; } ) ;
      if( pdgTable.end() == iter ) {
        std::stringstream ss ; ss << "Particle with pdg id " << pdg << " not found" << std::endl ;
        throw std::runtime_error( ss.str() ) ;
      }
      return *iter ;
    }
    
    //----------------------------------------------------------------------------
  
    bool PDGHelper::isQuark( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ; 
      return ( absPdg > 0 && absPdg < 7 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isLepton( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ; 
      return ( absPdg > 10 && absPdg < 19 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isHadron( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if( contains( { 1000000010, 1000010010 }, absPdg ) ) {
        return true ;
      }
      if( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if( isMeson( p ) ) {
        return true ;
      }
      if( isBaryon( p ) ) {
        return true ;
      }
      if( isPentaQuark( p ) ) {
        return true ;
      }
      if( isRHadron( p ) ) {
        return true ;
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isMeson( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( absPdg <= 100 ) {
        return false ;
      }
      const auto fid = fundamentalId( p ) ;
      if ( fid > 0 && fid <= 100 ) {
        return false ;
      }
      if ( contains( {130, 210, 310, 150, 350, 510, 530, 110, 990, 9990}, absPdg ) ) {
        return true ;
      }
      if ( DigitsHelper::digit<Digit::Nj>( p._data._digits ) > 0 
        && DigitsHelper::digit<Digit::Nq3>( p._data._digits ) > 0
        && DigitsHelper::digit<Digit::Nq2>( p._data._digits ) > 0
        && DigitsHelper::digit<Digit::Nq1>( p._data._digits ) == 0 ) {
        if ( DigitsHelper::digit<Digit::Nq3>( p._data._digits ) == DigitsHelper::digit<Digit::Nq2>( p._data._digits ) 
          && p._data._pdgid < 0 ) {
          return false ;
        }
        else {
          return true ;
        }
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isBaryon( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( absPdg <= 100 ) {
        return false ;
      }
      const auto fid = fundamentalId( p ) ;
      if ( fid > 0 && fid <= 100 ) {
        return false ;
      }
      if ( contains( {2110, 2210, 1000000010, 1000010010}, absPdg ) ) {
        return true ;
      }
      if ( DigitsHelper::digit<Digit::Nj>( p._data._digits ) > 0 
        && DigitsHelper::digit<Digit::Nq3>( p._data._digits ) > 0
        && DigitsHelper::digit<Digit::Nq2>( p._data._digits ) > 0
        && DigitsHelper::digit<Digit::Nq1>( p._data._digits ) > 0 ) {
        return true ;
      }
      if ( isRHadron( p ) || isPentaQuark( p ) ) {
        return false ;
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isDiQuark( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( absPdg <= 100 ) {
        return false ;
      }
      const auto fid = fundamentalId( p ) ;
      if ( fid > 0 && fid <= 100 ) {
        return false ;
      }
      if ( DigitsHelper::digit<Digit::Nj>( p._data._digits ) > 0 
        && DigitsHelper::digit<Digit::Nq3>( p._data._digits ) == 0
        && DigitsHelper::digit<Digit::Nq2>( p._data._digits ) > 0
        && DigitsHelper::digit<Digit::Nq1>( p._data._digits ) > 0 ) {
        return true ;
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isNucleus( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( contains( {2112, 2212}, absPdg ) ) {
        return true ;
      }
      if ( p.digit<Digit::N10>() == 1 && p.digit<Digit::N9>() == 0 ) {
        auto z = atomicZ( p ) ;
        auto a = atomicA( p ) ;
        if ( ! z.has_value() || ! a.has_value() ) {
          return false ;
        }
        else if ( a.value() >= abs( z.value() ) ) {
          return true ;
        }
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isPentaQuark( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( p.digit<Digit::N>() != 9 ) {
        return false ;
      }
      if ( p.digit<Digit::Nr>() == 9 || p.digit<Digit::Nr>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nj>() == 9 || p.digit<Digit::N1>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nq1>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nq3>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nj>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nq2>() > p.digit<Digit::Nq1>() ) {
        return false ;
      }
      if ( p.digit<Digit::Nq1>() > p.digit<Digit::N1>() ) {
        return false ;
      }
      if ( p.digit<Digit::N1>() > p.digit<Digit::Nr>() ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isGaugeBosonOrHiggs( const ParticleData &p ) {
      const auto absPdg = abs( p.pdg() ) ;
      return ( 21 <= absPdg && absPdg <= 40 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isSMGaugeBosonOrHiggs( const ParticleData &p ) {
      const auto absPdg = abs( p.pdg() ) ;
      if ( absPdg == 24 ) {
        return true ;
      }
      return ( 21 <= absPdg && absPdg <= 25 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isGeneratorSpecific( const ParticleData &p ) {
      const auto absPdg = abs( p.pdg() ) ;
      if ( 81 <= absPdg && absPdg <= 100 ) {
        return true ;
      }
      if ( 901 <= absPdg && absPdg <= 930 ) {
        return true ;
      }
      if ( 1901 <= absPdg && absPdg <= 1930 ) {
        return true ;
      }
      if ( 2901 <= absPdg && absPdg <= 2930 ) {
        return true ;
      }
      if ( 3901 <= absPdg && absPdg <= 3930 ) {
        return true ;
      }
      if ( contains( {998, 999, 20022, 480000000}, absPdg ) ) {
        return true ;
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isSpecialParticle( const ParticleData &p ) {
      return ( contains( {39, 41, 42, 51, 52, 53, 110, 990, 9990}, p.pdg() ) || isGeneratorSpecific( p ) ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isRHadron( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( p.digit<Digit::N>() != 1 ) {
        return false ;
      }
      if ( p.digit<Digit::Nr>() != 0 ) {
        return false ;
      }
      if ( isSUSY( p ) ) {
        return false ;
      }
      if ( p.digit<Digit::Nq2>() == 0 
        || p.digit<Digit::Nq3>() == 0
        || p.digit<Digit::Nj>() == 0 ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isQBall( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( extraBits( p._data._pdgid ) != 1 ) {
        return false ;
      }
      if ( p.digit<Digit::N>() != 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nr>() != 0 ) {
        return false ;
      }
      if ( ( floorDivision( absPdg, 10 ) % 10000 ) == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nj>() != 0 ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isDyon( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( p.digit<Digit::N>() != 4 ) {
        return false ;
      }
      if ( p.digit<Digit::Nr>() != 1 ) {
        return false ;
      }
      if ( contains( {1, 2}, static_cast<int>(p.digit<Digit::N1>()) ) ) {
        return false ;
      }
      if ( p.digit<Digit::Nq3>() == 0 ) {
        return false ;
      }
      if ( p.digit<Digit::Nj>() != 0 ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isSUSY( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( p.digit<Digit::N>() != 1 && p.digit<Digit::N>() != 2 ) {
        return false ;
      }
      if ( p.digit<Digit::Nr>() != 0 ) {
        return false ;
      }
      if ( fundamentalId( p ) == 0 ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isTechnicolor( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      return ( p.digit<Digit::N>() == 3 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::isCompositeQuarkOrLepton( const ParticleData &p ) {
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return false ;
      }
      if ( fundamentalId( p ) == 0 ) {
        return false ;
      }
      if ( ! ( p.digit<Digit::N>() != 4 && p.digit<Digit::Nr>() == 0 ) ) {
        return false ;
      }
      return true ;
    }
    
    //----------------------------------------------------------------------------
    
    bool PDGHelper::hasFundamentalAnti( const ParticleData &p ) {
      auto fid = fundamentalId( p ) ;
      if ( 81 <= fid && fid <= 100 ) {
        return contains( {82, 84, 85, 86, 87}, fid ) ;
      }
      auto cpConjugates = {21, 22, 23, 25, 32, 33, 35, 36, 39, 40, 43} ;
      std::set<int> unassigned ( { 9, 10, 19, 20, 26 } ) ;
      std::vector<int> v1 = range<int>( 26, 32 ) ; std::vector<int> v2 = range<int>( 45, 80 ) ;
      unassigned.insert( v1.begin(), v1.end() ) ;
      unassigned.insert( v2.begin(), v2.end() ) ;
      if ( (1 <= fid && fid <= 79) && ! contains( cpConjugates, fid ) ) {
        return ( ! contains( unassigned, fid )  ) ;
      }
      return false ;
    }
    
    //----------------------------------------------------------------------------
    
    int PDGHelper::extraBits( int pdgid ) {
      return floorDivision( pdgid, 10000000 ) ;
    }
    
    //----------------------------------------------------------------------------
    
    int PDGHelper::fundamentalId( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ; 
      if ( extraBits( p._data._pdgid ) > 0 ) {
        return 0 ;
      }
      if ( DigitsHelper::digit<Digit::Nq2>(p._data._digits) == 0 && DigitsHelper::digit<Digit::Nq1>(p._data._digits) == 0 ) {
        return absPdg % 10000 ;
      } 
      else if ( absPdg <= 100 ) {
        return absPdg ;
      }
      else {
        return 0 ;
      }
    }
    
    //----------------------------------------------------------------------------
    
    std::optional<int> PDGHelper::atomicA( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( contains( {2112, 2212}, absPdg ) ) {
        return 1 ;
      }
      if ( p.digit<Digit::N10>() != 1 || p.digit<Digit::N9>() != 0 ) {
        return std::nullopt ;
      }
      return floorDivision( absPdg, 10 ) % 1000 ; 
    }
    
    //----------------------------------------------------------------------------
    
    std::optional<int> PDGHelper::atomicZ( const ParticleData &p ) {
      const auto absPdg = abs( p._data._pdgid ) ;
      if ( absPdg == 2212 ) {
        return floorDivision( absPdg, 2212 ) ;
      }
      if ( absPdg == 2112 ) {
        return 0 ;
      }
      if ( p.digit<Digit::N10>() != 1 || p.digit<Digit::N9>() != 0 ) {
        return std::nullopt ;
      }
      return ( floorDivision( absPdg, 10000 ) % 1000 ) * ( floorDivision( p._data._pdgid, p._data._pdgid ) ) ;
    }
    
  }
  
}