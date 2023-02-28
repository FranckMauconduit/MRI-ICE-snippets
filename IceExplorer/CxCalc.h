
#ifndef CXCALC_h
#define CXCALC_h 1

#include "MrServers\MrVista\include\Ice\IceDefs.h"

class CxCalc
{
    public:
    
        float re;
        float im;
    
        // ----------------------------
        // - constructor / destructor -
        // ----------------------------        
        CxCalc();
        CxCalc(float);
        CxCalc(float, float);
        
        ~CxCalc();
        
        float   real       ( void );
        float   imag       ( void );
        float   abs        ( void );
        float   abs_square ( void );
        CxCalc  conj       ( void );
        CxCalc  conjprod   (const CxCalc&);
        
        void    print( void );
        
        CxCalc operator + (const CxCalc&);
        CxCalc operator + (const double&);
        
        CxCalc operator * (const CxCalc&);
        CxCalc operator * (const double&);
        
        CxCalc& operator = (const CxCalc&);
        
        CxCalc& operator += (const CxCalc&);
        
        // template<typename T>
        // CxCalc operator = (const T&);
        
        // std::ostream&  operator<<( std::ostream&, const CxCalc &c) const;
        std::ostream&  operator<<( std::ostream& );
        
        
        // template<typename T>
        // CxCalc operator + (const float&);
        
        // CMPLX operator - (const CMPLX, T);
        // CMPLX operator * (const CMPLX, T);
        // CMPLX operator / (const CMPLX, T);
        
    private:
        
        
};

CxCalc::CxCalc()
    : re(0)
    , im(0)
{}

CxCalc::~CxCalc()
{}

CxCalc::CxCalc( float f1)
    : re(f1)
    , im(0)
{}

CxCalc::CxCalc( float f1, float f2)
    : re(f1)
    , im(f2)
{}

CxCalc CxCalc::operator+(const CxCalc &cx) {
    return CxCalc( re+cx.re, im+cx.im );
}

CxCalc CxCalc::operator+(const double &v) { return CxCalc( re+static_cast<float>(v), im ); }

CxCalc CxCalc::operator*(const CxCalc &cx) {
    return CxCalc( re*cx.re-im*cx.im, re*cx.im+im*cx.re );
}

CxCalc CxCalc::operator*(const double &d) {
    return CxCalc( d*re, d*im );
}

CxCalc& CxCalc::operator=(const CxCalc &other) 
{
    if (this != &other) {
        re = other.re;
        im = other.im;
    }
    return *this; 
}

CxCalc& CxCalc::operator+=(const CxCalc &other) 
{
    if (this != &other) {
        re += other.re;
        im += other.im;
    }
    return *this; 
}

// template<typename T>
// CxCalc CxCalc::operator=(const T &t) {
    // return CxCalc(t,0);
// }

std::ostream& CxCalc::operator<<(std::ostream& os)
{
    // os << "( " << re << ", " << im << " )";
    os << "hello";
    return os;
}

// template<typename T>
// CxCalc CxCalc::operator+(const T &value) {
    // static_assert(std::is_artithmetic<T>::value,"An arithmetic type is required");
    // return CxCalc( re+value, im );
// }

float CxCalc::real()
{return re;}

float CxCalc::imag()
{return im;}

float CxCalc::abs()
{return sqrtf( abs_square() );}

float CxCalc::abs_square()
{return ( re*re+im*im );}

// CxCalc CxCalc::prod(const CxCalc &c1, const CxCalc &c2)
// {
    // CxCalc c3;
    // c3.re = c1.re*c2.re-c1.im*c2.im;
    // c3.im = c1.re*c2.im+c1.im*c2.re;
    // return c3;
// }

// CxCalc CxCalc::prod(const CxCalc &c1, const float &fl)
// {
    // CxCalc c2;
    // c2.re=c1.re*fl;
    // c2.im=c1.im*fl;
    // return c2;
// }

// CxCalc CxCalc::div(const CxCalc &c1, const float &fl)
// {
    // if ( fl==0 ) {return prod(c1,1e10);}
    // float dv=1/fl;
    // return prod(c1,dv);
// }

CxCalc CxCalc::conj()
{return CxCalc(re, -im);}

CxCalc CxCalc::conjprod(const CxCalc &c1)
{
    // CxCalc temp = c1;
    // return this * temp.conj();
    return CxCalc(0,0);
}

void CxCalc::print( void )
{
    ICE_OUT("complex value = ( "<<re<<" , "<<im<<" )\n");
}

#endif
