#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graph.h"

typedef struct GRAPH_NODE_ *pnode;

void delete_edge_to(pnode *head, int id);


pedge creat_edge(int w, pnode *pNode);

void addEdge(pedge *e_src, int w, pnode *p_dest) {
    pedge e = creat_edge(w, p_dest);
    if (!*e_src) {
        *e_src = e;
        return;
    }
    pedge tmp = *e_src;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = e;
}

pedge creat_edge(int w, pnode *p_dest) {
    pedge e = (pedge) malloc(sizeof(edge));
    if (!e) {
        printf("Error: memory error");
        exit(1);
    }
    e->weight = w;
    e->endpoint = *p_dest;
    e->next = NULL;
    return e;
}

void build_graph_cmd(pnode *head) {
    int numOfNoeds;
    if (scanf(" %d", &numOfNoeds) != 1) // digit
        printf("Error: not get value as expected");  //self checks
//    printf("\nnum: %d\n", numOfNoeds);
    for (int i = 0; i < numOfNoeds; ++i) {
        add_last(head, i);
    }
}
//

pnode create_node(int id) {
    pnode n = (pnode) malloc(sizeof(node));
    if (n) {
        n->node_num = id;
        n->next = NULL;
        n->nextH = NULL;
        n->edges = NULL;
    }
    return n;
}

void add_first(pnode *H, int id) {
    pnode n = create_node(id);
    if (!n) {
        printf("no memory!");
        return;
    }
    n->next = (struct node *) *H;
    *H = n;
}

void add_last(pnode *H, int id) {
    pnode n = create_node(id);
    if (!n) {
        printf("no memory!");
        return;
    }
    if (!*H) {
        *H = n;
        return;
    }
    pnode tmp = *H;
    while (tmp->next)
        tmp = (pnode) tmp->next;
    tmp->next = (struct node *) n;
}

void delete(pnode *H) {
    while (*H) {
        pnode tmp = *H;
        *H = (pnode) (*H)->next;
        free(tmp);
    }
}

void printGraph_cmd(pnode H) {
    while (H) {
        printf("n:%d ", H->node_num);
        pedge e_tmp = H->edges;
        while (e_tmp) {
            printf("e:(%d, %d) ", (e_tmp->endpoint->node_num), (e_tmp->weight));
            e_tmp = e_tmp->next;
        }
        printf("-> ");
        H = (pnode) H->next;
    }
    printf("||\n");
}

void deleteGraph_cmd(pnode *head) {
    while (*head) {
        pnode tmp = *head;
        *head = (pnode) (*head)->next;
        //delete edges from tmp:
        pedge edgeHead = tmp->edges;
        while (edgeHead) {
            pedge eTmp = edgeHead;
            edgeHead = edgeHead->next;
            free(eTmp);
        }
        delete_edge_to(head, tmp->node_num);
        free(tmp);
    }

void delete_edge_to(pnode *head, int id) {
    pnode tmp = *head;
    while (tmp) {
        //check first edge:
        if (tmp->edges && tmp->edges->endpoint->node_num == id) {
            pedge e = tmp->edges;
            tmp->edges = tmp->edges->next;
            free(e);
            if (!e)
                printf("Error: not delete");
        }
        //check all other edges:
        pedge e = tmp->edges;   //assume that dont have more than 1 edge to id from some node -> Todo: check this
        while (e && e->next &&  e->next->endpoint->node_num != id)
            e = e->next;
        if (e != NULL && e->next != NULL) {    // @e && e->next && e->next->endpoint->node_num == id 
            pedge eTmp = e->next;
            e->next = e->next->next;
            free(eTmp);
        }
        tmp = (pnode) tmp->next;
    }
}

pnode search(pnode *head, int id) {
    pnode tmp = *head;
    while (tmp) {
        if (tmp->node_num != id)
            tmp = (pnode) tmp->next;
        else {
            return tmp;
        }
    }
    return NULL;   //return null if not find
}

void delete_node_cmd(pnode *head, int id) {
    pnode tmp = *head;
    if (tmp->node_num == id) {
        *head = (pnode) tmp->next;
        //delete edges from tmp:
        pedge edgeHead = tmp->edges;
        while (edgeHead) {
            pedge eTmp = edgeHead;
            edgeHead = edgeHead->next;
            free(eTmp);
        }
        delete_edge_to(head, tmp->node_num);
        free(tmp);
        return;
    }
    pnode tmp2;
    while (tmp && tmp->next) {
        tmp2 = (pnode) tmp->next;
        if (tmp2->node_num != id)
            tmp = (pnode) tmp->next;
        else
            break;
    }
    if (tmp == NULL || tmp2 == NULL)
        return; //id not found
    // delet node id:
    tmp->next = tmp2->next;
    //delete edges from tmp:
    pedge edgeHead = tmp2->edges;
    while (edgeHead) {
        pedge eTmp = edgeHead;
        edgeHead = edgeHead->next;
        free(eTmp);
    }
    delete_edge_to(head, tmp2->node_num);
    free(tmp2);
    return;
}

int sizeLLforDijkstra(pnode *head) {
    pnode tmp = *head;
    int counter = 0;
    while (tmp != NULL) {
        counter++;
        tmp = tmp->nextH;
    }
    return counter;
}

pnode min(pnode *head) {
    pnode tmp = *head;
    pnode m = tmp;

    int d = tmp->dist;
    while (tmp != NULL) {
        if (tmp->dist < d) {
            d = tmp->dist;
            m = tmp;
        }

        tmp = tmp->nextH;
    }
    if (m == *head) {
        if ((*head)->nextH == NULL)
            *head = NULL;
        else
            *head = m->nextH;
    } else {
        tmp = *head;
        pnode prev = NULL;
        while (tmp != NULL) {
            if (tmp == m) {
                prev->nextH = tmp->nextH;
                break;
            }
            prev = tmp;
            tmp = tmp->nextH;
        }
    }
    return m;
}

// function for add node to last Dijkstra's queue.
void addlastD(pnode *head, pnode n) {
    pnode tmp = *head;
    if (!tmp) {
        *head = n;
        return;
    }
    while (tmp->nextH != NULL) {
        tmp = tmp->nextH;
    }
    tmp->nextH = n;
}

int dijkstra(pnode *head, pnode src, pnode dest) {
    pnode tmp = *head;
    //Init all nodes in graph.
    while (tmp != NULL) {
        tmp->dist = 99999;//INT_MAX;
        tmp->visit = 0;
        tmp->nextH = NULL;
        tmp = (pnode) tmp->next;
    }
    //Init queue.
    pnode *h = &src;
    src->dist = 0;

    while (sizeLLforDijkstra(h) > 0) {
        //Get the node with minimum value in queue.
        pnode v = min(h);
        v->visit = 2;

        if (v == dest) {
            return v->dist;
        }
        pedge e = v->edges;
        while (e != NULL) {
            pnode n = e->endpoint;
            if (n->visit == 2) {
                e = e->next;
                continue;
            }
            int alt = v->dist + e->weight;
            if (alt < n->dist) {
                n->dist = alt;

                //If the node not in queue and not visited
                if (n->visit < 1) {
                    addlastD(h, n);
                    n->visit = 1;
                }
            }
            e = e->next;
        }
    }
    return -1;
}

int m = 0;

void swap(int *arr, int a, int b) {
    int tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}

void sp_permutation(pnode *head, int res[], int arr[], int len) {
    int i = 0, j = 1, sum = 0, x;
    while (j < len) {
        x = dijkstra(head, search(head, arr[i++]), search(head, arr[j++]));
        if (x == -1) {
            res[m++] = -1;
            return;
        }
        sum += x;
    }
    res[m++] = sum;
}

/* permute an array recursively */
void permute(pnode *head, int res[], int arr[], int start, int end) {
    int i;
    if (start == end) /* this function is done */{
        sp_permutation(head, res, arr, end);
        return;
    }
    permute(head, res, arr, start + 1, end); /* start at next element */
    /* permute remaining elements recursively */
    for (i = start + 1; i < end; i++) {
        if (arr[start] == arr[i]) continue; /* skip */
        swap(arr, start, i);
        permute(head, res, arr, start + 1, end);
        swap(arr, start, i); /* restore element order */

    }
}

int tsp(pnode *head, int arr[], int len) {
    m = 0;
    int f = len, s = 1;
    while (f > 0) {
        s *= f;
        f--;
    }
    int *a = (int *) malloc(sizeof(int) * s);
    if (a == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    permute(head, a, arr, 0, len);
    int min = 99999; //INT_MAX;
    for (int i = 0; i < s; ++i) {
        if (a[i] != -1 && a[i] < min)
            min = a[i];
    }
    if (min == 99999) //INT_MAX
        min = -1;
    free(a);
    return min;
}