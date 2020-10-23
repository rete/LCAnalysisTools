
#include <LCAnalysisTools/PDGTable.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lc_analysis {
  namespace pdg {

    class PDGTableReader {
    public:
      PDGTableReader() {
        readTable() ;
      }
      
      template <typename T>
      void convert( const std::string &str, T &value ) {
        std::stringstream ss(str) ;
        if ( ! (ss >> value) ) {
          throw ;
        }
      }
      
      template <typename T>
      void convert( const std::string &str, std::optional<T> &value ) {
        if ( "null" == str ) {
          value = std::nullopt ;
          return ;
        }
        std::stringstream ss(str) ;
        if ( ! (ss >> *value) ) {
          throw ;
        }
      }
      
      template <typename T>
      void setDigits( const std::string &str, Digits &digits ) {
        auto digitStr = str ;
        if ( digitStr.empty() ) {
          throw ;
        }
        if ( digitStr[0] == '-' ) {
          digitStr.erase( 0, 1 ) ;
        }
        if ( digitStr.size() > MaxDigits ) {
          throw ;
        }
        for ( auto i=0 ; i<MaxDigits ; i++ ) {
          if ( i < MaxDigits-digitStr.size() ) {
            digits[i] = -1 ;            
          }
          else {
            convert<Digits::value_type>( std::string(digitStr[i]), digits[i] ) ;
          }
        }
      }
      
      
      
      std::vector<std::string> tokenize(const std::string& source, const char* delimiters) {
        std::string value = source;
        std::vector<std::string> tokens;
        auto delimeterIndex = value.find_first_of(delimiters);
        while (delimeterIndex != std::string::npos)
        {
        	std::string token = value.substr(0, delimeterIndex);
        	tokens.push_back(token);
        	value = value.substr(delimeterIndex + 1);
        	delimeterIndex = value.find_first_of(delimiters);
        }
        return tokens;
      }
      
      void readTable() {
        const std::string fname = "/home/local/soft/LCAnalysisTools/source/python/pdg-table.txt" ;
        std::fstream f; f.open( fname, std::ios::in ) ;
        if ( ! f.good() ) {
          throw std::runtime_error( "Couldn't open PDG table '" + fname + "'" ) ;
        }
        std::string line ;
        while ( std::getline( f, line ) ) {
          if ( line.empty() ) {
            break ;
          }
          if ( '#' == line[0] ) {
            continue ;
          }
          auto tokens = tokenize( line, "," ) ;          
          if ( tokens.size() != 10 ) {
            std::cout << "N=" << tokens.size() << ", line: " << line << std::endl ;            
          }
          ParticleData::Data pdgdata ;
          convert( tokens[0], pdgdata._pdgid ) ;
        }
      }
    };
    
    static const PDGTableReader __pdgTableReader__ ;
    
  }
}