// Standard C++ includes
#include <algorithm>
#include <iostream>
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

void generateSpikes(const std::vector<uint32_t> &ids, std::vector<uint32_t> &words)
{
    for(uint32_t i : ids) {
        assert((i / 32) < words.size());
        words[i / 32] |= (1 << (i % 32));
    }
}

void prefetch(uint32_t idPre, uint32_t row, Trace &trace)
{
    trace.emplace_back(true, idPre, row);
}

void printTrace(const std::vector<std::tuple<bool, uint32_t>> &trace)
{
    for(const auto &t : trace) {
        std::cout << std::get<1>(t) << " - " << std::get<2> << "(" << (std::get<0>(t) ? "prefetch" : "process") << ")" << std::endl;
    }
}

void propagate(const uint32_t *spikeWord, uint32_t numSpikeWords, std::vector<std::tuple<bool, uint32_t>> &trace)
{
    // Find first spike to prefetch
    uint32_t currentSpikeWord;
    uint32_t idPre;
    uint32_t w = 0;
    for(; w < numSpikeWords; w++) {
        // Get spike word
        currentSpikeWord = spikeWord[w];
        
        // Reset neuron ID
        idPre = (w * 32) + 31;
        
        // If spike word is zero, no spikes at all so advance to next word and keep prefetching
        if(currentSpikeWord == 0) {
            currentSpikeWord = spikeWord[w + 1];
        }
        // Otherwise, if spikeWord == 1 there is one spike
        else if(currentSpikeWord == 1) {
            // Prefetch first row corresponding to spike
            prefetch(idPre - 31, 0, trace);
            
            // Advance to next word and stop searching
            w++;
            if(w < numSpikeWords) {
                currentSpikeWord = spikeWord[w];
            }
            idPre = (w * 32) + 31;
            break;
        }
        // Otherwise
        else {
            // Get ID of first spike in word
            uint32_t numLZ =__builtin_clz(currentSpikeWord);
            currentSpikeWord <<= (numLZ + 1);
            
            // Update idPre
            idPre -= numLZ;
            
            // Prefetch first row corresponding to spike and stop
            prefetch(idPre, 0, trace);
            
            idPre--;
            break;
        }
    }
    
    // Keep looping through spike words
    for(;w < numSpikeWords; w++) {
        // While bits remain
        while(currentSpikeWord != 0) {
            // Calculate leading zeros
            const int numLZ = __builtin_clz(currentSpikeWord);
        
            // If all bits have now been processed, zero spike word
            // Otherwise shift past the spike we have found
            currentSpikeWord = (numLZ == 31) ? 0 : (currentSpikeWord << (numLZ + 1));
        
            // Subtract number of leading zeros from neuron ID
            idPre -= numLZ;
        
            // Prefetch spike
            prefetch(idPre, trace);
            
            
            //process(oldIDPre);
        
            // New neuron id of the highest bit of this word
            idPre--;
        }
        
        // Get next word and calculate it's starting index
        if(w < (numSpikeWords - 1)) {
            currentSpikeWord = spikeWord[w + 1];
        }
        idPre = ((w + 1) * 32) + 31;
    }
}
}

TEST(DMAEventLoop, NoSpikes)
{
    std::vector<uint32_t> spikeWords(4, 0);
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);
    
    ASSERT_TRUE(trace.empty());
}

TEST(DMAEventLoop, AllSpikes)
{
    std::vector<uint32_t> spikeWords(4, 0xFFFFFFFF);
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);
    
    std::sort(trace.begin(), trace.end());
    ASSERT_EQ(trace.size(), 32 * 4);
    for(uint32_t i = 0; i < 32 * 4; i++) {
        ASSERT_EQ(trace[i], std::make_tuple(true, i));
    }
}

TEST(DMAEventLoop, EmptyFirstWord)
{
    std::vector<uint32_t> spikeWords(4, 0);
    std::fill_n(spikeWords.begin() + 1, 3, 0xFFFFFFFF);
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);
    
    std::sort(trace.begin(), trace.end());
    ASSERT_EQ(trace.size(), 32 * 3);
    for(uint32_t i = 0; i < 32 * 3; i++) {
        ASSERT_EQ(trace[i], std::make_tuple(true, i + 32));
    }
}

TEST(DMAEventLoop, OneFirstWord)
{
    std::vector<uint32_t> spikeWords(4, 1);
    std::fill_n(spikeWords.begin() + 1, 3, 0xFFFFFFFF);
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);    
    
    std::sort(trace.begin(), trace.end());
    ASSERT_EQ(trace.size(), (32 * 3) + 1);
    ASSERT_EQ(trace[0], std::make_tuple(true, 0));
    for(uint32_t i = 0; i < 32 * 3; i++) {
        ASSERT_EQ(trace[i + 1], std::make_tuple(true, i + 32));
    }
}

TEST(DMAEventLoop, EmptyOneFirstWord)
{
    std::vector<uint32_t> spikeWords(4, 0);
    spikeWords[1] = 1;
    spikeWords[2] = 0xFFFFFFFF;
    spikeWords[3] = 0xFFFFFFFF;
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);    
    
    std::sort(trace.begin(), trace.end());
    ASSERT_EQ(trace.size(), (32 * 2) + 1);
    ASSERT_EQ(trace[0], std::make_tuple(true, 32));
    for(uint32_t i = 0; i < 32 * 2; i++) {
        ASSERT_EQ(trace[i + 1], std::make_tuple(true, i + 64));
    }
}

TEST(DMAEventLoop, EmptyMiddle)
{
    std::vector<uint32_t> spikeWords(4, 0xFFFFFFFF);
    spikeWords[2] = 0;
    std::vector<std::tuple<bool, uint32_t>> trace;
    
    propagate(spikeWords.data(), spikeWords.size(), trace);
    
    std::sort(trace.begin(), trace.end());
    ASSERT_EQ(trace.size(), 32 * 3);
    for(uint32_t i = 0; i < 32 * 2; i++) {
        ASSERT_EQ(trace[i], std::make_tuple(true, i));
    }
    for(uint32_t i = 0; i < 32; i++) {
        ASSERT_EQ(trace[i + 64], std::make_tuple(true, i + 96));
    }
}
