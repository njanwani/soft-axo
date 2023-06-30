#ifndef FILTER_H
#define FILTER_h


class Filter {
    public:
        Filter(float x_init, float T, float LOW_VAL, float HIGH_VAL, float low_th, float high_th);
        float read(float u, float dt);
        float get_x();
        float get_threshold_x();

    private:
        float T;
        float x;
        float x_th;
        float LOW_VAL;
        float HIGH_VAL;
        float low_th;
        float high_th;
};

#endif
