
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
#pragma once
#include <string>

namespace Parfait {
class Facet;
namespace STL {
    class BinaryReader {
      public:
        explicit BinaryReader(std::string fileName);
        std::vector<Facet> readFacets();
        std::vector<Facet> readFacets(int start, int end);
        bool isConsistent();

      private:
        std::string fileName;
        void throwIfFileDoesNotExist();
        void* putPointerAtFacet(FILE* fp, int start);
        void chompHeader(FILE* fp);
        void readFacet(FILE* fp, Facet& facet);
        size_t readFacetCount();
        bool isFacetCountCorrect(size_t count);
    };

    class AsciiReader {
      public:
        explicit AsciiReader(std::string filename);
        std::vector<Facet> readFacets();

      private:
        std::string filename;
    };
}
}

#include "STLReader.hpp"
