
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
#include <string>

namespace Parfait {
namespace LinearPartitioner {

    template <typename T>
    Range<T>::Range(T start_in, T end_in) : start(start_in), end(end_in) {}

    inline OwnerLocalIdPair::OwnerLocalIdPair(long owner_in, long id_in) : owner(owner_in), id(id_in) {}

    inline long computeStartGivenLeftOvers(long id, long work_per_worker, long left_overs) {
        long start;
        if (id <= left_overs)
            start = id * (work_per_worker + 1);
        else {
            long add = left_overs * (work_per_worker + 1);
            start = (id - left_overs) * (work_per_worker) + add;
        }
        return start;
    }
    inline void throwIfIdInvalid(long id, long numIds) {
        if (id >= numIds or id < 0)
            throw std::logic_error("Requested partitioning information of invalid Id: " + std::to_string(id));
    }
    inline Range<long> getRangeForWorker(long worker_id, long numWorkItems, long numWorkers) {
        throwIfIdInvalid(worker_id, numWorkers);
        long left_overs = numWorkItems % numWorkers;
        long work_per_worker = numWorkItems / numWorkers;
        long start = computeStartGivenLeftOvers(worker_id, work_per_worker, left_overs);
        long end = computeStartGivenLeftOvers(worker_id + 1, work_per_worker, left_overs);
        return {start, end};
    }

    inline long getWorkerOfWorkItem(long item_id, long numWorkItems, long numWorkers) {
        throwIfIdInvalid(item_id, numWorkItems);
        long num_left_workers = numWorkItems % numWorkers;
        long work_items_per_worker = numWorkItems / numWorkers;
        long num_work_items_left = (work_items_per_worker + 1) * num_left_workers;
        if (item_id < num_work_items_left) return item_id / (work_items_per_worker + 1);
        item_id -= num_work_items_left;
        return item_id / work_items_per_worker + num_left_workers;
    }

    inline OwnerLocalIdPair getOwnerLocalIdPairOfGlobalItem(long globalItemId, long numWorkItems, long numWorkers) {
        auto owner = getWorkerOfWorkItem(globalItemId, numWorkItems, numWorkers);
        return {owner, getLocalIdOfItemOnWorker(globalItemId, owner, numWorkItems, numWorkers)};
    }

    inline long getLocalIdOfItemOnWorker(long globalItemId, long workerId, long numWorkItems, long numWorkers) {
        auto range = getRangeForWorker(workerId, numWorkItems, numWorkers);
        return globalItemId - range.start;
    }

    template <typename T>
    bool Range<T>::owns(T id) const {
        return id >= start and id < end;
    }

    inline std::vector<long> getOwnersOfEveryWorkItem(long num_work_items, long num_workers) {
        std::vector<long> ranges(num_workers + 1);
        for (int proc = 0; proc < num_workers; proc++) {
            ranges[proc] = Parfait::LinearPartitioner::getRangeForWorker(proc, num_work_items, num_workers).start;
        }
        ranges.back() = Parfait::LinearPartitioner::getRangeForWorker(num_workers - 1, num_work_items, num_workers).end;
        return ranges;
    }
}
}
