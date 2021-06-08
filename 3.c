//Name - Ayush
//Roll - 001911001081
//Department - Information Technology
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct pair
{
    int x, y;
};

struct polyNode
{
    float coefficient;
    int exponent;
    struct polyNode *next;
};

struct polyLL
{
    struct polyNode *head;
};

typedef struct pair Pair;
typedef struct polyNode Node;
typedef struct polyLL Poly;

Node *createNewNode(float coefficient, int exponent)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    if (!temp)
    {
        printf("\nError!\n");
        exit(1);
    }
    temp->coefficient = coefficient;
    temp->exponent = exponent;
    temp->next = NULL;
    return temp;
}

Poly *createNewPolyLL()
{
    Poly *poly = (Poly *)malloc(sizeof(Poly));
    if (!poly)
    {
        printf("\nError!\n");
        exit(1);
    }
    poly->head = NULL;
    return poly;
}

void displayPolyNomial(Poly *node)
{
    if (!node->head)
        return;

    Node *iter = node->head;
    if (iter)
        printf("%f * x^(%d)  ", iter->coefficient, iter->exponent);
    iter = iter->next;

    while (iter)
    {
        printf("+  %f * x^(%d)  ", iter->coefficient, iter->exponent);
        iter = iter->next;
    }
}

void removePoly(Poly *node)
{
    if (node->head)
    {
        Node *iter = node->head;
        Node *temp;
        while (iter)
        {
            temp = iter;
            iter = iter->next;
            free(temp);
        }
    }
    free(node);
}

void insertPolyNode(float coefficient, int exponent, Poly *req)
{
    Node *temp = createNewNode(coefficient, exponent);
    if (req->head == NULL)
    {
        req->head = temp;
    }
    else
    {
        if (req->head->exponent < exponent)
        {
            temp->next = req->head;
            req->head = temp;
            return;
        }
        Node *iter = req->head;
        Node *prev = iter;
        while (iter)
        {
            if (iter->exponent == exponent)
            {
                iter->coefficient += coefficient;
                free(temp);
                return;
            }
            else if (iter->exponent < exponent)
            {
                prev->next = temp;
                temp->next = iter;
                return;
            }
            prev = iter;
            iter = iter->next;
        }
        prev->next = temp;
        temp->next = NULL;
    }
}

Node *addPolySubFn(Node *node1, Node *node2)
{
    if (!node1 && !node2)
        return NULL;

    if (!node1)
    {
        Node *x, *y, *iter = NULL;
        x = node2;

        while (x)
        {
            if (iter == NULL)
            {
                iter = createNewNode(x->coefficient, x->exponent);
                y = iter;
            }
            else
            {
                y->next = createNewNode(x->coefficient, x->exponent);
                y = y->next;
            }

            x = x->next;
        }

        return iter;
    }

    if (!node2)
    {
        Node *x, *y, *tempHead = NULL;
        x = node1;

        while (x)
        {
            if (tempHead == NULL)
            {
                tempHead = createNewNode(x->coefficient, x->exponent);
                y = tempHead;
            }
            else
            {
                y->next = createNewNode(x->coefficient, x->exponent);
                y = y->next;
            }

            x = x->next;
        }
        return tempHead;
    }
    Node *res = NULL;

    if (node1->exponent > node2->exponent)
    {
        res = createNewNode(node1->coefficient, node1->exponent);
        res->next = addPolySubFn(node1->next, node2);
    }
    else if (node1->exponent < node2->exponent)
    {
        res = createNewNode(node2->coefficient, node2->exponent);
        res->next = addPolySubFn(node1, node2->next);
    }
    else
    {
        res = createNewNode(node1->coefficient + node2->coefficient, node1->exponent);
        res->next = addPolySubFn(node1->next, node2->next);
    }

    return res;
}

Poly *addPolyFn(Poly *x, Poly *y)
{
    Poly *res = createNewPolyLL();
    res->head = addPolySubFn(x->head, y->head);

    return res;
}

Poly *multPolySubFn(Node *node1, Node *node2)
{
    Poly *iter = createNewPolyLL();

    if (!node1 && !node2)
        return iter;

    Node *x, *y;

    x = node1;
    while (x)
    {
        y = node2;
        while (y)
        {
            insertPolyNode(x->coefficient * y->coefficient, x->exponent + y->exponent, iter);
            y = y->next;
        }

        x = x->next;
    }

    return iter;
}

Poly *multPolyFn(Poly *node1, Poly *node2)
{
    return multPolySubFn(node1->head, node2->head);
}

void secretGenFn(int x, Pair pair[])
{
    Poly **LL = (Poly **)malloc(x * sizeof(Poly *));
    for (long long i = 0; i < x; i++)
    {
        LL[i] = createNewPolyLL();
        insertPolyNode(1, 0, LL[i]);
    }

    float deno[x];

    for (long long i = 0; i < x; i++)
    {
        deno[i] = 1;
        for (long long j = 0; j < x; j++)
        {
            if (j != i)
            {
                Poly *reqNode = createNewPolyLL();
                insertPolyNode(1, 1, reqNode);
                insertPolyNode(-pair[j].x, 0, reqNode);
                LL[i] = multPolyFn(LL[i], reqNode);
                deno[i] *= (pair[i].x - pair[j].x);
                removePoly(reqNode);
            }
        }
    }

    Poly *poly2 = createNewPolyLL();
    insertPolyNode(0, 0, poly2);

    for (long long i = 0; i < x; i++)
    {
        Poly *temp = createNewPolyLL();
        insertPolyNode(pair[i].y / deno[i], 0, temp);

        temp = multPolyFn(temp, LL[i]);
        poly2 = addPolyFn(poly2, temp);

        removePoly(temp);
    }

    displayPolyNomial(poly2);
}

int main()
{
    printf("Enter the number of pairs: ");
    int k;
    scanf("%d", &k);
    Pair *pairs = (Pair *)malloc(k * sizeof(Pair));
    for (long long i = 0; i < k; i++)
    {
        printf("\nEnter %lld-th pair:\n", i + 1);
        printf("x : ");
        scanf("%d", &pairs[i].x);
        printf("f(x) : ");
        scanf("%d", &pairs[i].y);
    }

    printf("\nThe decrypted polynomial is: \n");
    secretGenFn(k, pairs);

    return 0;
}
