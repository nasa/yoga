
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

#include <parfait/STL.h>
#include <parfait/STLFactory.h>

using namespace Parfait;

TEST_CASE("STLTests, findDomain") {
    auto facets = STLFactory::getUnitCube();
    STL::scaleToUnitLength(facets);
    auto domain = STL::findDomain(facets);
    REQUIRE(-0.5 == Approx(domain.lo[0]));
    REQUIRE(0.5 == Approx(domain.hi[0]));
}

TEST_CASE("STL scaled") {
    Parfait::Point<double> p1 = {0, 0, 0};
    Parfait::Point<double> p2 = {1, 0, 0};
    Parfait::Point<double> p3 = {1, 1, 0};

    std::vector<Facet> facets;
    facets.emplace_back(p1, p2, p3);
    STL::rescale(10, facets);
    REQUIRE(facets[0].points[0][0] == Approx(0.0));
    REQUIRE(facets[0].points[2][0] == Approx(10.0));
}
