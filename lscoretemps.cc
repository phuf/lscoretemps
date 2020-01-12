#include <iostream>
#include <sensors/sensors.h>

int main() {
    sensors_init(NULL);
    sensors_chip_name const * cn;
    int c = 0;

    while ((cn = sensors_get_detected_chips(0, &c)) != 0) {
        if (cn->bus.type != SENSORS_BUS_TYPE_ISA) {
            continue;
        }

        sensors_feature const *feat;
        int f = 0;

        while ((feat = sensors_get_features(cn, &f)) != 0) {
            std::cout << sensors_get_label(cn, feat) << ": ";

            sensors_subfeature const *subf;
            int s = 0;
            double temp_input = 0;
            double temp_max = 0;
            double temp_crit = 0;

            while ((subf = sensors_get_all_subfeatures(cn, feat, &s)) != 0) {
                if (subf->flags & SENSORS_MODE_R) {
                    double* value = nullptr;

                    switch (subf->type) {
                        case SENSORS_SUBFEATURE_TEMP_INPUT :
                            value = &temp_input;
                            break;

                        case SENSORS_SUBFEATURE_TEMP_MAX:
                            value = &temp_max;
                            break;

                        case SENSORS_SUBFEATURE_TEMP_CRIT:
                            value = &temp_crit;
                            break;
                    }

                    if (value) { 
                        int rc = sensors_get_value(cn, subf->number, value);
                        if (rc < 0) {
                            std::cerr << "err: " << rc;
                        }
                    }
                }
            }
                    std::cout << temp_input << " ( high = " << temp_max << ", crit = " << temp_crit << ")" << std::endl;
        }
    }    

    sensors_cleanup();
}
