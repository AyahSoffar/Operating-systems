#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>


int element_no;
int *arr;
void merge(int left, int mid, int right)
{
    int i, j, k;
    int lSize = mid - left + 1;
    int rSize =  right - mid;

    /* create temp arrays */
    int L[lSize], R[rSize];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < lSize; i++)
        L[i] = arr[left + i];
    for (j = 0; j < rSize; j++)
        R[j] = arr[mid + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (j< rSize && i < lSize)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[] and R[], if there
       are any */
    while (j < rSize)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    while (i < lSize)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
}


void* merge_sort(void* args)
{


    int* new= (int*)args;
    int start= new[0];
    int end=new[1];

    pthread_t thread1, thread2;
    int check;

    if(start<end)
    {
        int mid=(end+start)/2;
        int limits[2]= {start, mid};
        check= pthread_create(&thread1, NULL, merge_sort, (void*)limits);
        int limits1[2]= {mid+1, end};
        check= pthread_create(&thread2, NULL, merge_sort, (void*)limits1);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        merge(start, mid, end);


    }
}
void read_input()
{

    FILE * f;
    f=fopen("input2.txt", "r");
    if (f !=NULL)
    {

        while(!feof(f))
        {
            fscanf(f, "%d", &element_no);
            fscanf(f, "\n");

            arr=(int*)malloc(element_no * sizeof(int));

            for(int i=0; i<element_no; i++)
            {
                fscanf(f,"%d ", &arr[i]);
            }
        }
    }
}

int main()
{
    read_input();
    int args[2]= {0, element_no-1};
    merge_sort((void*)args);
    for(int i=0; i<element_no; i++)
        printf("%d ", arr[i]);

    return 0;
}




