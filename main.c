#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1000
#define MAX_FIELDS 12
//YASH JAIN

char month[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

struct DateTime {
    int day, month, year;
};
int returnRegion(char **region){
    if(*region[0] == 'N') return 1;
    else if(*region[0] == 'S') return 2;
    else if(*region[0] == 'E') return 3;
    return 4;
}
int CSV(char *line, char *fields[]) {
    int count = 0;
    char *token = strtok(line, ",");
    while (token != NULL && count < MAX_FIELDS) {
        fields[count++] = token;
        token = strtok(NULL, ",");
    }
    return count;
}


int findMonth(char *c){
    int idx = 0;
    for(int i = 0; i < 12; i++){
        while(idx < 4){
            if(c[idx] == month[i][idx]) idx++;
            else{
                idx = 0;
                break;
            }

            if(idx == 4){
                return i + 1;
            }
        }
    }

    return -1;
}


// Comparator function for sorting date-times
int compareDateTimes(const void* a, const void* b) {
    const struct DateTime* dtA = (const struct DateTime*)a;
    const struct DateTime* dtB = (const struct DateTime*)b;

    // Compare years
    if (dtA->year != dtB->year) {
        return dtA->year - dtB->year;
    }

    // Compare months
    if (dtA->month != dtB->month) {
        return dtA->month - dtB->month;
    }

    // Compare days
    return dtA->day - dtB->day;
    
}

// Merge function for merging two sorted arrays
void merge(struct DateTime* arr, int left, int middle, int right, struct DateTime* temp) {
    int i = left, j = middle + 1, k = 0;

    while (i <= middle && j <= right) {
        if (compareDateTimes(&arr[i], &arr[j]) <= 0) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= middle) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = 0; i < k; i++) {
        arr[left + i] = temp[i];
    }
}

// Recursive merge sort function
void meSort(struct DateTime* arr, int left, int right, struct DateTime* temp) {
    if (left < right) {
        int middle = (left + right) / 2;
        meSort(arr, left, middle, temp);
        meSort(arr, middle + 1, right, temp);
        merge(arr, left, middle, right, temp);
    }
}



int main(void) {
    FILE *inputFile = fopen("week01.csv", "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outputFile = fopen("finals.csv", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    char line[MAX_LINE_SIZE];
    char *fields[MAX_FIELDS];

    int check = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }


        int fieldCount = CSV(line, fields);

        struct DateTime dates[10];

        if(check == 0){
            check++;
            for (int i = 0; i < fieldCount; i++) {
                fprintf(outputFile, "%s", fields[i]);

                if (i < fieldCount - 1) {
                    fprintf(outputFile, ",");
                }
            }
            fprintf(outputFile, "\n");
        }

        else {
            for (int i = 1; i < fieldCount; i++) {

                if(i != 0 && i != 11){
                    int start = 0;
                    if(strlen(fields[i]) == 11){
                        dates[i - 1].day = (fields[i][start++] - '0')*10 + (fields[i][start++] - '0');
                    } else {
                        dates[i - 1].day = fields[i][start++] - '0';
                    }
                    
  
                    start++;
                    char month[4];
                
                    for(int x = 0; x < 3; x++){
                        month[x] = fields[i][start];
                        start++;
                    }
                    
                    dates[i - 1].month = findMonth(month);
                    start++;

                    dates[i - 1].year = 1000 * (fields[i][start++] - '0') + 100 * (fields[i][start++] - '0') + 10 * (fields[i][start++] - '0') + (fields[i][start++] - '0');
                }
            }

            int region = returnRegion(&fields[11]);

            struct DateTime temp[10];
            meSort(dates, 0, 9, temp);
            
            int startdate;
            switch(region){
                case 1:
                    break;
                case 2:
                    for(int i = 0; i < 10; i++){
                        dates[i] = temp[9 - i];
                    }
                    break;
                case 3:
                      for(int i = 0; i < 10; i++){
                        dates[i] = temp[9 - i];
                    }
                    // while(compareDateTimes(&dates[startdate],&dates[startdate-1])==0&&startdate<10){
                    //     startdate++;
                    // }

                    if(compareDateTimes(&dates[4], &dates[5]) == 0){
                        startdate = 6;
                    } else {
                        startdate = 5;
                    }

                    for(int i = startdate; i < 10; i++){
                        dates[i] = temp[9 - i];
                    }
                    break;
                    

                case 4:
                    for(int i = 0; i < 10; i++){
                        dates[i] = temp[9 - i];
                    }
                    // while(compareDateTimes(&dates[startdate],&dates[startdate-1])==0&&startdate<10){
                    //     startdate++;
                    // }

                    if(compareDateTimes(&dates[4], &dates[5]) == 0){
                        startdate = 6;
                    } else {
                        startdate = 5;
                    }

                    for(int i = startdate; i < 10; i++){
                        dates[i] = temp[9 - i];
                    }
                    break;
                default:
                    break;
            }
            
            fprintf(outputFile, "%s,", fields[0]);
            for(int i = 0; i < 10; i++){
                fprintf(outputFile,"%d-", dates[i].day);
                fprintf(outputFile,"%s-", month[dates[i].month - 1]);
                fprintf(outputFile,"%d,", dates[i].year);
            }

            fprintf(outputFile, "%s", fields[11]);
            fprintf(outputFile, "\n");

        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
