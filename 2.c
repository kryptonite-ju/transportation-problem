//Name - Ayush
//Roll - 001911001081
//Department - Information Technology
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct matset
{
    int value;
    bool covered, intersected;
} matset;

typedef struct coordinates
{
    int row, col;
} coordinates;

typedef struct solution
{
    int person, job, cost;
} soln;

typedef struct Restrict
{
    int person, job;
} Restrict;

int main(int argc, char *argv[])
{
    int i, j, k, n, p, order;
    Restrict *restricted = NULL;
    for (i = argc; i < 4; i++)
        argv[i] = "0";
    if (atoi(argv[1]) == 1)
        printf("Enter order of Profit Matrix  ");
    else
        printf("Enter order of Cost Matrix  ");
    if (atoi(argv[3]) == 1)
    {
        printf("Person  ");
        scanf("%d", &n);
        printf("\t\t\t    Job  ");
        scanf("%d", &k);
        if (n > k)
            order = n;
        else
            order = k;
    }
    else
    {
        scanf("%d", &order);
        n = k = order;
    }
    if (atoi(argv[2]) == 1)
    {
        printf("\nNo of restrictions  ");
        scanf("%d", &i);
        restricted = (Restrict *)malloc(i * sizeof(Restrict));
        char special_case;
        int c, ch, count = 1;
        for (j = 0; j < i; j++)
        {
            printf("%d %d\n", i, j);
            printf("\nRestriction %d\n\nRestricted job  ", count);
            scanf("%d", &restricted[j].job);
            printf("\n     to person  ");
            scanf("%d", &restricted[j].person);
            restricted[j].person--;
            restricted[j].job--;
        }
    }

    p = i;

    int CostMatrix[order][order];

    printf("\nEnter the contents of matrix\n\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < k; j++)
        {
            scanf("%d", &CostMatrix[i][j]);
            if (atoi(argv[1]) == 1)
                CostMatrix[i][j] *= -1;
        }
    }

    if (n < k)
    {
        for (i = n; i < k; i++)
        {
            for (j = 0; j < k; j++)
                CostMatrix[i][j] = 0;
        }
    }

    if (n > k)
    {
        for (i = 0; i < n; i++)
        {
            for (j = k; j < n; j++)
                CostMatrix[i][j] = 0;
        }
    }

    matset DeducedMatrix[order][order];
    bool Solution[order][order];
    soln solution[order];

    int row_zero_count = 0, col_zero_count = 0, line_count = 0, intersect_count = 0, max_zero, min_zero, min[order], min_cost = 0, ticked;

    coordinates line[order];

    k = 0;
Solve:
    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            DeducedMatrix[i][j].value = CostMatrix[i][j];
            DeducedMatrix[i][j].covered = false;
            DeducedMatrix[i][j].intersected = false;
            if (restricted)
            {
                for (n = 0; n < p; n++)
                {
                    if (restricted[n].person == i && restricted[n].job == j)
                    {
                        DeducedMatrix[i][j].value = INT_MAX;
                        break;
                    }
                }
            }
            Solution[i][j] = false;
            printf("%d  ", DeducedMatrix[i][j].value);
        }
        printf("\n");
    }

    for (i = 0; i < order; i++)
    {
        min[i] = CostMatrix[i][0];
        for (j = 0; j < order; j++)
        {
            if (CostMatrix[i][j] < min[i])
                min[i] = CostMatrix[i][j];
        }
    }

    for (i = 0; i < order; i++)
    {
        printf("\n");
        for (j = 0; j < order; j++)
        {
            DeducedMatrix[i][j].value -= min[i];
            printf("%4d", DeducedMatrix[i][j].value);
        }
    }

    for (i = 0; i < order; i++)
    {
        min[i] = DeducedMatrix[0][i].value;
        for (j = 0; j < order; j++)
        {
            if (DeducedMatrix[j][i].value < min[i])
                min[i] = DeducedMatrix[j][i].value;
        }
    }

    printf("\n\n");
    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            DeducedMatrix[j][i].value -= min[i];
        }
    }

    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
            printf("%4d", DeducedMatrix[i][j].value);
        printf("\n");
    }

    while (line_count < order)
    {
        do
        {
            max_zero = 0;
            for (i = 0; i < order; i++)
            {
                row_zero_count = col_zero_count = 0;
                int max;
                for (j = 0; j < order; j++)
                {
                    if (DeducedMatrix[i][j].value == 0 && !DeducedMatrix[i][j].covered)
                        row_zero_count++;
                    if (DeducedMatrix[j][i].value == 0 && !DeducedMatrix[j][i].covered)
                        col_zero_count++;
                }
                if (k == 0)
                    max = max_zero;
                if (k == 1)
                    max = max_zero - 1;
                if (row_zero_count > max)
                {
                    max_zero = row_zero_count;
                    line[line_count].row = i;
                    line[line_count].col = 0;
                    if (i == 0)
                        line[line_count].row = -1;
                }

                if (k == 0)
                    max = max_zero;
                if (k == 1)
                    max = max_zero - 1;

                if (col_zero_count > max)
                {
                    max_zero = col_zero_count;
                    line[line_count].row = 0;
                    line[line_count].col = i;
                    if (i == 0)
                        line[line_count].col = -1;
                }
            }
            if (line_count == order && max_zero != 0)
                break;
            if (max_zero != 0 && line_count < order)
            {
                printf("%d %d\n", line[line_count].row, line[line_count].col);
                for (i = 0; i < order; i++)
                {
                    if (line[line_count].row == 0)
                    {
                        if (line[line_count].col == -1)
                            DeducedMatrix[i][0].covered = true;
                        else
                            DeducedMatrix[i][line[line_count].col].covered = true;
                    }
                    if (line[line_count].col == 0)
                    {
                        if (line[line_count].row == -1)
                            DeducedMatrix[0][i].covered = true;
                        else
                            DeducedMatrix[line[line_count].row][i].covered = true;
                    }
                }
                for (i = line_count - 1; i >= 0; i--)
                {
                    if (line[line_count].row == 0 && line[i].col == 0)
                    {
                        int row = line[i].row, col = line[line_count].col;
                        if (row == -1)
                            row = 0;
                        if (col == -1)
                            col = 0;
                        DeducedMatrix[row][col].intersected = true;
                    }
                    if (line[line_count].col == 0 && line[i].row == 0)
                    {
                        int row = line[line_count].row, col = line[i].col;
                        if (row == -1)
                            row = 0;
                        if (col == -1)
                            col = 0;
                        DeducedMatrix[row][col].intersected = true;
                    }
                }

                line_count++;
            }
        } while (max_zero != 0);

        if (line_count == order && max_zero != 0)
        {
            for (i = 0; i < order; i++)
            {
                for (j = 0; j < order; j++)
                {
                    DeducedMatrix[i][j].covered = true;
                    DeducedMatrix[i][j].intersected = false;
                }
            }
        }

        if (line_count != order)
        {
            min[0] = INT_MAX;
            for (i = 0; i < order; i++)
            {
                for (j = 0; j < order; j++)
                {
                    if (!DeducedMatrix[i][j].covered && DeducedMatrix[i][j].value < min[0])
                        min[0] = DeducedMatrix[i][j].value;
                }
            }

            for (i = 0; i < order; i++)
            {
                for (j = 0; j < order; j++)
                {
                    if (!DeducedMatrix[i][j].covered)
                    {
                        DeducedMatrix[i][j].value -= min[0];
                    }
                    else
                    {
                        if (DeducedMatrix[i][j].intersected)
                        {
                            DeducedMatrix[i][j].value += min[0];
                            DeducedMatrix[i][j].intersected = false;
                        }
                        DeducedMatrix[i][j].covered = false;
                    }
                }
            }
            line_count = 0;
            printf("\n\n%d\n\n", min[0]);
        }
    }
    for (i = 0; i < order; i++)
    {
        printf("\n");
        for (j = 0; j < order; j++)
        {
            printf("%4d", DeducedMatrix[i][j].value);
            if (DeducedMatrix[i][j].covered)
                printf("c");
        }
    }

    printf("\n");

    for (i = 0; i < order; i++)
    {
        for (j = 0; j < order; j++)
        {
            if (DeducedMatrix[i][j].value == 0 && !DeducedMatrix[i][j].intersected)
            {
                printf("%d %d\n", i, j);
                Solution[i][j] = true;
            }
        }
    }

    line_count = 0;

    printf("\n");
    do
    {
        min_zero = order;
        for (i = 0; i < order; i++)
        {
            row_zero_count = 0;
            for (j = 0; j < order; j++)
            {
                if (Solution[i][j])
                    row_zero_count++;
            }
            if (row_zero_count != 0 && row_zero_count < min_zero)
            {
                min_zero = row_zero_count;
                ticked = i;
            }
        }

        for (i = 0; i < order; i++)
        {
            if (Solution[ticked][i])
            {
                printf("%d %d\n", ticked, i);
                solution[line_count].person = ticked + 1;
                solution[line_count].job = i + 1;
                solution[line_count++].cost = CostMatrix[ticked][i];
                for (j = 0; j < order; j++)
                {
                    Solution[ticked][j] = false;
                    Solution[j][i] = false;
                }
                break;
            }
        }
    } while (min_zero != order);

    if (line_count != order && k == 0)
    {
        k = 1;
        line_count = 0;
        intersect_count = 0;
        row_zero_count = col_zero_count = 0;
        goto Solve;
    }

    for (i = 0; i < order; i++)
    {
        printf("\nJob%d is assigned to person%d\n", solution[i].job, solution[i].person);
        min_cost += solution[i].cost;
    }

    printf("\nTotal assignment cost=%d\n", atoi(argv[1]) == 1 ? -1 * min_cost : min_cost);

    return 0;
}
