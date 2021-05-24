#include "trail.h"

using namespace std;
using namespace SdimpleRocket;

Trail::Trail(Vector3 position = Vector3(0, 0, 0), Float mass = 1, Vector3 velocity = Vector3(0, 0, 0)) {
  m_m = mass;
  m_state.r = position;
  m_state.v = velocity;
  m_derivative.dr = 0;
  m_derivative.dv = 0;
  m_panta = m_state.v.getY()/m_state.v.getX();
  update();
}

void Trail::updateMass() {
  m_p = m_state.v * m_m;
  m_im = pow(m_m, -1);
}

void Trail::update() {
  updateMass();
}


Derivative Trail::evalAccel(State s, Derivative d, Float dt) const {
  State n;
  Derivative out;
  //n.r = s.r + d.dr * dt; //update position: old position + velocity * time. Not needed for this computation.
  n.v = s.v + d.dv * dt; //update velocity: old vel + accel * time

  out.dr = n.v; //obviously, since dr/t is the velocity
  out.dv = m_derivative.dv;
  return out;
}


void Trail::integrateRk4Accel(Float dt) {
  Derivative a, b, c, d;
  a = evalAccel(m_state, m_derivative, 0);
  b = evalAccel(m_state, a, dt * 0.5);
  c = evalAccel(m_state, b, dt * 0.5);
  d = evalAccel(m_state, c, dt);
  //weighted average of multiple points across the time interval [0, dt]
  Vector3 drdt = (a.dr + Float(2.0) * (b.dr + c.dr) + d.dr) / Float(6.0);
  Vector3 dvdt = (a.dv + Float(2.0) * (b.dv + c.dv) + d.dv) / Float(6.0);
  //now that we have a good estimate of velocity and acceleration, use them to update position and velocity.
  m_state.r += drdt * dt;
  m_state.v += dvdt * dt;
  m_panta = m_state.v.getY()/m_state.v.getX();
}

