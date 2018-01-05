#ifndef RAND_RANGE_H
#define RAND_RANGE_H

class RandRange {
  public:
    static double Double(double min, double max) {
			return min + (((double)rand() / (double)RAND_MAX) * (max - min));
		}
    
    static float Float(float min, float max) {
			return min + (((float)rand() / (float)RAND_MAX) * (max - min));
		}

    static int Int(int min, int max) {
			return ((int)rand() % (max - min + 1)) + min;
		}
};
#endif
