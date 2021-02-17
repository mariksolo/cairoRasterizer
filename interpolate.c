// assumes i1 < i2
void interpolate(int i1, int d1, int i2, int d2, float values[]) {

    if (i1 == i2) {
        values[0] = (float) d1;
        return;
    }

    float a = ((float) d1 - d2) / ((float) i1 - i2);
    float d = (float) d1;

    int index = 0;
    for (int i = i1; i <= i2; i++) {
        values[index] = (float) d;
        d += a;
        index++;
    }

}