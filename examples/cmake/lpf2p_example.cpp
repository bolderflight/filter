/*
* Tuan D Luong
* tdluong@crimson.ua.edu
* 
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "filter.h"
#include <array>
#include <iostream>

int main() {
    float x[] = {
    1,
    1.5,
    1,
    0.5,
    -0.1,
    0.1,
    5,
    5.1,
    };

    float cutoff_hz = 5.0f;
    float samp_hz = 100.0f;

    /*
    First implementation follows BFS with separate init and filter method
    */
    bfs::Lpf2p<float> lpf2p_bfs;

    std::cout<<"Second implementation\n";

    // Init
    lpf2p_bfs.Init(cutoff_hz, samp_hz, x[0]);
    std::cout<<x[0]<<"\n"; // This method skip the first val in the loop

    // Apply filter
    for (size_t i = 1; i < (sizeof(x) / sizeof(x[0])); i++){
        float y = lpf2p_bfs.Filter(x[i]);
        std::cout<<y<<"\n";
    }

    /*
    Second implementation follows Ardupilot by just using the filter method
    */

    bfs::Lpf2p<float> lpf2p_ardu;
    std::cout<<"Second implementation\n";
    for (size_t i = 0; i < (sizeof(x) / sizeof(x[0])); i++){
        float y = lpf2p_ardu.Filter(x[i], cutoff_hz, samp_hz);
        std::cout<<y<<"\n";
    }
}