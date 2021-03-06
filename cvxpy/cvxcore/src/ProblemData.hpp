//   Copyright 2017 Steven Diamond
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include <map>
#include <vector>

/* Stores the result of calling BUILD_MATRIX on a collection of LinOp
 * trees. */
class ProblemData {
public:
  /* COO sparse matrix representation. V stores the data, I the row indices
   * and J the column indices. */
  std::map<int, std::vector<std::vector<double> > > TensorV;
  std::map<int, std::vector<std::vector<int> > > TensorI;
  std::map<int, std::vector<std::vector<int> > > TensorJ;

  // Pointers needed to extract V, I, J.
  int param_id;
  int vec_idx;

  // Initialize TensorV/I/J for the given parameter.
  void init_id(int new_param_id, int param_size) {
    assert(TensorV.count(new_param_id) == 0);
    std::vector<std::vector<double> > vecV(param_size);
    std::vector<std::vector<int> > vecI(param_size);
    std::vector<std::vector<int> > vecJ(param_size);
    for (int i = 0; i < param_size; ++i) {
      std::vector<double> elemV;
      std::vector<int> elemI;
      std::vector<int> elemJ;
      vecV.push_back(elemV);
      vecI.push_back(elemI);
      vecJ.push_back(elemJ);
    }
    TensorV[new_param_id] = vecV;
    TensorI[new_param_id] = vecI;
    TensorJ[new_param_id] = vecJ;
  }

  /*******************************************
   * The functions below return problemData vectors as contiguous 1d
   * numpy arrays.
   *
   * Note the function prototypes must match CVXCanon.i exactly to
   * properly run and compile.
   *
   * Each function is wrapped using SWIG's numpy.i typemap, so can
   * be called in python using
   *
   * 				problemData.getV(N)
   *
   * where N is the length of the vector V. The double *pointer VALUES
   * is generated by the wrapper, which allocates space for NUM_VALUES
   * elements. Thus, NUM_VALUES must be exactly the length of the array.
   ********************************************/

  // Get length of V, I, J.
  int getLen() {
    std::vector<double> V = TensorV[param_id][vec_idx];
    return V.size();
  }

  /**
   * Returns the data vector V as a contiguous 1D numpy array.
   */
  void getV(double *values, int num_values) {
    std::vector<double> V = TensorV[param_id][vec_idx];
    for (int i = 0; i < num_values; i++) {
      values[i] = V[i];
    }
  }

  /**
   * Returns the row index vector I as a contiguous 1D numpy array.
   */
  void getI(double *values, int num_values) {
    std::vector<int> I = TensorI[param_id][vec_idx];
    for (int i = 0; i < num_values; i++) {
      values[i] = I[i];
    }
  }

  /**
   * Returns the column index vector J as a contiguous 1D numpy array.
   */
  void getJ(double *values, int num_values) {
    std::vector<int> J = TensorJ[param_id][vec_idx];
    for (int i = 0; i < num_values; i++) {
      values[i] = J[i];
    }
  }
};

#endif
