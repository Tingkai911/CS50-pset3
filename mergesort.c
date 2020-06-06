#include <cs50.h>
#include <stdio.h>

void mergeSort (int arr[], int left_index, int right_index);

int main (void)
{
    int arr[10] = {46, 27, 96, 1000, 11, -90, 99, 63, 20, 56};

    mergeSort(arr,0,9);

    for (int i = 0; i<10; i++)
    {
        printf("%d, ", arr[i]);
    }

    printf("\n");

}

// O(nlog n) and Omega(nlog n) => theta (nlog n)
// WORKING
void mergeSort (int arr[], int left_index, int right_index)
{
    // return case
    if(left_index == right_index)
    {
        return;
    }

    int middle_index = (left_index+right_index)/2;

    // recursive case
    mergeSort(arr, left_index, middle_index);
    mergeSort(arr, middle_index + 1, right_index);

    // temp arrays for left half and right half
    int n1 = middle_index - left_index + 1;
    int temp_left_arr[n1];
    int n2 = right_index - middle_index;
    int temp_right_arr[n2];
    for (int i = 0; i<n1; i++)
        temp_left_arr[i] = arr[left_index + i];
    for (int j = 0; j<n2; j++)
        temp_right_arr[j] = arr[middle_index + j + 1];

    // comparing and sorting
    int i = 0;
    int j = 0;
    int k = left_index;

    while (i < n1 && j < n2)
    {
        if(temp_left_arr[i] <= temp_right_arr[j])
        {
            arr[k] = temp_left_arr[i];
            i++;
        }
        else
        {
            arr[k] = temp_right_arr[j];
            j++;
        }
        k++;
    }

    // copy remaining elements in temp_left_arr if any
    while (i < n1)
    {
        arr[k] = temp_left_arr[i];
        i++;
        k++;
    }

    // copy remaining elements in temp_right_arr if any
    while (j < n2)
    {
        arr[k] = temp_right_arr[j];
        j++;
        k++;
    }

}