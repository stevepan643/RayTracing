#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct interval {
  double min, max;
} interval;

interval interval_from(double min, double max) { return (interval){min, max}; }
double interval_size(interval in) { return in.max - in.min; }
bool interval_caontains(interval in, double x) {
  return in.min <= x && x <= in.max;
}
bool interval_surrounds(interval in, double x) {
  return in.min < x && x < in.max;
}
double interval_clamp(interval in, double x) {
  if (x < in.min)
    return in.min;
  if (x > in.max)
    return in.max;
  return x;
}

#endif
