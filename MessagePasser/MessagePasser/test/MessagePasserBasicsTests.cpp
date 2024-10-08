
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#include "../MessagePasser.h"
#include <RingAssertions.h>

TEST_CASE("MessagePasser basic stuff"){
    MessagePasser mp(MPI_COMM_WORLD);

    REQUIRE(MPI_INT == mp.Type(int()));
    REQUIRE(MPI_FLOAT == mp.Type(float()));
    REQUIRE(MPI_DOUBLE == mp.Type(double()));
    REQUIRE(MPI_LONG == mp.Type(long()));

    int myRank=0;
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    REQUIRE(myRank == mp.Rank());

    int size=0;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    REQUIRE(size == mp.NumberOfProcesses());

    // not sure how to test barrier...but it works...promise
    mp.Barrier();

    auto status = mp.NonBlockingBarrier();
    status.wait();
    REQUIRE(status.isComplete());
}



