#pragma once

#include <cassert>
#include <vector>

#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

namespace Menge {

/*!
 @brief   The underlying definition of a set-based selector.

 The set-based selector contains a unique collection of `Data` elements. Data instances can be drawn
 from the set based on index, uniform random selection, or weighted random selection.
 */
template <typename Data>
class SetSelector {
 public:
  /*!
   @brief   Reports the number of data elements in the set. *Not* thread safe.
   */
  size_t size() const { return _data.size(); }

  /*!
   @brief   Reports the number of data elements in the set. Thread safe version.
   */
  size_t size_concurrent() const;

  /*!
   @brief   Adds a data element to the set (with an implied unit weight of 1.0). This is thread
            safe.

   @param data  The data element to add to the set. The data element must live longer than this
                set.
   */
  void addData(Data* data) { addData(data, 1.f); }

  /*!
   @brief   Adds a data element to the set with the given weight. This is thread safe.

   @param data    The data element to add to the set. The data element must live longer than this
                  set.
   @param weight  The weight for this element.
   */
  void addData(Data* data, float weight) {
    _lock.lockWrite();
    _data.emplace_back(data, weight);
    _total_weight += weight;
    _lock.releaseWrite();
  }

  /*!
   @brief   Returns the ith element in the set (where i = `index`).
   
   The ordering is defined by the order in which they were added to the set through calls to
   addData(). *Not* thread safe.

   @param index  The index of the desired data element.
   */
  Data* getByIndex(size_t index) {
    assert(index < _data.size());
    return _data[index].data;
  }

  /*!
   @brief   Returns the ith element in the set (where i = `index`).
   
   The ordering is defined by the order in which they were added to the set through calls to
   addData(). The thread safe version.

   @param index  The index of the desired data element.
   */
  Data* getByIndexConcurrent(size_t index) {
    _lock.lockRead();
    Data* data = getByIndex(index);
    _lock.releaseRead();
    return data;
  }

  /*!
   @brief   Select one of the elements where all elements have equal probability.
   */
  Data* getRandom() const {
    const size_t DATA_COUNT = _data.size();
    if (DATA_COUNT) {
      size_t index = static_cast<size_t>(DATA_COUNT * _random_value.getValue());
      // A hacky way to handle a random value of 1.0.
      index = index < DATA_COUNT ? index : DATA_COUNT - 1;
      return _data[index].data;
    }
    return nullptr;
  }

  /*!
   @brief   Select on of the elements with probabilities proportional to their relative weights.
   */
  Data* getWeighted() const {
    Data* result = nullptr;
    if (_data.size() > 0) {
      const float TGT_WEIGHT = _total_weight * _random_value.getValue();
      float accum_weight = 0.f;
      for (auto& item : _data) {
        accum_weight += item.weight;
        if (accum_weight > TGT_WEIGHT) return item.data;
      }
    }
    return result;
  }

 private:
  // Internal structure for storing data and weight.
  struct WeightedData {
    WeightedData(Data* data_in, float weight_in) : data(data_in), weight(weight_in) {}
    Data* data;
    float weight;
  };

  // The members of the set.
  std::vector<WeightedData> _data;

  // Bookkeeping -- the total accumulated weight.
  float _total_weight{0.f};

  // Built-in random-number generator.
  mutable Math::UniformFloatGenerator _random_value{0.f, 1.f};

  // The lock to maintain readers-writer access to the structure which controls the data.
  mutable ReadersWriterLock _lock;
};
}  // namespace Menge
