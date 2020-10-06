

#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <set>
#include <initializer_list>

namespace lc_analysis {
  
  template <typename T>
  inline bool contains( const std::initializer_list<T> &l, const T &value ) {
    return ( std::find( l.begin(), l.end(), value ) != l.end() ) ;
  }
  
  template <typename T>
  inline bool contains( const std::vector<T> &l, const T &value ) {
    return ( std::find( l.begin(), l.end(), value ) != l.end() ) ;
  }
  
  template <typename T>
  inline bool contains( const std::set<T> &l, const T &value ) {
    return (l.find( value ) != l.end()) ; 
  }
  
  inline int floorDivision( int i, int div ) {
    return static_cast<int>( std::floor( i / div ) ) ;  
  }
  
  template < typename T = size_t >
  class range {
  public:

    ///////////////////////////////////////////////////////////
    // Constructor 
    ///////////////////////////////////////////////////////////
    //
    // INPUT:
    //      from - Starting number of the sequence.
    //      end - Generate numbers up to, but not including this number.
    //      step -  Difference between each number in the sequence.     
    //
    // REMARKS:
    //      Parameters must be all positive or all negative
    //
    range( const T from, const T end, const T step = 1 ) 
      : kFrom( from ), kEnd( end ), kStep( step ) {
      assert( kStep != 0 ) ;
      assert( ( kFrom >= 0 && kEnd > 0 && kStep > 0 ) || ( kFrom < 0 && kEnd < 0 && kStep < 0 ) ) ;
    }

    // Default from==0, step==1
    range( const T end ) 
      : kFrom( 0 ), kEnd( end ), kStep( 1 ) {
      assert( kEnd > 0 ) ;
    }

  public:

    class _range_iter {
      T fVal;
      const T kStep;
    public:
      _range_iter( const T v, const T step ) : fVal( v ), kStep( step ) {}
      operator T  () const            { return fVal ; }
      operator const T & ()           { return fVal ; }
      const T operator * () const     { return fVal ; }
      const _range_iter & operator ++ ()  { fVal += kStep ; return * this ; }

      bool operator == ( const _range_iter & ri ) const {
        return ! operator != ( ri ) ;
      }

      bool operator != ( const _range_iter & ri ) const {   
        // This is a tricky part - when working with iterators
        // it checks only once for != which must be a hit to stop;
        // However, this does not work if increasing kStart by N times kSteps skips over kEnd
        return fVal < 0 ? fVal > ri.fVal : fVal < ri.fVal ;  
      }                                               
    };                                                  

    const _range_iter begin()   { return _range_iter( kFrom, kStep ) ; }
    const _range_iter end()     { return _range_iter( kEnd, kStep ) ; }

  public:

    // Conversion to any vector< T >
    operator std::vector< T > ( void ) {
      std::vector< T > retRange ;
      for( T i = kFrom; i < kEnd; i += kStep )
        retRange.push_back( i ) ;
      return retRange ;    // use move semantics here
    }
      
  private:
    const T kFrom, kEnd, kStep ;
  };
  
}
