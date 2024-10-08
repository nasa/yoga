#pragma once
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space
// Administration. No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
//
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.

#include "Adt.h"
#include "Point.h"
#include "UnitTransformer.h"

namespace Parfait {
class Adt3DExtent {
  public:
    Adt3DExtent() = delete;
    Adt3DExtent(const Extent<double>& domain);
    void reserve(size_t n);
    void store(int id, const Extent<double>& extent);
    std::vector<int> retrieve(const Extent<double>& domain) const;
    void retrieve(const Extent<double>& domain, std::vector<int>& ids) const;
    void removeFirst(int id, const Extent<double>& e);
    Parfait::Extent<double> boundingExtent() const;

    // Warning: after calling tighten(), you can no longer store
    // new items in the tree.
    // Tighten shrinks each node of the tree to fit its children
    // to speed up queries (sometimes, by quite a lot).
    void tighten();

  private:
    UnitTransformer<double> unitTransformer;
    Adt<6> adt;
};
}
#include "Adt3dExtent.hpp"
