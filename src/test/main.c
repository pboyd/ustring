#include <stdio.h>

#include "CuTest.h"

CuSuite* utf8_suite();
CuSuite* utf16_suite();

int main(int argc, char **argv) {
        CuString *output = CuStringNew();
        CuSuite* suite = CuSuiteNew();
        
        CuSuiteAddSuite(suite, utf8_suite());
        CuSuiteAddSuite(suite, utf16_suite());
    
        CuSuiteRun(suite);
        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);
        printf("%s\n", output->buffer);
}
