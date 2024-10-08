
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
#include <parfait/Extent.h>
#include <parfait/ExtentMeshWrapper.h>

using namespace Parfait;

TEST_CASE("To turn extent boxes into a mesh that can be visualized") {
    Extent<double> e{{0, 0, 0}, {1, 1, 1}};
    ExtentMeshWrapper extentMesh;
    extentMesh.addExtent(e);
    REQUIRE(8 == extentMesh.numberOfNodes());
    REQUIRE(1 == extentMesh.numberOfCells());
    REQUIRE(8 == extentMesh.numberOfNodesInCell(0));
    auto p = extentMesh.getNode(1);
    REQUIRE(p[0] == 1);
    REQUIRE(p[1] == 0);
    REQUIRE(p[2] == 0);

    extentMesh.addExtent(e);
    REQUIRE(16 == extentMesh.numberOfNodes());
    REQUIRE(2 == extentMesh.numberOfCells());
    p = extentMesh.getNode(13);
    REQUIRE(p[0] == 1);
    REQUIRE(p[1] == 0);
    REQUIRE(p[2] == 1);

    auto vertexIds = extentMesh.getNodesInCell(1);
    REQUIRE(8 == vertexIds[0]);
    REQUIRE(9 == vertexIds[1]);
    REQUIRE(10 == vertexIds[2]);
    REQUIRE(11 == vertexIds[3]);
    REQUIRE(12 == vertexIds[4]);
    REQUIRE(13 == vertexIds[5]);
    REQUIRE(14 == vertexIds[6]);
    REQUIRE(15 == vertexIds[7]);
}

TEST_CASE("test constructor that takes a single extent") {
    Extent<double> e{{0, 0, 0}, {1, 1, 1}};
    ExtentMeshWrapper wrapper1(e);
    REQUIRE(8 == wrapper1.numberOfNodes());
}

TEST_CASE("test vector constructor") {
    Extent<double> e{{0, 0, 0}, {1, 1, 1}};
    std::vector<Extent<double>> vec;
    vec.push_back(e);
    vec.push_back(e);
    ExtentMeshWrapper wrapper(vec);
    REQUIRE(16 == wrapper.numberOfNodes());
}
