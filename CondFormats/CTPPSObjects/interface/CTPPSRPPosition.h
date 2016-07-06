
#ifndef _CTPPSRPPosition_h_
#define _CTPPSRPPosition_h_



#include "CondFormats/Serialization/interface/Serializable.h"

class CTPPSRPPosition
{
 public:
  //   CTPPSRPPosition(){}
  virtual ~CTPPSRPPosition(){}

  CTPPSRPPosition(double dist=0.0,double offset=0.0, double LVD=-1.0, double MOTOR=-1.0, double RESOLVER=-1.0 );


  double getDistBPCenter() const  {return m_distBPCenter;}
  double getOffset() const        {return m_offset;}
  double getRawLVD() const        {return m_lvd;}
  double getRawMotor() const      {return m_motor;}
  double getRawResolver() const   {return m_resolver;}

 private:
  double m_distBPCenter;
  double m_offset;
  double m_lvd;
  double m_motor;
  double m_resolver;

  COND_SERIALIZABLE;

};

#endif


