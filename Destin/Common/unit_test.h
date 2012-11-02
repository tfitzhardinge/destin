#ifndef unit_test_h
#define unit_test_h

#include <stdbool.h>
#include <stdio.h>

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>


int TEST_HAS_FAILURES = false; //checked at the end to determine if any tests have failed

#define RUN( f ){                           \
    printf("Running " #f "\n");             \
    int r = f();                            \
    if(r != 0){                             \
        TEST_HAS_FAILURES = true;           \
        printf("FAILED\n" );                \
    }else{                                  \
        printf("\n");                       \
    }                                       \
}                                           \

#define assertTrue( E )\
{\
    if(!( E )  ){\
        printf("assertTrue FAILED, line: %i, expression: " #E "\n", __LINE__);\
        return 1;\
    }\
}\

#define assertFalse( E)\
{\
    if( ( E )  ){\
        printf("assertFalse FAILED, line: %i, expression: " #E "\n", __LINE__);\
        return 1;\
    }\
}\

#define assertFloatEquals( expected, actual, epsilon ){\
    if( fabs(expected - actual ) > epsilon  ){\
        printf("assertFloatEquals FAILED, line: %i, expected: %f, actual: %f, difference: %e\n", __LINE__, expected, actual, (expected - actual));\
        return 1;\
    }\
}\

bool _assertFloatArrayEquals(float * expected, float * actual, int length, double epsilon, int line){
    int i;
    if(length <= 0 ){
        printf("assertFloatArrayEquals FAILED, line: %i, negative or zero array length: %i", line, length);
        return false;
    }

    for(i = 0 ; i < length ; i++){
        if(fabs( expected[i] - actual[i]) > epsilon ){
            printf("assertFloatArrayEquals FAILED, line: %i, on index %i with array length %i\n", line, i, length );
            printf("expected: %f, actual: %f, difference: %e\n", expected[i], actual[i], expected[i] - actual[i]);
            return false;
        }
    }
    return true;
}

bool _assertIntArrayEquals(int * expected, int * actual, int length, int line){
    int i;
    if(length <= 0 ){
        printf("assertIntArrayEquals FAILED, line: %i, negative or zero array length: %i", line, length);
        return false;
    }

    for(i = 0 ; i < length ; i++){
        if( expected[i] != actual[i] ){
            printf("assertIntArrayEquals FAILED, line: %i, on index %i with array length %i\n", line, i, length );
            printf("expected: %i, actual: %i\n", expected[i], actual[i]);
            return false;
        }
    }
    return true;
}


#define assertFloatArrayEquals( exp, act, len ){\
    if( ! _assertFloatArrayEquals( (exp), (act), (len), 0.0, __LINE__ ) ){\
        return 1;\
    }\
}\

/** Fills dest array with caller's of this macro recieved arguments
 * last_fixed_argument - plain text of the last fixed argument passed to
 * the caller function, which will not be put into dest array
 * length - length of the arguments array
 */
#define ut_varags_to_array( dest, last_fixed_argument, length, the_type ){\
    va_list arg_list;\
    va_start(arg_list, last_fixed_argument);\
    int i;\
    for ( i = 0; i < length; i++ ){\
        dest[i] = va_arg(arg_list, the_type);\
    }\
    va_end(arg_list);\
}\

/** same as assertFloatArrayEquals but asserts true if difference is less than epsilon
 */
#define assertFloatArrayEqualsE( exp, act, len, epsilon ){\
    if( ! _assertFloatArrayEquals( (exp), (act), (len), epsilon, __LINE__ ) ){\
        return 1;\
    }\
}\

bool _assertFloatArrayEqualsEV(float *actual, float epsilon, int len, int line, ...){
    float expected[len];
    ut_varags_to_array(expected, line, len, double);
    return _assertFloatArrayEquals(expected, actual, len, epsilon, line );
}



#define assertFloatArrayEqualsEV( act, epsilon, len, args... ){\
    if(! _assertFloatArrayEqualsEV(act, epsilon, len, __LINE__, args )){\
        return 1;\
    }\
}\

bool _assertIntArrayEqualsEV(int *actual, int len, int line, ...){
    int expected[len];
    ut_varags_to_array(expected, line, len, int);
    return _assertIntArrayEquals(expected, actual, len, line );
}

#define assertIntArrayEqualsEV( act, len, expecteds... ){\
    if(! _assertIntArrayEqualsEV(act, len, __LINE__, expecteds )){\
        return 1;\
    }\
}\

void printFloatArray(float * array, int length){
    int i;
    printf("float array: ");
    for(i = 0; i < length - 1 ; i++){
        printf("%i: %f, ", i,array[i]);
    }
    printf("%i: %f\n",i,array[i]);
}


/** assigns the dest array the float values pass as arguments
 * CAUTION: Be sure to write float constants. To pass 1 for example, write 1.0
 * and not 1 or they will be skipped or other errors may occur
 *
 * @length - number of float elements passed in.
 */
void assignFloatArray(float * dest, int length, ...){

    ut_varags_to_array(dest, length, length, double);
}

/** converts arguments into an array which is returned.
 *
 * CAUTION: Be sure to write float constants. To pass 1 for example, write 1.0
 * and not 1 or they will be skipped or other errors may occur
 *
 * argc: how many arguments passed
 *
 * user takes ownership of array.
 */
float * toFloatArray(long c, ...) {

    //compiler note: ‘float’ is promoted to ‘double’ when passed through ‘...’
    //so have to start with double then cast to float
    float * a = (float *)malloc(c * sizeof(float));

    if(a==NULL){
        printf("toArray error\n");
        exit(1);
    }

    ut_varags_to_array(a, c, c, double);

    return a;
}

bool _assertIntEquals( int expected, int actual, int line){
    if( expected == actual ){
        return true;
    }else{
        printf("assertIntEquals FAILED, line %i, expected: %i, actual: %i\n", line, expected, actual );
        return false;
    }

}

#define assertIntEquals( expected, actual){\
    if( ! _assertIntEquals(expected, actual, __LINE__ )){\
        return 1;\
    }\
}\


#endif
