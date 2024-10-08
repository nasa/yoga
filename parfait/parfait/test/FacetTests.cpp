
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
#include <parfait/Facet.h>
#include <RingAssertions.h>

using namespace Parfait;

TEST_CASE("Facet, Exists") {
    Point<double> a{0, 1, 2};
    Point<double> b{3, 4, 5};
    Point<double> c{6, 7, 8};

    Facet g(a, b, c);
}

TEST_CASE("Facet, FacetRayIntersection") {
    Facet f({0, 0, 0}, {.5, 0, 0}, {0, .5, 0});

    Point<double> lo{0.25, 0.25, 1};
    Point<double> hi{0.25, 0.25, -1};

    Point<double> intersect{};
    REQUIRE(f.WhereDoesEdgeIntersect(lo, hi, intersect));

    REQUIRE(0.25 == intersect[0]);
    REQUIRE(0.25 == intersect[1]);
    REQUIRE(0.00 == intersect[2]);
}

TEST_CASE("Facet edge parameter") {
    Facet f({0, 0, 0}, {.5, 0, 0}, {0, .5, 0});

    Point<double> lo{0.25, 0.25, 1};
    Point<double> hi{0.25, 0.25, -1};

    double t;
    REQUIRE(f.calcEdgeIntersection(lo, hi, t));
    REQUIRE(t == Approx(0.5));
}

TEST_CASE("Facet, NormalVector") {
    Facet f({0, 0, 0}, {.5, 0, 0}, {0, .5, 0});

    auto normal = f.computeNormal();
    REQUIRE(0 == normal[0]);
    REQUIRE(0 == normal[1]);
    REQUIRE(1 == normal[2]);
}

TEST_CASE("Facet, get closest point") {
    Facet f({0, 0, 0}, {.5, 0, 0}, {0, .5, 0});

    Point<double> p = {0, 0, 0};

    auto closest = f.getClosestPoint(p);
    double dist = (closest - p).magnitude();
    REQUIRE(dist == Approx(0));
}

TEST_CASE("Facet intersects with Extent") {
    {
        Facet f({0, 0, .4}, {.5, 0, 0.4}, {0, .5, 0.4});
        Parfait::Extent<double> e = {{0, 0, 0}, {1, 1, 1}};
        REQUIRE(f.intersects(e));
    }
    {
        Facet f({0, 0.4, 0}, {.5, .4, 0}, {0, 0.4, 0.5});
        Parfait::Extent<double> e = {{0, 0, 0}, {1, 1, 1}};
        REQUIRE(f.intersects(e));
    }
    {
        Facet f({0, 0.4, 1.1}, {.5, .4, 1.1}, {0, 0.4, 1.1});
        Parfait::Extent<double> e = {{0, 0, 0}, {1, 1, 1}};
        REQUIRE(not f.intersects(e));
    }
}

TEST_CASE("Facet facet intersection") {
    {
        Facet f1({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
        Facet f2({0, 0, 1}, {1, 0, 1}, {0, 1, 1});
        REQUIRE(not f1.intersects(f2));
    }
    {
        Facet f1({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
        Facet f2({0, 1, -.1}, {0, 0, -.1}, {1, 0, 1});
        REQUIRE(f1.intersects(f2));
    }
}

TEST_CASE("Aspect ratio") {
    double h = sqrt(0.75);
    Facet f({0, 0, 0}, {1, 0, 0}, {.5, h, 0});
    REQUIRE(f.aspectRatio() == Approx(1.0));
}
