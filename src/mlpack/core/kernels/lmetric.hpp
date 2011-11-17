/**
 * @file lmetric.h
 * @author Ryan Curtin
 *
 * Generalized L-metric, allowing both squared distances to be returned as well
 * as non-squared distances.  The squared distances are faster to compute.
 *
 * This also gives several convenience typedefs for commonly used L-metrics.
 */
#ifndef __MLPACK_CORE_KERNELS_LMETRIC_H
#define __MLPACK_CORE_KERNELS_LMETRIC_H

#include <mlpack/core.h>

namespace mlpack {
namespace kernel {

/**
 * The L_p metric for arbitrary integer p, with an option to take the root.
 *
 * This class implements the standard L_p metric for two arbitary vectors @f$ x
 * @f$ and @f$ y @f$ of dimensionality @f$ n @f$:
 *
 * @f[
 * d(x, y) = \left( \sum_{i = 1}^{n} | x_i - y_i |^p \right)^{\frac{1}{p}}.
 * @f]
 *
 * The value of p is given as a template parameter.
 *
 * In addition, the function @f$ d(x, y) @f$ can be simplified, neglecting the
 * p-root calculation.  This is done by specifying the t_take_root template
 * parameter to be false.  Then,
 *
 * @f[
 * d(x, y) = \sum_{i = 1}^{n} | x_i - y_i |^p
 * @f]
 *
 * It is faster to compute that distance, so t_take_root is by default off.
 *
 * A few convenience typedefs are given:
 *
 *  - ManhattanDistance
 *  - EuclideanDistance
 *  - SquaredEuclideanDistance
 *
 * @tparam t_pow Power of metric; i.e. t_pow = 1 gives the L1-norm (Manhattan
 *   distance).
 * @tparam t_take_root If true, the t_pow'th root of the result is taken before
 *   it is returned.  In the case of the L2-norm (t_pow = 2), when t_take_root
 *   is true, the squared L2 distance is returned.  It is slightly faster to set
 *   t_take_root = false, because one fewer call to pow() is required.
 */
template<int t_pow, bool t_take_root = false>
class LMetric {
 public:
  /***
   * Default constructor does nothing, but is required to satisfy the Kernel
   * policy.
   */
  LMetric() { }

  /**
   * Computes the distance between two points.
   */
  static double Evaluate(const arma::vec& a, const arma::vec& b);
};

// Doxygen will not include this specialization.
//! @cond

// The implementation is not split into a _impl.h file because it is so simple;
// the unspecialized implementation of the one function is given below.
// Unspecialized implementation.  This should almost never be used...
template<int t_pow, bool t_take_root>
double LMetric<t_pow, t_take_root>::Evaluate(const arma::vec& a,
                                             const arma::vec& b) {
  double sum = 0;
  for (size_t i = 0; i < a.n_elem; i++)
    sum += pow(fabs(a[i] - b[i]), t_pow);

  if (!t_take_root) // Suboptimal to have this here.
    return sum;

  return pow(sum, (1.0 / t_pow));
}

//! @endcond

// Convenience typedefs.

/***
 * The Manhattan (L1) distance.
 */
typedef LMetric<1, false> ManhattanDistance;

/***
 * The squared Euclidean (L2) distance.
 */
typedef LMetric<2, false> SquaredEuclideanDistance;

/***
 * The Euclidean (L2) distance.
 */
typedef LMetric<2, true> EuclideanDistance;

}; // namespace kernel
}; // namespace mlpack

#endif
