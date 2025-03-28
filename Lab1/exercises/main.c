#include <stdio.h>
#include <math.h>

double calculate_mean(int arr[], int size) {
    float sum = 0.0;
    // complete
    for(int i = 0; i<size;i++){
        sum+=arr[i];
    }

    return sum/size; // change to return mean
}

double calculate_standard_deviation(int arr[], int size) {
    // complete
    float mean = calculate_mean(arr,size);
    float sum_squared_diff = 0.0;

    for(int i = 0; i < size; i++){
        sum_squared_diff += pow(arr[i]-mean,2);
    }
    float variance = sum_squared_diff/size;

    return sqrt(variance); // change to return standard deviation
}

int main() {
    // Test case 1
    int test_array[] = {1, -2, 3, 4, 5, -299};
    int size = sizeof(test_array) / sizeof(test_array[0]);


    printf("Test Case 1\n");

    // Calculating mean
    double mean = calculate_mean(test_array, size);
    printf("Mean: %.2f\n", mean);

    // Calculating standard deviation
    double std_dev = calculate_standard_deviation(test_array, size);
    printf("Standard Deviation: %.2f\n", std_dev);

    return 0;
}
