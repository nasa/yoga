
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
#include <parfait/Adt.h>

using namespace Parfait;

TEST_CASE("Adt, StoreNothing") {
    Adt<3> adt;
    double extent[6] = {0, 0, 0, 1, 1, 1};
    auto inside = adt.retrieve(extent);

    REQUIRE(inside.empty());
}

TEST_CASE("Adt, Store3DPoints") {
    Adt<3> adt;
    double extent[6] = {0, 0, 0, 1, 1, 1};
    double point1[3] = {0.1, 0.1, 0.1};
    double point2[3] = {0.7, 0.7, 0.7};
    adt.store(10, point1);
    adt.store(11, point2);

    std::vector<int> inside;
    inside = adt.retrieve(extent);

    REQUIRE(2 == inside.size());

    extent[0] = 0;
    extent[1] = 0;
    extent[2] = 0;
    extent[3] = 0.5;
    extent[4] = 0.5;
    extent[5] = 0.5;
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(10 == inside[0]);

    extent[0] = 0.5;
    extent[1] = 0.5;
    extent[2] = 0.5;
    extent[3] = 1;
    extent[4] = 1;
    extent[5] = 1;
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(11 == inside[0]);
}

TEST_CASE("Adt, Store2DPoints") {
    Adt<2> adt;
    double extent[4] = {0, 0, 1, 1};
    double point1[2] = {0.1, 0.1};
    double point2[2] = {0.7, 0.7};
    adt.store(10, point1);
    adt.store(11, point2);

    REQUIRE(2 == adt.size());

    std::vector<int> inside;
    inside = adt.retrieve(extent);

    REQUIRE(2 == inside.size());

    extent[0] = 0;
    extent[1] = 0;
    extent[2] = 0.5;
    extent[3] = 0.5;
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(10 == inside[0]);

    extent[0] = 0.5;
    extent[1] = 0.5;
    extent[2] = 1;
    extent[3] = 1;
    inside = adt.retrieve(extent);
    REQUIRE(1 == inside.size());
    REQUIRE(11 == inside[0]);
}

TEST_CASE("Adt, Store3DExtents") {
    Adt<6> adt;

    double extent1[6] = {
        .1,
        .1,
        .1,
        .2,
        .2,
        .2,
    };
    double extent2[6] = {
        .7,
        .7,
        .7,
        .9,
        .9,
        .9,
    };
    adt.store(10, extent1);
    adt.store(11, extent2);

    double searchExtent1[6] = {0, 0, 0, .5, .5, .5};
    double searchExtent2[6] = {.5, .5, .5, 1, 1, 1};

    std::vector<int> inside;
    inside = adt.retrieve(searchExtent1);

    REQUIRE(10 == inside[0]);
    REQUIRE(1 == inside.size());

    inside = adt.retrieve(searchExtent2);
    REQUIRE(11 == inside[0]);
    REQUIRE(1 == inside.size());

    double searchExtent3[6] = {0, 0, 0, 1, 1, 1};
    inside = adt.retrieve(searchExtent3);
    REQUIRE(2 == inside.size());
}

TEST_CASE("Adt, CornerCaseStoreMinus10") {
    Adt<3> adt;
    double extent[6] = {0, 0, 0, 1, 1, 1};
    double domain[6] = {0, 0, 0, 0, 0, 0};
    adt.store(-10, domain);
    auto inside = adt.retrieve(extent);

    REQUIRE(1 == inside.size());
}