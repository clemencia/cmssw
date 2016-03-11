#ifndef CTPPSALIGNMENTCORRECTIONS_H
#define CTPPSALIGNMENTCORRECTIONS_H
 
#include <iostream>
#include <string>

#include "CondFormats/Serialization/interface/Serializable.h"

// create vector of this to have already the set of corrections
class CTPPSAlignmentCorrections{
 public:

  //Default constructor
 CTPPSAlignmentCorrections():
  m_detid(0),
    m_rpid(0){
    setShiftsAndRotations();
  }

  CTPPSAlignmentCorrections(int, int, float,float,float,float,float,float,float,float,float,float);
  int    getDetID(){return m_detid;}
  int    getRPID(){return m_rpid;}
  void   setDetID(int detid){m_detid=detid;}
  void   setRPID(int rpid){m_rpid=rpid;}
  void   setRPandDetIDs(int rpid, int detid);

  float    getShiftX(){return m_sh_x;}
  float    getShiftY(){return m_sh_y;}
  float    getShiftZ(){return m_sh_z;}
  float    getShiftR(){return m_sh_r;}
  float    getShiftXError(){return m_sh_x_e;}
  float    getShiftYError(){return m_sh_y_e;}
  float    getShiftZError(){return m_sh_z_e;}
  float    getShiftRError(){return m_sh_r_e;}
  float    getRotZ(){return m_rot_z;}
  float    getRotZError(){return m_rot_z_e;}

  void    setShiftX(float sh){m_sh_x=sh;}
  void    setShiftY(float sh){m_sh_y=sh;}
  void    setShiftZ(float sh){m_sh_z=sh;}
  void    setShiftR(float sh){m_sh_r=sh;}
  void    setShiftXError(float she){m_sh_x_e=she;}
  void    setShiftYError(float she){m_sh_y_e=she;}
  void    setShiftZError(float she){m_sh_z_e=she;}
  void    setShiftRError(float she){m_sh_r_e=she;}
  void    setRotZ(float rot){m_rot_z=rot;}
  void    setRotZError(float rote){m_rot_z_e=rote;}

  void    setShiftsAndRotations(float shr=0.0, float shx=0.0,float shy=0.0,float shz=0.0,float rotz=0.0,float shre=0.0,float shxe=0.0,float shye=0.0,float shze=0.0,float rotze=0.0);

 private:

  int m_detid;
  int m_rpid;

  //shifts in x,y,z
  float m_sh_x;
  float m_sh_x_e;
  
  float m_sh_y;
  float m_sh_y_e;

  float m_sh_z;
  float m_sh_z_e;

  //rotation in z
  float m_rot_z;
  float m_rot_z_e;

  //shift in radial direction
  float m_sh_r;
  float m_sh_r_e;

  COND_SERIALIZABLE;

};


#endif
