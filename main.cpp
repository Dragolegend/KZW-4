#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <numeric>

const int nodeN = 10; // liczba miast
std::vector<std::pair<int, int>> nodes; // dane instancji
std::vector<int> saPath; // rozwiązanie
double saBestDist; // najlepsze znalezione rozwiązanie (Dist)
std::vector<int> saBestPath; // najlepsze znalezione rozwiązanie (path)
double saTempStart = 10000; // temperatura Startowa
double saTempAlpha = 0.976; // współczynnik zmiany temperatury
double saTemp; // bieżąca temperatura

void NodeRand() {
    nodes.clear();
    for (int i = 0; i < nodeN; i++) {
        nodes.emplace_back(rand() % 40, rand() % 20);
    }
}

std::vector<int> RandPath() {
    std::vector<int> path(nodeN);
    std::iota(path.begin(), path.end(), 0);
    std::random_shuffle(path.begin(), path.end());
    return path;
}

double PathDist(const std::vector<int>& path) {
    double dist = 0;
    for (int i = 0; i < path.size(); i++) {
        int n0 = path[i];
        int n1 = path[(i + 1) % path.size()];
        double x0 = nodes[n0].first;
        double y0 = nodes[n0].second;
        double x1 = nodes[n1].first;
        double y1 = nodes[n1].second;
        dist += std::round(std::sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1)));
    }
    return dist;
}

std::pair<int, int> RandAB() {
    int a = rand() % nodes.size();
    int b = rand() % (nodes.size() - 1);
    if (b >= a) b++;
    if (a > b) std::swap(a, b);
    return {a, b};
}

void PathMove(std::vector<int>& path, std::pair<int, int> move) {
    int a = move.first;
    int b = move.second;
    while (a < b) {
        std::swap(path[a], path[b]);
        a++;
        b--;
    }
}

void SaStep(std::vector<int>& path, double temp) {
    double c0 = PathDist(path);
    for (int i = 0; i < 100; i++) {
        auto move = RandAB();
        PathMove(path, move);
        double c1 = PathDist(path);
        if (c1 > c0) {
            double praw = std::exp((c0 - c1) / temp);
            if (!(rand() / static_cast<double>(RAND_MAX) < praw)) {
                PathMove(path, move); // Odrzucenie ruchu
                continue;
            }
        }
        c0 = c1; // Akceptacja ruchu
        if (saBestDist > c0) {
            saBestDist = c0;
            saBestPath = path;
        }
    }
}

void Run() {
    saPath = RandPath();
    saBestPath = saPath;
    saBestDist = PathDist(saPath);

    int loopN = 400;
    saTemp = saTempStart;

    while (loopN--) {
        SaStep(saPath, saTemp);
        saTemp *= saTempAlpha;
    }
}

void PrintNodes() {
    for (int i = 0; i < nodes.size(); i++) {
        std::cout << "Miasto " << i << ": (" << nodes[i].first << ", " << nodes[i].second << ")\n";
    }
}

void PrintPath(const std::vector<int>& path) {
    std::cout << "Najlepsza ścieżka:\n";
    for (int i = 0; i < path.size(); i++) {
        std::cout << path[i] << " -> ";
    }
    std::cout << path[0] << "\n";
}

void PrintAsciiArt() {
    std::vector<std::string> grid(20, std::string(40, ' '));

    for (int i = 0; i < nodes.size(); i++) {
        int x = nodes[i].first;
        int y = nodes[i].second;
        grid[y][x] = 'O';
    }

    for (int i = 0; i < saBestPath.size(); i++) {
        int n0 = saBestPath[i];
        int n1 = saBestPath[(i + 1) % saBestPath.size()];
        int x0 = nodes[n0].first;
        int y0 = nodes[n0].second;
        int x1 = nodes[n1].first;
        int y1 = nodes[n1].second;

        int dx = (x1 - x0) ? (x1 - x0) / std::abs(x1 - x0) : 0;
        int dy = (y1 - y0) ? (y1 - y0) / std::abs(y1 - y0) : 0;

        int x = x0, y = y0;
        while (x != x1 || y != y1) {
            grid[y][x] = '*';
            if (x != x1) x += dx;
            if (y != y1) y += dy;
        }
    }

    for (const auto& line : grid) {
        std::cout << line << "\n";
    }
}

int main() {
    srand(time(nullptr));

    NodeRand();
    PrintNodes();

    Run();
    PrintPath(saBestPath);

    std::cout << "Najkrótsza odległość: " << saBestDist << "\n";

    PrintAsciiArt();

    return 0;
}
