#include <stdio.h>

void bst(double idx[2][8]);

int main() {
    // 소수점 계산을 위해 double 타입 2차원 배열 선언
    // idx[0]은 p (성공 확률), idx[1]은 q (실패 확률)
    double idx[2][8] = {
        {0.0, 0.04, 0.03, 0.05, 0.03, 0.15, 0.15, 0.05}, 
        {0.15, 0.04, 0.04, 0.03, 0.05, 0.04, 0.10, 0.05} 
    };

    bst(idx); // 함수 호출
    
    return 0;
}

// 매개변수로 확률 배열을 받아 OBST 테이블을 계산하고 출력하는 함수
void bst(double idx[2][8]) {
    int n = 7;
    // 8x8 크기의 2차원 배열 선언 (0으로 초기화)
    double w[8][8] = {0.0};
    double c[8][8] = {0.0};
    int r[8][8] = {0};

    // 1. 초기화 단계 (대각선 l=0)
    for (int i = 0; i <= n; i++) {
        w[i][i] = idx[1][i]; // 실패 확률 q값을 w 대각선에 할당
        c[i][i] = 0.0;       // 비용 대각선은 0
    }

    // 2. 동적 계획법 (3중 for문)
    for (int l = 1; l <= n; l++) {           // l: 대각선 간격
        for (int i = 0; i <= n - l; i++) {   // i: 행
            int j = i + l;                   // j: 열
            
            // Weight 점화식 적용
            w[i][j] = w[i][j-1] + idx[0][j] + idx[1][j];
            
            c[i][j] = 9999.0; // 최소값을 찾기 위해 아주 큰 값으로 초기화

            // k 후보 탐색 (i < k <= j)
            for (int k = i + 1; k <= j; k++) {
                // Cost 점화식 적용
                double temp = c[i][k-1] + c[k][j] + w[i][j];
                
                // 최소 비용 갱신 및 루트(k) 저장
                if (temp < c[i][j]) {
                    c[i][j] = temp;
                    r[i][j] = k;
                }
            }
        }
    }

    // 3. 결과 테이블 출력 (콘솔 출력용)
    printf("=== Weight Table (W) ===\n");
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i <= j) printf("%5.2f ", w[i][j]);
            else printf("      "); // 빈칸 처리
        }
        printf("\n");
    }

    printf("\n=== Cost Table (C) ===\n");
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i <= j) printf("%5.2f ", c[i][j]);
            else printf("      ");
        }
        printf("\n");
    }

    printf("\n=== Root Table (R) ===\n");
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i < j) printf("%5d ", r[i][j]);
            else if (i == j) printf("    - "); // 대각선은 비움
            else printf("      ");
        }
        printf("\n");
    }
}

