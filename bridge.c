#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY 1 
#define BLACK 2
#define Max 100

// 노드 구조체
typedef struct Node {
    int dest;
    struct Node* next;
} Node;

// 그래프 구조체
typedef struct {
    int V;
    Node* Adj[Max]; 
    int color[Max];
    int parent[Max];
} Graph;

// 배열 및 변수
int disc[Max], low[Max];
int timer = 0; // DFS 탐색 시 시간 기록을 위한 타이머

// 모든 브리지를 목록으로 저장하기 위한 구조체와 배열
typedef struct {
    int u, v;
} Bridge;
Bridge bridge_list[Max];
int bridge_count = 0;

// 함수 선언부
void DFS(Graph *G);
void DFS_Visit(Graph *G, int u);
void processBridgeMenu(Graph *G);
int edgeExists(Graph *G, int u, int v);

int main() {
    Graph G;
    printf("정점 개수 입력: ");
    scanf("%d", &G.V);
    
    bridge_count = 0;
    timer = 0;

    for (int i = 1; i <= G.V; i++) {
    G.Adj[i] = NULL;   // 주소록을 깨끗하게 비워줍니다.
}
    
    printf("간선 입력 (-1 -1 입력 시 종료): \n");
    while(1) {
        int u, v;
        scanf("%d %d", &u, &v);
        if (u == -1 && v == -1) break; // 종료 조건
        
        // u의 인접 리스트에 v 추가
        Node* newNode1 = (Node*)malloc(sizeof(Node));
        newNode1->dest = v;
        newNode1->next = G.Adj[u];
        G.Adj[u] = newNode1;
        
        // v의 인접 리스트에 u 추가 (무방향 그래프이므로 양쪽 다 연결)
        Node* newNode2 = (Node*)malloc(sizeof(Node));
        newNode2->dest = u;
        newNode2->next = G.Adj[v];
        G.Adj[v] = newNode2;
    }
    
    // DFS 탐색 시작
    DFS(&G);
    
    processBridgeMenu(&G);
    
    return 0;
}

void DFS(Graph *G){
     for (int i = 1; i <= G->V; i++) {
        G->color[i] = WHITE;
        G->parent[i] = -1;        
    }
    timer = 0;

    for(int i = 1; i <= G->V; i++){
        
        if(G->color[i] == WHITE){
            DFS_Visit(G, i);
        }
    }
}

void DFS_Visit(Graph *G, int u) {
    timer = timer + 1;
    disc[u] = timer;
    G->color[u] = GRAY;
    low[u] = timer;

    Node* curr = G->Adj[u]; // u의 인접 리스트의 첫 번째 노드부터 시작
    
    while (curr != NULL) {
        int v = curr->dest; // 현재 이웃 정점 v

        if (G->color[v] == WHITE) { 
            G->parent[v] = u;
            DFS_Visit(G, v);
        

            low[u] = (low[u] < low[v]) ? low[u] : low[v]; // low[u] 업데이트

            if(low[v] > disc[u]) { 
                // (u, v)가 브리지인 경우
                bridge_list[bridge_count].u = u;
                bridge_list[bridge_count].v = v;
                bridge_count++;
            }
        }
        else if(v != G->parent[u]) {
            // 역방향 간선 처리
            low[u] = (low[u] < disc[v]) ? low[u] : disc[v];
        }

        curr = curr->next; // 다음 이웃 노드로 이동
    }

    G->color[u] = BLACK;
}

int edgeExists(Graph *G, int u, int v) { // 간선 (u, v)가 그래프에 존재하는지 확인하는 함수
    Node* curr = G->Adj[u];

    while (curr != NULL) {
        if (curr->dest == v) {
            return 1;
        }
        curr = curr->next;
    }

    return 0;
}

// 하나의 함수에서 1, 2 선택 처리
void processBridgeMenu(Graph *G) {
    int menu;
    int flag = 1;
   
    while(flag) {
        printf("\n[메뉴 선택] 1: 모든 브리지 출력, 2: 특정 간선 판별, 0: 종료\n");
        scanf("%d", &menu);

        if (menu == 0) {
            flag = 0;
        }
        else if (menu == 1) {
            // 모든 브리지 목록 출력 기능
            printf("--- 그래프 내의 모든 브리지 목록 ---\n");
            if (bridge_count == 0) {
                printf("존재하는 브리지가 없습니다.\n");
            } else {
                for (int i = 0; i < bridge_count; i++) {
                    printf("(%d, %d)\n", bridge_list[i].u, bridge_list[i].v);
                }
            }
        } 
        else if (menu == 2) {
            // 특정 간선 판별 기능
            int check_u, check_v;
            printf("판별할 특정 간선 쌍 (u v) 입력: ");
            scanf("%d %d", &check_u, &check_v);

            int is_bridge = 0;
            for (int i = 0; i < bridge_count; i++) {
                if ((bridge_list[i].u == check_u && bridge_list[i].v == check_v) || 
                    (bridge_list[i].u == check_v && bridge_list[i].v == check_u)) {
                    is_bridge = 1;
                    break;
                }
            }

            if (!edgeExists(G, check_u, check_v)) {
                printf("=> 간선 (%d, %d)는 그래프에 존재하지 않습니다.\n",
                    check_u, check_v);
            }
            else if (is_bridge) {
                printf("=> 간선 (%d, %d)는 브리지입니다.\n",
                    check_u, check_v);
            }
            else {
                printf("=> 간선 (%d, %d)는 브리지가 아닙니다.\n",
                    check_u, check_v);
            }
        }
    }
}