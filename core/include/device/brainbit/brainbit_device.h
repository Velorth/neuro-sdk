/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BRAINBIT_SOURCE_H
#define BRAINBIT_SOURCE_H

#include <condition_variable>
#include <atomic>
#include <map>
#include <list>
#include "device/request_handler.h"
#include "brainbit_command.h"

#define BRAINBIT_SOURCE_CLASSNAME "Brainbit source"

/**
 * BrainBit configuration values and constants
 */
#define BRAINBIT_PACKET_SIZE 20
//#define BRAINBIT_MAX_PACKET_NUMBER USHRT_MAX
#define BRAINBIT_MAX_PACKET_DELAY 10000
#define BRAINBIT_SORT_MAP_SIZE 50 //matches 200 ms delay in output buffers
#define BRAINBIT_T3_CHANNEL_INDEX 0
#define BRAINBIT_O1_CHANNEL_INDEX 1
#define BRAINBIT_T4_CHANNEL_INDEX 2
#define BRAINBIT_O2_CHANNEL_INDEX 3

//Eeg state determination constants
#define RHYTHM_CALC_WINDOW_LENGTH 1250
#define RHYTHM_BASE_NORMAL_COUNT 6u
#define ALPHA_START_FREQ 8.f //Hz
#define ALPHA_STOP_FREQ 14.f //HZ
#define ALPHA_BANDWIDTH (ALPHA_STOP_FREQ - ALPHA_START_FREQ)

#define BETA_START_FREQ 14.f //Hz
#define BETA_STOP_FREQ 34.f //Hz
#define BETA_BANDWIDTH (BETA_STOP_FREQ - BETA_START_FREQ)

#define THETA_START_FREQ 4.f //Hz
#define THETA_STOP_FREQ 8.f //Hz
#define THETA_BANDWIDTH (THETA_STOP_FREQ - THETA_START_FREQ)

#define RHYTHM_MINIMAL_BANDWIDTH THETA_BANDWIDTH

#define ALPHA_NORMAL_CFC (RHYTHM_MINIMAL_BANDWIDTH/ALPHA_BANDWIDTH)
#define BETA_NORMAL_CFC (RHYTHM_MINIMAL_BANDWIDTH/BETA_BANDWIDTH)
#define THETA_NORMAL_CFC (RHYTHM_MINIMAL_BANDWIDTH/THETA_BANDWIDTH)

//Relax and meditation conditional params
#define RELAX_INCLUDE_BETA_BOTTOM_THRESHOLD 20
#define RELAX_INCLUDE_BETA_TOP_THRESHOLD 100
#define RELAX_INCLUDE_BETA_HITS_COUNT 5

#define MEDIT_INCLUDE_BETA_BOTTOM_THRESHOLD 20
#define MEDIT_INCLUDE_BETA_TOP_THRESHOLD 100
#define MEDIT_INCLUDE_BETA_HITS_COUNT 5

//Eeg state limits
#define PX1 1.3f
#define PX2 1.45f
#define PX3 1.55f
#define PX4 1.7f
#define NX1 .65f
#define NX2 .45f
#define NX3 .2f
#define NX4 .0f
#define PY1 .9f
#define PY2 .85f
#define PY3 .8f
#define PY4 .7f
#define NY1 1.1f
#define NY2 1.15f
#define NY3 1.25f
#define NY4 1.3f

//Eeg state values
//intermediate state values does not have explicit names
//but could be used in different analysis
#define STATE_SLEEP 10
#define STATE_DEEP_RELAX 8
#define STATE_RELAX 4
#define STATE_NEUTRAL 0
#define STATE_NORMAL_ACTIVATION -4
#define STATE_EXCITEMENT -8
#define STATE_DEEP_EXCITEMENT -10

#define STATE_STRESS_HITS_COUNT 10
#define STATE_ATTENTION_HITS_COUNT 10

/**
 * Represents two-byte BrainBit data sample
 * as byte array and short value
 */
typedef union
{
    short shortValue;
    unsigned char bytes[2];
} BrainBitData;

struct EegRhythmValues
{
    double alpha;
    double beta;
    double theta;

    EegRhythmValues& operator+=(const  EegRhythmValues& rhs)
    {
        alpha += rhs.alpha;
        beta += rhs.beta;
        theta += rhs.theta;
        return *this;
    }

    EegRhythmValues& operator+=(const double& k)
    {
        alpha += k;
        beta += k;
        theta += k;
        return *this;
    }

    EegRhythmValues& operator*=(const double& k)
    {
        alpha *= k;
        beta *= k;
        theta *= k;
        return *this;
    }

    EegRhythmValues operator/(const double& k)
    {
        return  EegRhythmValues{alpha/k, beta/k, theta/k};
    }

    EegRhythmValues operator+(const  EegRhythmValues& rhs)
    {
        return  EegRhythmValues{rhs.alpha + alpha, rhs.beta + beta, rhs.theta + theta};
    }

    EegRhythmValues& operator=(const double& v)
    {
        alpha = v;
        beta = v;
        theta = v;
        return *this;
    }
};



#endif
