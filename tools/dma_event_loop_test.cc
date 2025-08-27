// Standard C++ includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cassert>

// Google test includes
#include "gtest/gtest.h"

namespace
{
// Prefetch or process, neuron, row
using Trace = std::vector<std::tuple<bool, uint32_t, uint32_t>>;

auto makeTrace(bool prefetch, uint32_t spikeID, uint32_t row)
{
    return std::make_tuple(prefetch, spikeID, row);
}

void generateSpikeWords(const std::vector<uint32_t> &ids, std::vector<uint32_t> &words)
{
    for(uint32_t i : ids) {
        assert((i / 32) < words.size());
        words[i / 32] |= (1 << (i % 32));
    }
    
    for(uint32_t w : words) {
        std::cout << w << ", ";
    }
    std::cout << std::endl;
}

void prefetch(uint32_t idPre, uint32_t row, Trace &trace)
{
    trace.emplace_back(true, idPre, row);
}

void process(uint32_t idPre, uint32_t row, Trace &trace)
{
    trace.emplace_back(false, idPre, row);
}

void printTrace(const Trace &trace)
{
    for(const auto &t : trace) {
        std::cout << std::get<1>(t) << " - " << std::get<2>(t) << "(" << (std::get<0>(t) ? "prefetch" : "process") << ")" << std::endl;
    }
}

bool validateTrace(const std::vector<uint32_t> &spikeIDs, const Trace &trace, uint32_t numRows)
{
    std::unordered_map<uint32_t, std::vector<std::pair<bool, bool>>> spikeIDProcessed;
    for(auto s : spikeIDs) {
        spikeIDProcessed[s].resize(numRows, std::make_pair(false, false));
    }
    
    bool success = true;
    for(const auto &t : trace) {
        if(std::get<2>(t) >= numRows) {
            std::cerr << "Invalid row:" << std::get<2>(t) << " found in trace" << std::endl;
            success = false;
            continue;
        }
        
        // Search for spike ID
        auto idIter = spikeIDProcessed.find(std::get<1>(t));
        if(idIter == spikeIDProcessed.cend()) {
            std::cerr << "Unknown spike ID:" << std::get<1>(t) << " found in trace" << std::endl;
            success = false;
            continue;
        }

        // If prefetch
        auto &state = idIter->second.at(std::get<2>(t));
        if(std::get<0>(t)) {
            // If row has already been prefetched
            if(state.first) {
                std::cerr << "Spike ID:" << std::get<1>(t) << " prefetched multiple times in trace" << std::endl;
                success = false;
                continue;
            }
            if(state.second) {
                std::cerr << "Spike ID:" << std::get<1>(t) << " processed before it is prefetched" << std::endl;
                success = false;
                continue;
            }
            
            // Mark row as prefetched
            state.first = true;
        }
        // Process
        else {
            // If row hasn't been prefetched
            if(!state.first) {
                std::cerr << "Spike ID:" << std::get<1>(t) << " not prefetched before it's processed" << std::endl;
                success = false;
                continue;
            }
            if(state.second) {
                std::cerr << "Spike ID:" << std::get<1>(t) << " processed multiple times in trace" << std::endl;
                success = false;
                continue;
            }
            
            // Mark row as processed
            state.second = true;
        }
    }
    
    for(const auto &s : spikeIDProcessed) {
        for(const auto &r : s.second) {
            if(!r.first) {
                std::cerr << "Spike ID:" << s.first << " never prefetched" << std::endl;
                success = false;
            }
            if(!r.second) {
                std::cerr << "Spike ID:" << s.first << " never processed" << std::endl;
                success = false;
            }
        }
    }
    
    return success;
}

void propagate(const uint32_t *spikeWord, const uint32_t *spikeWordEnd, uint32_t numRows, Trace &trace)
{
    // Find first spike to prefetch
    uint32_t currentSpikeWord;
    uint32_t idPre;
    uint32_t wordStartID = 31;
    uint32_t prevIdPre;
    while(spikeWord != spikeWordEnd) {
        // Get next spike word
        currentSpikeWord = *spikeWord++;

        // Reset neuron ID
        idPre = wordStartID;
        wordStartID += 32;
       
        // If spike word contains any spikes
        if(currentSpikeWord != 0) {
            // Get ID of first spike in word
            uint32_t numLZ =__builtin_clz(currentSpikeWord);

            // If all bits have now been processed, zero spike word
            // Otherwise shift past the spike we have found
            currentSpikeWord = (numLZ == 31) ? 0 : (currentSpikeWord << (numLZ + 1));
            
            // Update idPre
            idPre -= numLZ;
            
            // Prefetch first row corresponding to spike and stop
            prevIdPre = idPre;
            prefetch(idPre, 0, trace);
            
            idPre--;
            break;
        }
    }

    // If we have preprocessed all spike words i.e. there are no spikes, stop
    if(spikeWord == spikeWordEnd) {
        return;
    }
    
    // Keep looping through spike words
    do {
        // While bits remain
        while(currentSpikeWord != 0) {
            // Loop through all but last row
            for(uint32_t r = 0; r < (numRows - 1); r++) {
                // Prefetch next row
                prefetch(prevIdPre, r + 1, trace);
                
                // Process this row
                process(prevIdPre, r, trace);
            }
    
            // Calculate leading zeros
            const int numLZ = __builtin_clz(currentSpikeWord);
        
            // If all bits have now been processed, zero spike word
            // Otherwise shift past the spike we have found
            currentSpikeWord = (numLZ == 31) ? 0 : (currentSpikeWord << (numLZ + 1));
        
            // Subtract number of leading zeros from neuron ID
            idPre -= numLZ;
        
            // Prefetch first row of new spike
            prefetch(idPre, 0, trace);
            
            // Process last row of previous spike
            process(prevIdPre, numRows - 1, trace);
            
            // Update previous spike
            prevIdPre = idPre;
            
            // New neuron id of the highest bit of this word
            idPre--;
        }
        
        // Get next word and calculate it's starting index
        currentSpikeWord = *spikeWord++;

        // Reset neuron ID
        idPre = wordStartID;
        wordStartID += 32;
    } while(spikeWord <= spikeWordEnd);
    
    // Loop through all but last row
    for(uint32_t r = 0; r < (numRows - 1); r++) {
        // Prefetch next row
        prefetch(prevIdPre, r + 1, trace);
        
        // Process this row
        process(prevIdPre, r, trace);
    }
    
    // Process last row of previous spike
    process(prevIdPre, numRows - 1, trace);
}
}

class PropagateTest : public testing::TestWithParam<uint32_t> 
{
  // You can implement all the usual fixture class members here.
  // To access the test parameter, call GetParam() from class
  // TestWithParam<T>.
};

TEST_P(PropagateTest, NoSpikes)
{
    std::vector<uint32_t> spikeWords(4, 0);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);
    
    if(!trace.empty()) {
        printTrace(trace);
        FAIL();
    }
}

TEST_P(PropagateTest, AllSpikes)
{
    std::vector<uint32_t> spikeIDs(4 * 32);
    std::iota(spikeIDs.begin(), spikeIDs.end(), 0);
    
    std::vector<uint32_t> spikeWords(4);
    generateSpikeWords(spikeIDs, spikeWords);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);
    
    if(!validateTrace(spikeIDs, trace, GetParam())) {
        printTrace(trace);
        FAIL();
    }
}

TEST_P(PropagateTest, EmptyFirstWord)
{
    std::vector<uint32_t> spikeIDs(3 * 32);
    std::iota(spikeIDs.begin(), spikeIDs.end(), 32);
    
    std::vector<uint32_t> spikeWords(4);
    generateSpikeWords(spikeIDs, spikeWords);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);
    
    if(!validateTrace(spikeIDs, trace, GetParam())) {
        printTrace(trace);
        FAIL();
    }
}

TEST_P(PropagateTest, OneFirstWord)
{
    std::vector<uint32_t> spikeIDs(3 * 32);
    std::iota(spikeIDs.begin(), spikeIDs.end(), 32);
    spikeIDs.insert(spikeIDs.begin(), 0);
    
    std::vector<uint32_t> spikeWords(4);
    generateSpikeWords(spikeIDs, spikeWords);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);    
    
    if(!validateTrace(spikeIDs, trace, GetParam())) {
        printTrace(trace);
        FAIL();
    }
}


TEST_P(PropagateTest, EmptyOneFirstWord)
{
    std::vector<uint32_t> spikeIDs(2 * 32);
    std::iota(spikeIDs.begin(), spikeIDs.end(), 64);
    spikeIDs.insert(spikeIDs.begin(), 32);
    
    std::vector<uint32_t> spikeWords(4);
    generateSpikeWords(spikeIDs, spikeWords);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);    
    
    if(!validateTrace(spikeIDs, trace, GetParam())) {
        printTrace(trace);
        FAIL();
    }
}

TEST_P(PropagateTest, EmptyMiddle)
{
    std::vector<uint32_t> spikeIDs(3 * 32);
    std::iota(spikeIDs.begin(), spikeIDs.begin() + 64, 0);
    std::iota(spikeIDs.begin() + 64, spikeIDs.end(), 96);
    
    std::vector<uint32_t> spikeWords(4);
    generateSpikeWords(spikeIDs, spikeWords);
    
    Trace trace;
    propagate(spikeWords.data(), spikeWords.data() + spikeWords.size(), GetParam(), trace);
    
    if(!validateTrace(spikeIDs, trace, GetParam())) {
        printTrace(trace);
        FAIL();
    }
}

INSTANTIATE_TEST_SUITE_P(RowCounts,
                         PropagateTest,
                         testing::Values(1, 2, 3));
