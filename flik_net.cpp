//
// Created by 김민서 on 26. 5. 24.
// Wireless Network with FLTK GUI (수정됨 - Y 좌표 사용)
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>
#include <string>

// FLTK 헤더
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/fl_draw.H>

using namespace std;

// ==================== 데이터 구조 ====================
struct Point {
    int x, y, z;
};

int n;
double r;
vector<Point> points;
vector<vector<int>> graph;
vector<bool> visited;
vector<vector<int>> groups;

// ==================== 거리 계산 함수 ====================
double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// ==================== 그래프 생성 ====================
void buildGraph() {
    graph.assign(n, {});
    double r2 = r * r;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = points[i].x - points[j].x;
            double dy = points[i].y - points[j].y;
            double dz = points[i].z - points[j].z;

            if (dx*dx + dy*dy + dz*dz <= r2) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
}

// ==================== 연결 요소 찾기 (DFS) ====================
void findConnectedComponents() {
    visited.assign(n, false);
    groups.clear();

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            vector<int> group;
            stack<int> st;
            st.push(i);
            visited[i] = true;

            while (!st.empty()) {
                int cur = st.top();
                st.pop();
                group.push_back(cur);

                for (int nxt : graph[cur]) {
                    if (!visited[nxt]) {
                        visited[nxt] = true;
                        st.push(nxt);
                    }
                }
            }

            sort(group.begin(), group.end());
            groups.push_back(group);
        }
    }
}

// ==================== GUI 윈도우 클래스 ====================
class NetworkWindow : public Fl_Window {
private:
    Fl_Multiline_Output* resultText;
    Fl_Button* closeButton;
    int windowWidth;
    int windowHeight;

    // 그룹별 색상 (최대 8개 그룹)
    const int groupColors[8] = {
        FL_RED,      // 빨강
        FL_BLUE,     // 파랑
        FL_GREEN,    // 초록
        FL_MAGENTA,  // 자주
        FL_DARK_CYAN,// 청록
        FL_DARK_YELLOW, // 노랑
        FL_DARK_MAGENTA, // 진한 자주
        FL_DARK_GREEN     // 진한 초록
    };

public:
    NetworkWindow(int w, int h, const char* title)
        : Fl_Window(w, h, title), windowWidth(w), windowHeight(h) {

        // 결과 텍스트 박스
        resultText = new Fl_Multiline_Output(10, h - 120, w - 20, 80);
        resultText->box(FL_DOWN_BOX);
        resultText->textsize(12);

        // 닫기 버튼
        closeButton = new Fl_Button(w - 80, h - 35, 70, 25, "Close");
        closeButton->callback([](Fl_Widget* widget, void* data) {
            Fl_Window* win = (Fl_Window*)data;
            win->hide();
        }, this);

        end();
    }

    void setResult(const string& text) {
        resultText->value(text.c_str());
    }

    void draw() override {
        Fl_Window::draw();

        // 배경색 (흰색)
        fl_color(FL_WHITE);
        fl_rectf(0, 0, windowWidth, windowHeight - 130);

        // 테두리 그리기
        fl_color(FL_BLACK);
        fl_rect(5, 5, windowWidth - 10, windowHeight - 140);

        // 제목
        fl_font(FL_HELVETICA_BOLD, 16);
        fl_color(FL_BLACK);
        fl_draw("Wireless Network Visualization", 10, 30);

        // 부제목
        fl_font(FL_HELVETICA, 12);
        string subtitle = "Points: " + to_string(n) + " | Range: " + to_string(r);
        fl_draw(subtitle.c_str(), 10, 55);

        if (points.empty() || groups.empty()) return;

        // ===== 수정된 부분: Y 좌표 사용 (Z 대신) =====
        // 좌표 범위 찾기 (x와 y 사용, z는 무시)
        int minX = points[0].x, maxX = points[0].x;
        int minY = points[0].y, maxY = points[0].y;

        for (const auto& p : points) {
            minX = min(minX, p.x);
            maxX = max(maxX, p.x);
            minY = min(minY, p.y);
            maxY = max(maxY, p.y);
        }

        // 여유 공간 추가 (10% 여백)
        int padding = 40;
        int drawAreaX1 = padding;
        int drawAreaX2 = windowWidth - padding;
        int drawAreaY1 = 80;
        int drawAreaY2 = windowHeight - 150;

        int drawWidth = drawAreaX2 - drawAreaX1;
        int drawHeight = drawAreaY2 - drawAreaY1;

        // 스케일 계산 (좌표를 화면 좌표로 변환)
        double scaleX = (maxX - minX) > 0 ? drawWidth / (double)(maxX - minX) : drawWidth / 2.0;
        double scaleY = (maxY - minY) > 0 ? drawHeight / (double)(maxY - minY) : drawHeight / 2.0;

        auto toScreenX = [&](int x) -> int {
            return drawAreaX1 + (int)((x - minX) * scaleX);
        };

        auto toScreenY = [&](int y) -> int {
            return drawAreaY1 + (int)((y - minY) * scaleY);
        };

        // 1. 먼저 간선 그리기
        fl_line_style(FL_SOLID, 1);
        for (int i = 0; i < n; i++) {
            for (int j : graph[i]) {
                if (i < j) {
                    int x1 = toScreenX(points[i].x);
                    int y1 = toScreenY(points[i].y);
                    int x2 = toScreenX(points[j].x);
                    int y2 = toScreenY(points[j].y);

                    fl_color(FL_GRAY);
                    fl_line(x1, y1, x2, y2);
                }
            }
        }

        // 2. 각 점의 그룹 정보 저장
        vector<int> pointToGroup(n, -1);
        for (size_t g = 0; g < groups.size(); g++) {
            for (int v : groups[g]) {
                pointToGroup[v] = g;
            }
        }

        // 3. 점 그리기
        for (int i = 0; i < n; i++) {
            int screenX = toScreenX(points[i].x);
            int screenY = toScreenY(points[i].y);
            int groupId = pointToGroup[i];
            int color = (groupId >= 0 && groupId < 8) ? groupColors[groupId] : FL_BLACK;

            // 외곽선 (검정)
            fl_color(FL_BLACK);
            fl_circle(screenX, screenY, 12);

            // 내부 (그룹 색상)
            fl_color(color);
            fl_circle(screenX, screenY, 10);

            // 점 인덱스 (1-based)
            fl_color(FL_WHITE);
            fl_font(FL_HELVETICA_BOLD, 11);
            char idxStr[5];
            snprintf(idxStr, sizeof(idxStr), "%d", i + 1);
            fl_draw(idxStr, screenX - 5, screenY + 4);
        }

        // 4. 범례 (Legend)
        int legendX = windowWidth - 120;
        int legendY = 80;
        fl_font(FL_HELVETICA, 11);
        fl_color(FL_BLACK);
        fl_draw("Legend (Group)", legendX, legendY);

        for (size_t g = 0; g < groups.size() && g < 8; g++) {
            int y = legendY + 20 + (int)g * 18;
            fl_color(groupColors[g]);
            fl_rectf(legendX, y - 12, 12, 12);
            fl_color(FL_BLACK);
            fl_rect(legendX, y - 12, 12, 12);

            string groupText = "Group " + to_string(g + 1) + " (" + to_string(groups[g].size()) + ")";
            fl_draw(groupText.c_str(), legendX + 18, y);
        }
    }
};

// ==================== 입력 받는 함수 ====================
bool getInput() {
    cout << "\n========================================\n";
    cout << "    Wireless Network Analyzer\n";
    cout << "========================================\n\n";

    cout << "Enter number of points (n) and transmission range (r):\n";
    cout << "> ";
    cin >> n >> r;

    if (n <= 0) {
        cout << "Error: n must be positive!\n";
        return false;
    }

    cout << "\nEnter " << n << " points (x y z):\n";
    points.resize(n);
    for (int i = 0; i < n; i++) {
        cout << "Point " << i + 1 << ": ";
        cin >> points[i].x >> points[i].y >> points[i].z;
    }

    return true;
}

// ==================== 결과 문자열 생성 ====================
string makeResultString() {
    string result = "=== RESULT ===\n\n";
    result += "Number of networks: " + to_string(groups.size()) + "\n\n";

    for (size_t i = 0; i < groups.size(); i++) {
        result += "Group " + to_string(i + 1) + ": ";
        for (size_t j = 0; j < groups[i].size(); j++) {
            result += to_string(groups[i][j] + 1);
            if (j < groups[i].size() - 1) result += ", ";
        }
        result += "\n";
    }

    return result;
}

// ==================== 메인 함수 ====================
int main() {
    // 1. 터미널에서 입력 받기
    if (!getInput()) {
        return 1;
    }

    // 2. 그래프 생성
    buildGraph();

    // 3. 연결 요소 찾기
    findConnectedComponents();

    // 4. 결과를 터미널에도 출력
    cout << "\n========================================\n";
    cout << makeResultString();
    cout << "========================================\n";
    cout << "\nLaunching GUI window...\n";
    cout << "Close the window to exit.\n";

    // 5. GUI 창 생성 및 표시
    NetworkWindow win(900, 650, "Wireless Network Visualization");
    win.setResult(makeResultString());
    win.show();

    // 6. GUI 이벤트 루프 실행
    return Fl::run();
}