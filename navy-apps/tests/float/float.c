#include "FLOAT.h"

#include <stdio.h>
#include <unistd.h>

float F2float(FLOAT F_val) { return (float)F_val / (1 << 16); }

int main()
{
    // Test values
    float f_a = 1.2f;
    float f_b = 5.6f;
    float f_c = -1.2f;

    printf("FLOAT Test Program\n");

    // Convert to FLOAT
    FLOAT F_a = f2F(f_a);
    FLOAT F_b = f2F(f_b);
    FLOAT F_c = f2F(f_c);

    printf("--- Value Representations ---\n");
    printf("Float a: %f, FLOAT A: %d (0x%x)\n", f_a, F_a, F_a);
    printf("Expected for 1.2: 78643 (0x13333)\n\n");

    printf("Float b: %f, FLOAT B: %d (0x%x)\n", f_b, F_b, F_b);
    printf("Expected for 5.6: 367001 (0x59999)\n\n");

    printf("Float c: %f, FLOAT C: %d (0x%x)\n", f_c, F_c, F_c);
    printf("Expected for -1.2: -78643 (0xfffecccd for 2's complement of 0x13333)\n\n");

    // Test Multiplication
    printf("--- Multiplication Test (a * b) ---\n");
    float real_mul_res    = f_a * f_b;
    FLOAT F_mul_res       = F_mul_F(F_a, F_b);
    float F_mul_res_float = F2float(F_mul_res);
    float mul_error       = fabsf(real_mul_res - F_mul_res_float);

    printf("Real float: %f * %f = %f\n", f_a, f_b, real_mul_res);
    printf("FLOAT     : %d * %d = %d (represents %f)\n", F_a, F_b, F_mul_res, F_mul_res_float);
    printf("Error     : %f\n\n", mul_error);

    // Test Division
    printf("--- Division Test (b / a) ---\n");
    float real_div_res    = f_b / f_a;
    FLOAT F_div_res       = F_div_F(F_b, F_a);
    float F_div_res_float = F2float(F_div_res);
    float div_error       = fabsf(real_div_res - F_div_res_float);

    printf("Real float: %f / %f = %f\n", f_b, f_a, real_div_res);
    printf("FLOAT     : %d / %d = %d (represents %f)\n", F_b, F_a, F_div_res, F_div_res_float);
    printf("Error     : %f\n\n", div_error);

    // Test Addition (F_add_F is not defined, let's use int arithmetic for scaled integers)
    // Note: Direct addition of FLOATs is equivalent to adding the scaled integers.
    // F_add_F(A,B) would simply be A + B.
    printf("--- Addition Test (a + b) ---\n");
    float real_add_res    = f_a + f_b;
    FLOAT F_add_res       = F_a + F_b;  // Direct addition for fixed-point
    float F_add_res_float = F2float(F_add_res);
    float add_error       = fabsf(real_add_res - F_add_res_float);

    printf("Real float: %f + %f = %f\n", f_a, f_b, real_add_res);
    printf("FLOAT     : %d + %d = %d (represents %f)\n", F_a, F_b, F_add_res, F_add_res_float);
    printf("Error     : %f\n\n", add_error);

    // Test Subtraction (F_sub_F is not defined, let's use int arithmetic for scaled integers)
    // Note: Direct subtraction of FLOATs is equivalent to subtracting the scaled integers.
    // F_sub_F(A,B) would simply be A - B.
    printf("--- Subtraction Test (b - a) ---\n");
    float real_sub_res    = f_b - f_a;
    FLOAT F_sub_res       = F_b - F_a;  // Direct subtraction for fixed-point
    float F_sub_res_float = F2float(F_sub_res);
    float sub_error       = fabsf(real_sub_res - F_sub_res_float);

    printf("Real float: %f - %f = %f\n", f_b, f_a, real_sub_res);
    printf("FLOAT     : %d - %d = %d (represents %f)\n", F_b, F_a, F_sub_res, F_sub_res_float);
    printf("Error     : %f\n\n", sub_error);

    // Test Fabs
    printf("--- Fabs Test (Fabs(c)) ---\n");
    float real_abs_c    = fabsf(f_c);
    FLOAT F_abs_c       = Fabs(F_c);
    float F_abs_c_float = F2float(F_abs_c);
    float abs_error     = fabsf(real_abs_c - F_abs_c_float);

    printf("Real float: fabsf(%f) = %f\n", f_c, real_abs_c);
    printf("FLOAT     : Fabs(%d) = %d (represents %f)\n", F_c, F_abs_c, F_abs_c_float);
    printf("Error     : %f\n\n", abs_error);

    // Test with an integer
    printf("--- Integer Conversion Test ---\n");
    int   val_int      = 10;
    FLOAT F_val_int    = int2F(val_int);
    int   val_int_back = F2int(F_val_int);
    printf("Original int: %d\n", val_int);
    printf("int2F(%d)   : %d (represents %f)\n", val_int, F_val_int, F2float(F_val_int));
    printf("F2int(%d)   : %d\n", F_val_int, val_int_back);
    printf("Error (int2F then F2float vs original): %f\n", fabsf((float)val_int - F2float(F_val_int)));
    printf("Error (int2F then F2int vs original)  : %d\n\n", val_int - val_int_back);

    // Test F_mul_int
    printf("--- F_mul_int Test (a * 5) ---\n");
    int   multiplier          = 5;
    float real_mul_int_res    = f_a * multiplier;
    FLOAT F_mul_int_res       = F_mul_int(F_a, multiplier);
    float F_mul_int_res_float = F2float(F_mul_int_res);
    float mul_int_error       = fabsf(real_mul_int_res - F_mul_int_res_float);

    printf("Real float: %f * %d = %f\n", f_a, multiplier, real_mul_int_res);
    printf("FLOAT     : F_mul_int(%d, %d) = %d (represents %f)\n", F_a, multiplier, F_mul_int_res, F_mul_int_res_float);
    printf("Error     : %f\n\n", mul_int_error);

    // Test F_div_int
    printf("--- F_div_int Test (b / 2) ---\n");
    int   divisor             = 2;
    float real_div_int_res    = f_b / divisor;
    FLOAT F_div_int_res       = F_div_int(F_b, divisor);
    float F_div_int_res_float = F2float(F_div_int_res);
    float div_int_error       = fabsf(real_div_int_res - F_div_int_res_float);

    printf("Real float: %f / %d = %f\n", f_b, divisor, real_div_int_res);
    printf("FLOAT     : F_div_int(%d, %d) = %d (represents %f)\n", F_b, divisor, F_div_int_res, F_div_int_res_float);
    printf("Error     : %f\n\n", div_int_error);

    return 0;
}
