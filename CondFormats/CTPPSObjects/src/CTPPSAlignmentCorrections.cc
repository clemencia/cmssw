#include "CondFormats/CTPPSObjects/interface/CTPPSAlignmentCorrections.h"

void CTPPSAlignmentCorrections::setShiftsAndRotations(float shr,
						      float shx,
						      float shy,
						      float shz,
						      float rotz,
						      float shre,
						      float shxe,
						      float shye,
						      float shze,
						      float rotze) {
  
  m_sh_x = shx;
  m_sh_y = shy;
  m_sh_z = shz;
  
  m_sh_x_e = shxe;
  m_sh_y_e = shye;
  m_sh_z_e = shze;
  
  m_rot_z   = rotz;
  m_rot_z_e = rotze;

  m_sh_r = shr;
  m_sh_r_e = shre;
  
}


void    CTPPSAlignmentCorrections::setRPandDetIDs(int rpid, int detid){
  m_detid=detid;
  m_rpid=rpid;
}
