//Name - Ayush
//Roll - 001911001081
//Department - Information Technology
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct search
{
	int origin;
	int destination;

} search;

typedef struct data
{
	int element;
	int cost;

} data;

typedef struct transportationProblem
{
	bool allocationsMade, loopCorner;
	int units;
	int cost;
	int non_basic;

} transportationProblem;

typedef struct dual
{
	int *xi, *yi;
	bool *found;
} dual;

int findLoop(transportationProblem **, int, int, int, bool, bool);

dual SearchDual(transportationProblem **, int, int, int, bool);

int main()
{
	data *Demand, *Supply;
	transportationProblem **costMatrix;
	search Isearch[6];
	int i, j, k = 0, originSites, destinationSites;
	int dummyTobeAdded, avail_lowest, avail_lowest_, avail_diff, demand_lowest, demand_lowest_, demand_diff, cost = 0;
	bool **crossedLocation;
	dual dual;
	int zj_cj;
	int choice;
	printf("Enter 1 for Maximization or 0 for minimization ");
	scanf("%d", &choice);
	printf("\nEnter Number of Origins");
	scanf("%d", &originSites);
	printf("\nEnter Number of Destinations\n");
	scanf("%d", &destinationSites);
	if (destinationSites >= originSites)
		dummyTobeAdded = destinationSites;
	else
		dummyTobeAdded = originSites;
	costMatrix = (transportationProblem **)malloc(originSites * sizeof(transportationProblem *));
	for (i = 0; i < originSites; i++)
		costMatrix[i] = (transportationProblem *)malloc(destinationSites * sizeof(transportationProblem));

	crossedLocation = (bool **)malloc(originSites * sizeof(bool *));
	crossedLocation[0] = (bool *)malloc((destinationSites + 1) * sizeof(bool));

	for (i = 1; i < originSites; i++)
		crossedLocation[i] = (bool *)malloc(sizeof(bool));
	for (i = 0; dummyTobeAdded > destinationSites + 1 ? i < dummyTobeAdded : i < destinationSites + 1; i++)
	{
		if (i > 0 && i < destinationSites + 1)
			crossedLocation[0][i] = false;
		if (i < originSites)
			crossedLocation[i][0] = false;
	}

	dual.xi = (int *)malloc(originSites * sizeof(int));
	dual.yi = (int *)malloc(destinationSites * sizeof(int));

	Demand = (data *)malloc(destinationSites * sizeof(data));
	Supply = (data *)malloc(originSites * sizeof(data));

	printf("Cost Matrix\n");
	for (i = 0; i < originSites; i++)
	{
		for (j = 0; j < destinationSites; j++)
		{
			scanf("%d", &costMatrix[i][j].cost);
			if (choice == 1)
				costMatrix[i][j].cost *= -1;
		}
	}

	for (i = 0; i < originSites; i++)
	{
		for (j = 0; j < destinationSites; j++)
			costMatrix[i][j].allocationsMade = costMatrix[i][j].loopCorner = false;
	}

	printf("Provide Supply\n");
	for (i = 0; i < originSites; i++)
	{
		scanf("%d", &Supply[i].element);
	}

	printf("Provide Demand\n");
	for (i = 0; i < destinationSites; i++)
	{
		scanf("%d", &Demand[i].element);
	}

	j = 0;
	for (i = 0; i < dummyTobeAdded; i++)
	{
		if (i < destinationSites)
			j += Demand[i].element;
		if (i < originSites)
			k += Supply[i].element;
	}

	if (j < k)
	{
		destinationSites++;
		if (destinationSites > dummyTobeAdded)
			dummyTobeAdded = destinationSites;
		Demand = (data *)realloc(Demand, destinationSites * sizeof(data));
		for (i = 0; i < originSites; i++)
			costMatrix[i] = (transportationProblem *)realloc(costMatrix[i], destinationSites * sizeof(transportationProblem));
		crossedLocation[0] = (bool *)realloc(crossedLocation[0], (destinationSites + 1) * sizeof(bool));
		dual.yi = (int *)realloc(dual.yi, destinationSites * sizeof(int));
		crossedLocation[0][destinationSites] = false;
		for (i = 0; i < originSites; i++)
		{
			costMatrix[i][destinationSites - 1].cost = 0;
			costMatrix[i][destinationSites - 1].allocationsMade = costMatrix[i][destinationSites - 1].loopCorner = false;
		}
		Demand[destinationSites - 1].element = k - j;
	}

	if (j > k)
	{
		originSites++;
		if (originSites > dummyTobeAdded)
			dummyTobeAdded = originSites;
		Supply = (data *)realloc(Supply, originSites * sizeof(data));
		costMatrix = (transportationProblem **)realloc(costMatrix, originSites * sizeof(transportationProblem *));
		costMatrix[originSites - 1] = (transportationProblem *)malloc(destinationSites * sizeof(transportationProblem));
		crossedLocation = (bool **)realloc(crossedLocation, originSites * sizeof(bool *));
		crossedLocation[originSites - 1] = (bool *)malloc(sizeof(bool));
		dual.xi = (int *)realloc(dual.xi, originSites * sizeof(int));
		crossedLocation[originSites - 1][0] = false;
		for (i = 0; i < destinationSites; i++)
		{
			costMatrix[originSites - 1][i].cost = 0;
			costMatrix[originSites - 1][i].allocationsMade = costMatrix[originSites - 1][i].loopCorner = false;
		}
		Supply[originSites - 1].element = j - k;
	}

	for (k = 0; k < originSites + destinationSites - 1; k++)
	{
		for (i = 0; i < dummyTobeAdded; i++)
		{
			if (i < originSites && crossedLocation[i][0] && crossedLocation[0][i + 1])
				continue;
			if (i < originSites && !crossedLocation[i][0])
			{
				avail_lowest = avail_lowest_ = avail_diff = INT_MAX;
			}
			if (i < destinationSites && !crossedLocation[0][i + 1])
			{
				demand_lowest = demand_lowest = demand_diff = INT_MAX;
			}
			for (j = 0; j < dummyTobeAdded; j++)
			{
				if (i < originSites && j < destinationSites && !crossedLocation[i][0] && !crossedLocation[0][j + 1])
				{
					if (costMatrix[i][j].cost > avail_lowest && ((costMatrix[i][j].cost - avail_lowest) < avail_diff))
					{
						avail_lowest_ = costMatrix[i][j].cost;
					}
					else if (costMatrix[i][j].cost <= avail_lowest)
					{
						avail_lowest_ = avail_lowest;
						avail_lowest = costMatrix[i][j].cost;
					}
					avail_diff = avail_lowest_ - avail_lowest;
				}
				if (j < originSites && i < destinationSites && !crossedLocation[0][i + 1] && !crossedLocation[j][0])
				{
					if (costMatrix[j][i].cost > demand_lowest && ((costMatrix[j][i].cost - demand_lowest) < demand_diff))
					{
						demand_lowest_ = costMatrix[j][i].cost;
					}
					else if (costMatrix[j][i].cost <= demand_lowest)
					{
						demand_lowest_ = demand_lowest;
						demand_lowest = costMatrix[j][i].cost;
					}
					demand_diff = demand_lowest_ - demand_lowest;
				}
			}
			if (i < originSites && !crossedLocation[i][0])
			{
				if (avail_lowest_ == INT_MAX)
					avail_lowest_ = avail_lowest;
				Supply[i].cost = avail_lowest_ - avail_lowest;
			}
			if (i < destinationSites && !crossedLocation[0][i + 1])
			{
				if (demand_lowest_ == INT_MAX)
					demand_lowest_ = demand_lowest;
				Demand[i].cost = demand_lowest_ - demand_lowest;
			}
		}
		int max = -1, max_index = 0, min = INT_MAX, min_index = 0;
		for (j = 0; j < dummyTobeAdded; j++)
		{
			if (j < originSites && !crossedLocation[j][0] && Supply[j].cost > max)
			{
				max = Supply[j].cost;
				max_index = j;
			}
			if (j < destinationSites && !crossedLocation[0][j + 1] && Demand[j].cost > max)
			{
				max = Demand[j].cost;
				max_index = originSites + j;
			}
		}
		for (j = 0; j < dummyTobeAdded; j++)
		{
			if (max_index < originSites && j < destinationSites && !crossedLocation[0][j + 1] && costMatrix[max_index][j].cost < min)
			{
				min = costMatrix[max_index][j].cost;
				min_index = j;
			}
			else if (j < originSites && max_index >= originSites && !crossedLocation[j][0] && costMatrix[j][max_index - originSites].cost < min)
			{
				min = costMatrix[j][max_index - originSites].cost;
				min_index = j;
			}
		}
		if (max_index < originSites)
		{
			if (Supply[max_index].element < Demand[min_index].element)
			{
				costMatrix[max_index][min_index].units = Supply[max_index].element;
				costMatrix[max_index][min_index].allocationsMade = true;
				Demand[min_index].element -= Supply[max_index].element;
				Supply[max_index].element = 0;
				crossedLocation[max_index][0] = true;
			}
			else
			{
				costMatrix[max_index][min_index].units = Demand[min_index].element;
				costMatrix[max_index][min_index].allocationsMade = true;
				Supply[max_index].element -= Demand[min_index].element;
				Demand[min_index].element = 0;
				crossedLocation[0][min_index + 1] = true;
			}
			Isearch[k].origin = max_index;
			Isearch[k].destination = min_index;
		}
		else
		{
			if (Supply[min_index].element < Demand[max_index - originSites].element)
			{
				costMatrix[min_index][max_index - originSites].units = Supply[min_index].element;
				costMatrix[min_index][max_index - originSites].allocationsMade = true;
				Demand[max_index - originSites].element -= Supply[min_index].element;
				Supply[min_index].element = 0;
				crossedLocation[min_index][0] = true;
			}
			else
			{
				costMatrix[min_index][max_index - originSites].units = Demand[max_index - originSites].element;
				costMatrix[min_index][max_index - originSites].allocationsMade = true;
				Supply[min_index].element -= Demand[max_index - originSites].element;
				Demand[max_index - originSites].element = 0;
				crossedLocation[0][(max_index - originSites) + 1] = true;
			}
			Isearch[k].origin = min_index;
			Isearch[k].destination = max_index - originSites;
		}
	}
	printf("Initial Basic Feasible Soln.\n");
	for (k = 0; k < originSites + destinationSites - 1; k++)
	{
		int row = Isearch[k].origin, col = Isearch[k].destination;
		cost += costMatrix[row][col].units * costMatrix[row][col].cost;
	}
	printf("Total assignment cost=%d\n", cost);
	while (1)
	{
		dual = SearchDual(costMatrix, originSites, destinationSites, 0, 0);

		int max = INT_MIN, rowNumber, columnNumber;
		for (i = 0; i < originSites; i++)
		{
			for (j = 0; j < destinationSites; j++)
			{
				if (!costMatrix[i][j].allocationsMade)
				{
					zj_cj = (dual.xi[i] + dual.yi[j]) - costMatrix[i][j].cost;
					if (zj_cj > max)
					{
						max = zj_cj;
						rowNumber = i;
						columnNumber = j;
					}
				}
			}
		}
		if (max > 0)
		{
			costMatrix[rowNumber][columnNumber].loopCorner = true;
			int temp = findLoop(costMatrix, originSites, destinationSites, rowNumber, 0, 1);
			int found = rowNumber;
			bool signChecker = 1, loopChecker = 0;
			costMatrix[rowNumber][columnNumber].units = temp;
			for (i = 0; i < dummyTobeAdded; i++)
			{
				if (loopChecker == 0 && i < destinationSites)
				{
					if (costMatrix[found][i].allocationsMade && costMatrix[found][i].loopCorner)
					{
						if (signChecker == 1)
						{
							costMatrix[found][i].units -= temp;
							if (costMatrix[found][i].units == 0 && !costMatrix[rowNumber][columnNumber].allocationsMade)
							{
								costMatrix[found][i].allocationsMade = false;
								costMatrix[rowNumber][columnNumber].allocationsMade = true;
							}
						}
						signChecker = 0;
						costMatrix[found][i].loopCorner = false;
						found = i;
						loopChecker = 1;
						i = -1;
					}
				}
				else if (loopChecker == 1 && i < originSites)
				{
					if (costMatrix[i][found].loopCorner)
					{
						if (i == rowNumber && found == columnNumber)
							break;
						costMatrix[i][found].units += temp;
						signChecker = 1;
						costMatrix[i][found].loopCorner = false;
						found = i;
						loopChecker = 0;
						i = -1;
					}
				}
			}
		}
		else
			break;
	}
	cost = 0;
	for (i = 0; i < originSites; i++)
	{
		for (j = 0; j < destinationSites; j++)
		{
			if (costMatrix[i][j].allocationsMade)
			{
				cost += costMatrix[i][j].units * costMatrix[i][j].cost;
			}
		}
	}
	printf("Total assignment cost=%d\n\n", choice == 1 ? -1 * cost : cost);
	return 0;
}

int findLoop(transportationProblem **costMatrix, int m, int n, int key, bool loopChecker, bool signChecker)
{
	static int count, call = 0, begin, temp;
	int i;
	if (call++ == 0)
		temp = INT_MAX;
	count = 1;
	for (i = 0; loopChecker == 0 ? i < n : i < m; i++)
	{
		if (loopChecker == 0 && costMatrix[key][i].allocationsMade && !costMatrix[key][i].loopCorner)
		{
			count++;
			costMatrix[key][i].loopCorner = true;
			temp = findLoop(costMatrix, m, n, i, 1, 0);
			if (count == 1)
				costMatrix[key][i].loopCorner = false;
			else
			{
				if (costMatrix[key][i].units - temp < 0)
					temp = costMatrix[key][i].units;
				return temp;
			}
		}
		else if (loopChecker == 1)
		{
			if (costMatrix[i][key].loopCorner && !costMatrix[i][key].allocationsMade)
			{
				count++;
				call = 0;
				return temp;
			}
			else if (costMatrix[i][key].allocationsMade && !costMatrix[i][key].loopCorner)
			{
				count++;
				costMatrix[i][key].loopCorner = true;
				temp = findLoop(costMatrix, m, n, i, 0, 1);
				if (count == 1)
					costMatrix[i][key].loopCorner = false;
				else
				{

					return temp;
				}
			}
		}
	}
	return temp;
}

dual SearchDual(transportationProblem **costMatrix, int m, int n, int no, bool info)
{
	static dual dual;
	static int call = 0;
	int i, more = n, less = m;
	if (m > n)
		more = m;
	if (call++ == 0)
	{
		dual.xi = (int *)malloc(m * sizeof(int));
		dual.yi = (int *)malloc(n * sizeof(int));
		dual.found = (bool *)malloc((m + n) * sizeof(bool));
		if (no >= m)
			no = 0;
		dual.xi[no] = 0;
		dual.found[no] = true;
		for (i = 1; i < m * n; i++)
			dual.found[i] = false;
	}
	for (i = 0; i < more; i++)
	{
		if (info == 0 && i < n && costMatrix[no][i].allocationsMade && !dual.found[m + i])
		{
			dual.yi[i] = costMatrix[no][i].cost - dual.xi[no];
			dual.found[m + i] = true;
			SearchDual(costMatrix, m, n, i, 1);
		}
		if (info == 1 && i < m && costMatrix[i][no].allocationsMade && !dual.found[i])
		{
			dual.xi[i] = costMatrix[i][no].cost - dual.yi[no];
			dual.found[i] = true;
			SearchDual(costMatrix, m, n, i, 0);
		}
	}
	call--;
	return dual;
}
