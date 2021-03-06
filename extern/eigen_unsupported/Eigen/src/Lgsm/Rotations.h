// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009-2011 CEA LIST (DIASI/LSI) <xde-support@saxifrage.cea.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef LGSM_ROTATIONS_H
#define LGSM_ROTATIONS_H

template <typename QuatDerived, typename Derived>
inline typename internal::traits<QuatDerived>::PlainObject projectRotation(
    const QuaternionBase<QuatDerived>& q,
    const MatrixBase<Derived>& n,
    typename internal::traits<Derived>::Scalar& theta)
{
  typedef typename internal::traits<Derived>::Scalar Scalar;
  typedef typename internal::traits<QuatDerived>::PlainObject Quaternion;
  typedef Matrix<Scalar, 3, 3> AdjointMatrix;

  const AdjointMatrix& R = q.toRotationMatrix();

  //tr(R^t*[n])
  Scalar tr1 = (R(2,1)-R(1,2))*n[0] + (R(0,2)-R(2,0))*n[1] + (R(1,0)-R(0,1))*n[2];
  //tr(R^t*[n]^2) = tr(R^t*n*n^t) - tr(R^t) since n^2 = 1
  Scalar tr2 =  n[0] * (R(0,0)*n[0]+R(1,0)*n[1]+R(2,0)*n[2])
    + n[1] * (R(0,1)*n[0]+R(1,1)*n[1]+R(2,1)*n[2])
    + n[2] * (R(0,2)*n[0]+R(1,2)*n[1]+R(2,2)*n[2])
    - R(0,0) - R(1,1) - R(2,2);

  Scalar alpha = atan2(tr1, tr2);
  theta = M_PI - alpha;
  Scalar s = sin(theta);
  Scalar c = cos(theta);
  Scalar ic = 1-c;

  AdjointMatrix Rp;
  Rp(0,0) =          + ic*n[0]*n[0] + c;
  Rp(0,1) = -s*n[2] + ic*n[0]*n[1];
  Rp(0,2) =  s*n[1] + ic*n[0]*n[2];
  Rp(1,0) =  s*n[2] + ic*n[1]*n[0];
  Rp(1,1) =          + ic*n[1]*n[1] + c;
  Rp(1,2) = -s*n[0] + ic*n[1]*n[2];
  Rp(2,0) = -s*n[1] + ic*n[2]*n[0];
  Rp(2,1) =  s*n[0] + ic*n[2]*n[1];
  Rp(2,2) =          + ic*n[2]*n[2] + c;

  return Quaternion(Rp);
}

template <typename QuatDerived, typename Derived>
inline typename internal::traits<QuatDerived>::PlainObject projectRotation(
    const QuaternionBase<QuatDerived>& q,
    const MatrixBase<Derived>& v)
{
  typename internal::traits<Derived>::Scalar tmp;
  return projectRotation(q, v, tmp);
}

  template <typename QuatDerived, typename Derived>
inline bool isRotationAround(const QuaternionBase<QuatDerived>& q, const MatrixBase<Derived>& v)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived);
  typedef typename QuaternionBase<QuatDerived>::Scalar Scalar;
  AngleAxis<Scalar> aa(q);
  return aa.axis().isApprox(v) || aa.axis().isApprox(-v) || aa.angle() == 0;  //if the rotation is very small, eigen put axis to (1,0,0) and angle to 0
}

  template <typename QuatDerived, typename Derived>
inline typename QuaternionBase<QuatDerived>::Scalar getAngleAround(const QuaternionBase<QuatDerived>& q, const MatrixBase<Derived>& v)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived);
  assert(isRotationAround(q,v));
  typedef typename QuaternionBase<QuatDerived>::Scalar Scalar;
  AngleAxis<Scalar> aa(q);
  return aa.angle()*(aa.axis().dot(v));
}

#endif
