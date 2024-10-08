
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
#include <RingAssertions.h>
#include <vector>
#include <parfait/Point.h>
#include <parfait/DualMetrics.h>

TEST_CASE("Add dual metrics tet") {
    std::vector<std::array<int, 2>> edges = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
    std::vector<Parfait::Point<double>> xyz = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 0, 1}};
    std::vector<int> tet = {0, 1, 2, 3};

    Parfait::DualMetrics dual_metrics(4, 6);
    auto getTet = [&](int tet_id) { return tet; };
    auto getXyz = [&](int node) { return xyz[node]; };
    Parfait::addTetDualMetrics(getTet, getXyz, 1, edges, dual_metrics);

    for (size_t edge = 0; edge < edges.size(); edge++) {
        int lnode = edges[edge][0];
        int rnode = edges[edge][1];
        auto& a = dual_metrics.edge_areas[edge];
        if (lnode == 0 and rnode == 1) {
            REQUIRE(a[0] == Approx(0.083333));
            REQUIRE(a[1] == Approx(-0.041667));
            REQUIRE(a[2] == Approx(0.041667));
        }
        if (lnode == 1 and rnode == 2) {
            REQUIRE(a[0] == Approx(-0.041667));
            REQUIRE(a[1] == Approx(0.083333));
            REQUIRE(a[2] == Approx(0));
        }
        if (lnode == 0 and rnode == 2) {
            REQUIRE(a[0] == Approx(0.041667));
            REQUIRE(a[1] == Approx(0.041667));
            REQUIRE(a[2] == Approx(0.041667));
        }
    }
}
TEST_CASE("Add dual metrics triangle") {
    std::vector<std::array<int, 2>> edges = {{0, 1}, {0, 2}, {1, 2}};
    std::vector<Parfait::Point<double>> xyz = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
    std::vector<int> tri = {0, 1, 2};

    Parfait::DualMetrics dual_metrics(3, 3);
    auto getTri = [&](int tri_id) { return tri; };
    auto getXyz = [&](int node) { return xyz[node]; };
    Parfait::addTriDualMetrics(getTri, getXyz, 1, edges, dual_metrics);

    auto center = Parfait::CGNS::Triangle::computeCenter(xyz);
    for (size_t edge = 0; edge < edges.size(); edge++) {
        auto a = dual_metrics.edge_areas[edge];
        int lnode = edges[edge].front();
        int rnode = edges[edge].back();
        auto e = 0.5 * (xyz[lnode] + xyz[rnode]) - center;
        REQUIRE(0.0 == Approx(Parfait::Point<double>::dot(a, e)));
        // The convention is that edge areas should point from low to high node
        auto edge_vector = xyz[std::max(rnode, lnode)] - xyz[std::min(rnode, lnode)];
        REQUIRE(Parfait::Point<double>::dot(a, edge_vector) > 0.0);
    }
    auto total_area = std::accumulate(dual_metrics.node_volumes.begin(), dual_metrics.node_volumes.end(), 0.0);
    REQUIRE(0.5 == Approx(total_area));
}