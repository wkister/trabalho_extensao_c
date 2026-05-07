#include "../include/statistics.h"

#include <math.h>

int stats_correlation_av1_nf_matii(const PivotTable *table, double *correlation, int *pair_count) {
    if (!table || !correlation || !pair_count) return -1;

    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xy = 0.0;
    double sum_x2 = 0.0;
    double sum_y2 = 0.0;
    int n = 0;

    for (int i = 0; i < table->count; i++) {
        const PivotRow *row = &table->rows[i];

        if (row->av1 >= 0.0 && row->nf_matii >= 0.0) {
            double x = row->av1;
            double y = row->nf_matii;

            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
            sum_y2 += y * y;
            n++;
        }
    }

    *pair_count = n;

    if (n < 2) {
        return -1;
    }

    double numerator = (n * sum_xy) - (sum_x * sum_y);
    double denominator_left = (n * sum_x2) - (sum_x * sum_x);
    double denominator_right = (n * sum_y2) - (sum_y * sum_y);

    if (denominator_left <= 0.0 || denominator_right <= 0.0) {
        return -1;
    }

    *correlation = numerator / sqrt(denominator_left * denominator_right);
    return 0;
}
