#ifndef __TRAIL_H__
#define __TRAIL_H__

#include "dataStructures.h"
#include "oglDataCache.h"
#include <string>
#include <vector>


namespace SdimpleRocket {


class Trail { 
 protected:
  Float m_m; //object mass, kg
  //Float m_h; //rotation around object origin, in pi-radians
  //Float m_w; //object angular velocity, pi-radians/s, only for the depth axis (z)
  State m_state; //this object's position and current velocity

  //as we compute now, these are secondary properties, entirely computed from primary properties
  Derivative m_derivative; //m_state derivatives, in respect to time
  Vector3 m_p; //momentum vector kg * m / s
  Float m_im; //object inverse mass, kg^{-1}
  Float m_panta;
  Vector3 m_force;//the resultant of all forces acting upon this object

  
  void update();
  void updateMass();

 public:
  
  Trail(Vector3 position, Float mass, Vector3 velocity);


  Vector3 getPosition() { return m_state.r; }
  void setPosition(Vector3 position) { m_state.r = position; }
  //void translate(Vector3 deltaR) { m_state.r += deltaR; }
  Float getMass() const { return m_m; }
  void setMass(Float mass) { m_m = mass; updateMass(); }
  Float getPanta() const { return m_panta; }
  void setPanta(Float panta) { m_panta = panta; }
  Vector3 getVelocity() const { return m_state.v; }
  void setVelocity(Vector3 velocity) { m_state.v = velocity; }
  Vector3 getDPosition() const { return m_derivative.dr; }
  void setDPosition(Vector3 dPosition) { m_derivative.dr = dPosition; }
  Vector3 getDVelocity() const { return m_derivative.dv; }
  void setDVelocity(Vector3 dVelocity) { m_derivative.dv = dVelocity; }
  State getState() const { return m_state; }
  void setState(State state) { m_state = state; }
  Derivative getDerivative() const { return m_derivative; }
  void setDerivative(Derivative Derivative) { m_derivative = Derivative; }
  void setAcceleration(const Vector3 &acceleration = Vector3()) { m_derivative.dv = acceleration; }
  void addAcceleration(const Vector3 &acceleration) { m_derivative.dv += acceleration; }


  //physics
  Vector3 linearAcceleration(Vector3 force) const;

  Derivative evalAccel(State s, const Derivative d, Float dt) const ;
  //move the object affected by a force during a certain time
  void integrateRk4Accel(Float dt);



  inline void integrateAccel(Float dt) { integrateRk4Accel(dt); }
  inline void forceToAccel() { m_derivative.dv += m_force / m_m; }

};


}
#endif
