
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

#include <vector>
#include <set>

namespace Parfait {
template <typename MeshType>
class NodeToNodeBuilder {
  public:
    NodeToNodeBuilder(MeshType& m) : mesh(m), node_to_node(mesh.numberOfNodes()){};

    std::vector<std::vector<int>> buildNodeToNodeConnectivity();

  private:
    MeshType& mesh;
    std::vector<std::set<int>> node_to_node;
    template <size_t N>
    void addCell(const std::vector<int>& cell, const std::array<std::array<int, 2>, N>& edges) {
        for (auto& edge : edges) {
            node_to_node[cell[edge[0]]].insert(cell[edge[1]]);
            node_to_node[cell[edge[1]]].insert(cell[edge[0]]);
        }
    }
};

}
#include "NodeToNode.hpp"