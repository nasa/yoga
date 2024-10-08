
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
#include <parfait/CartBlock.h>
#include <parfait/Point.h>
#include <cmath>

using namespace Parfait;

TEST_CASE("CartBlockTestsDimensionCheck") {
    double p1[3] = {1.0, 2.0, 3.0};
    double p2[3] = {2.2, 3.2, 4.2};

    CartBlock block(p1, p2, 1, 1, 1);
    double lo[3];
    double hi[3];
    lo[0] = block.lo[0];
    lo[1] = block.lo[1];
    lo[2] = block.lo[2];

    hi[0] = block.hi[0];
    hi[1] = block.hi[1];
    hi[2] = block.hi[2];

    REQUIRE(p1[0] == Approx(lo[0]));
    REQUIRE(p1[1] == Approx(lo[1]));
    REQUIRE(p1[2] == Approx(lo[2]));

    REQUIRE(p2[0] == Approx(hi[0]));
    REQUIRE(p2[1] == Approx(hi[1]));
    REQUIRE(p2[2] == Approx(hi[2]));
}
TEST_CASE("CartBlockTestsCells") {
    double p1[3] = {0.0, 0.0, 0.0};
    double p2[3] = {1.0, 1.0, 1.0};

    int numCells_x = 10;
    int numCells_y = 5;
    int numCells_z = 10;
    CartBlock block(p1, p2, numCells_x, numCells_y, numCells_z);

    REQUIRE(0.1 == Approx(block.get_dx()));
    REQUIRE(0.2 == Approx(block.get_dy()));
    REQUIRE(0.1 == Approx(block.get_dz()));

    REQUIRE(10 == block.numberOfCells_X());
    REQUIRE(5 == block.numberOfCells_Y());
    REQUIRE(10 == block.numberOfCells_Z());

    int numberOfCells = 500;
    REQUIRE(numberOfCells == block.numberOfCells());

    int numberOfNodes = 726;
    REQUIRE(numberOfNodes == block.numberOfNodes());
}

TEST_CASE("Make sure you get the right containing cell ids") {
    CartBlock block({0, 0, 0}, {1, 1, 1}, 2, 2, 2);

    Point<double> pointOutsideBlock(3, 3, 3);
    REQUIRE_THROWS(block.getIdOfContainingCell(pointOutsideBlock.data()));

    SECTION("Test inside each cell") {
        Point<double> p;
        for (int i = 0; i < block.numberOfCells(); ++i) {
            block.getCellCentroid(i, p.data());
            REQUIRE(i == block.getIdOfContainingCell(p.data()));
        }
    }
    SECTION("Test corners") {
        Point<double> p = {0, 0, 0};
        REQUIRE(0 == block.getIdOfContainingCell(p.data()));
        p = {1, 0, 0};
        REQUIRE(1 == block.getIdOfContainingCell(p.data()));
        p = {0, 1, 0};
        REQUIRE(2 == block.getIdOfContainingCell(p.data()));
        p = {1, 1, 0};
        REQUIRE(3 == block.getIdOfContainingCell(p.data()));
        p = {0, 0, 1};
        REQUIRE(4 == block.getIdOfContainingCell(p.data()));
        p = {1, 0, 1};
        REQUIRE(5 == block.getIdOfContainingCell(p.data()));
        p = {0, 1, 1};
        REQUIRE(6 == block.getIdOfContainingCell(p.data()));
        p = {1, 1, 1};
        REQUIRE(7 == block.getIdOfContainingCell(p.data()));
    }
    SECTION("Test range of overlapping cells") {
        Point<double> p1, p2;
        SECTION("bottom corner") {
            p1 = p2 = {0, 0, 0};
            auto range = block.getRangeOfOverlappingCells({p1, p2});
            REQUIRE(range.lo[0] == 0);
            REQUIRE(range.hi[0] == 1);
            REQUIRE(range.lo[1] == 0);
            REQUIRE(range.hi[1] == 1);
            REQUIRE(range.lo[2] == 0);
            REQUIRE(range.hi[2] == 1);
        }
        SECTION("top corner") {
            p1 = p2 = {1, 1, 1};
            auto range = block.getRangeOfOverlappingCells({p1, p2});
            int n = block.numberOfCells_X();
            REQUIRE(range.lo[0] == n - 1);
            REQUIRE(range.hi[0] == n);
            REQUIRE(range.lo[1] == n - 1);
            REQUIRE(range.hi[1] == n);
            REQUIRE(range.lo[2] == n - 1);
            REQUIRE(range.hi[2] == n);
        }
        SECTION("top face") {
            p1 = {0, 0, 1};
            p2 = {1, 1, 1};
            auto range = block.getRangeOfOverlappingCells({p1, p2});
            int n = block.numberOfCells_X();
            REQUIRE(range.lo[0] == 0);
            REQUIRE(range.hi[0] == n);
            REQUIRE(range.lo[1] == 0);
            REQUIRE(range.hi[1] == n);
            REQUIRE(range.lo[2] == n - 1);
            REQUIRE(range.hi[2] == n);
        }
    }
}

TEST_CASE("CartBlockTestsNodeRetrieval") {
    double p1[3] = {1.0, 2.0, 3.0};
    double p2[3] = {2.2, 3.2, 4.2};

    int numCells_x = 5;
    int numCells_y = 6;
    int numCells_z = 7;
    int numNodes_x = 6;
    int numNodes_y = 7;
    int numNodes_z = 8;
    CartBlock block(p1, p2, numCells_x, numCells_y, numCells_z);

    double p4[3], p5[3];

    int numNodes = numNodes_x * numNodes_y * numNodes_z;
    // block.get_lo_point(p4);
    p4[0] = block.lo[0];
    p4[1] = block.lo[1];
    p4[2] = block.lo[2];
    block.getNode(0, p5);

    REQUIRE(1.0 == Approx(p4[0]));
    REQUIRE(2.0 == Approx(p4[1]));
    REQUIRE(3.0 == Approx(p4[2]));

    REQUIRE(1.0 == Approx(p5[0]));
    REQUIRE(2.0 == Approx(p5[1]));
    REQUIRE(3.0 == Approx(p5[2]));

    // block.get_hi_point(p4);
    p4[0] = block.hi[0];
    p4[1] = block.hi[1];
    p4[2] = block.hi[2];
    block.getNode(numNodes - 1, p5);

    REQUIRE(2.2 == Approx(p4[0]));
    REQUIRE(3.2 == Approx(p4[1]));
    REQUIRE(4.2 == Approx(p4[2]));

    REQUIRE(2.2 == Approx(p5[0]));
    REQUIRE(3.2 == Approx(p5[1]));
    REQUIRE(4.2 == Approx(p5[2]));

    // block.get_hi_point(p4);
    p4[0] = block.hi[0];
    p4[1] = block.hi[1];
    p4[2] = block.hi[2];

    p4[2] -= block.getLength_Z();
    block.getNode(numCells_x + numNodes_x * (numCells_y), p5);

    REQUIRE(2.2 == Approx(p4[0]));
    REQUIRE(3.2 == Approx(p4[1]));
    REQUIRE(3.0 == Approx(p4[2]));
}

TEST_CASE("CartBlockTestsLegacyTests") {
    double p1[3], p2[3], p3[3], extent[6];
    p1[0] = 1.0;
    p1[1] = 2.0;
    p1[2] = 3.0;
    p2[0] = 2.2;
    p2[1] = 3.2;
    p2[2] = 4.2;
    p3[0] = 1.7;
    p3[1] = 2.7;
    p3[2] = 3.7;

    extent[0] = p1[0];
    extent[1] = p1[1];
    extent[2] = p1[2];
    extent[3] = p2[0];
    extent[4] = p2[1];
    extent[5] = p2[2];

    int ncells_x, ncells_y, ncells_z;
    ncells_x = 5;
    ncells_y = 6;
    ncells_z = 7;
    CartBlock c(p1, p2, ncells_x, ncells_y, ncells_z);
    CartBlock d(extent, ncells_x, ncells_y, ncells_z);

    // this also relies on the centroid being in the right cell
    int cell_id = c.getIdOfContainingCell(p3);
    auto cell = c.createExtentFromCell(cell_id);
    REQUIRE(cell.intersects(Point<double>(p3)));

    std::vector<int> ids;
    // make a box around the whole meshBuilder, and make sure you get
    // all the cells
    Extent<double> query_box(p1, p2);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_x * ncells_y * ncells_z) == (int)ids.size());

    // make a box that does not overlap the meshBuilder and assert that it returns no cells
    extent[0] = -10.0;
    extent[1] = -10.0;
    extent[2] = -10.0;
    extent[3] = 0.0;
    extent[4] = 10.0;
    extent[5] = 10.0;
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);

    REQUIRE((int)ids.size() == 0);

    // make a box bigger than the meshBuilder and make sure it returns all of them
    extent[3] = 10.0;
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_x * ncells_y * ncells_z) == (int)ids.size());

    // make a box that only touches the -z face
    extent[3] = 10.0;
    extent[5] = p1[2];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_x * ncells_y) == (int)ids.size());

    // make a box that only touches the +z face
    extent[5] = 10.0;
    extent[2] = p2[2];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    printf("+z face\n");
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_x * ncells_y) == (int)ids.size());

    // make a box that only touches the -x face
    extent[2] = -10.0;
    extent[3] = p1[0];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_z * ncells_y) == (int)ids.size());

    // make a box that only touches the +x face
    extent[3] = 10.0;
    extent[0] = p2[0];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_z * ncells_y) == (int)ids.size());

    // make a box that only touches the -y face
    extent[0] = -10.0;
    extent[4] = p1[1];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_z * ncells_x) == (int)ids.size());

    // make a box that only touches the +y face
    extent[4] = 10.0;
    extent[1] = p2[1];
    // query_box.resize(extent);
    query_box = Extent<double>(extent);
    ids = c.getCellIdsInExtent(query_box);
    REQUIRE((ncells_z * ncells_x) == (int)ids.size());
}

TEST_CASE("get cell nodes") {
    CartBlock block({0, 0, 0}, {1, 1, 1}, 1, 1, 1);
    auto cell = block.getNodesInCell(0);
    REQUIRE(8 == cell.size());

    REQUIRE(0 == cell[0]);
    REQUIRE(1 == cell[1]);
    REQUIRE(3 == cell[2]);
    REQUIRE(2 == cell[3]);
    REQUIRE(4 == cell[4]);
    REQUIRE(5 == cell[5]);
    REQUIRE(7 == cell[6]);
    REQUIRE(6 == cell[7]);
}

TEST_CASE("Special case intersecting part of a min-face") {
    CartBlock block({-8.809568, -176.999718, 69.201588}, {1142.415365, 176.999718, 210.646513}, 20, 20, 20);
    Point<double> p{1016.946487, 0.000000, 82.681965};
    auto id_range = block.getCellIdsInExtent({p, p});
    printf("There are %li cells overlapping this extent\n", id_range.size());
    REQUIRE(block.intersects(p));
    REQUIRE(id_range.size() > 0);
}
