#ifndef _LCANALYSISTOOLS_PDGTABLE_H
#define _LCANALYSISTOOLS_PDGTABLE_H

#include <vector>
#include <unordered_map>
#include <sstream>

#include <LCAnalysisTools/PDGHelper.h>

namespace lc_analysis {
  namespace pdg {
    
    class PDGTableReader ;

    class PDGTable {
      friend class PDGTableReader ;
      
    public:
      PDGTable( const PDGTable & ) = delete ;
      PDGTable &operator=( const PDGTable & ) = delete ;
      
      static PDGTable &instance() { 
        static PDGTable pdgTable ;
        return pdgTable ; 
      }
      
      const ParticleData &particle( int pdg ) const {
        auto iter = std::find_if( _particleMap.begin(), _particleMap.end(), [&]( auto part ) { return part.second.pdg() == pdg ; } ) ;
        if( _particleMap.end() == iter ) {
          std::stringstream ss ; ss << "Particle with pdg id " << pdg << " not found" << std::endl ;
          throw std::runtime_error( ss.str() ) ;
        }
        return iter->second ;
      }
      
    private:
      PDGTable() = default ;
      ~PDGTable() = default ;
      
    private:
      std::unordered_map<int, ParticleData>    _particleMap {} ;
    };
    
  }
}

#endif