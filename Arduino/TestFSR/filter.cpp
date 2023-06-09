#include "filter.h"

Filter::Filter(float x_init, float T, float LOW_TH, float HIGH_TH, float low_th, float high_th) {
    Filter::x = x_init;
    Filter::T = T;
    Filter::LOW_TH = LOW_TH;
    Filter::HIGH_TH = HIGH_TH;
    Filter::low_th = low_th;
    Filter::high_th = high_th;
    get_threshold_x();
}

float Filter::read(float u, float dt) {
    Filter::x += dt / T * (u - Filter::x);
    return Filter::x;
    // return u;
}

float Filter::get_x() {
    return Filter::x;
}

float Filter::get_threshold_x() {
    if (Filter::x < Filter::low_th) {
        Filter::x_th = Filter::LOW_TH;
    } else if (Filter::x > Filter::high_th) {
        Filter::x_th = Filter::HIGH_TH;
    }
    return Filter::x_th;
}