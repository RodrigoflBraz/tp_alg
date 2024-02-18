#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


// REFERENCIA USADA:: https://cp-algorithms.com/geometry/planar.html#eulers-theorem


struct Ponto {
    double x, y;
};

// Distancia euclidiana de a para b.
double Distancia(Ponto a, Ponto b) {
    double x = (a.x - b.x);
    double y = (a.y - b.y);

    return sqrt(x * x + y * y);
}

// Coeficiente da reta que passa na origem e p.
double Inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}

// Coeficiente da reta orientada de p para q.v1
double InclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}

// Determina se ao caminhar de a para b e depois de b para c estamos fazendo uma curva a esquerda, a direita, ou seguindo em frente.
int TipoCurva(Ponto a, Ponto b, Ponto c) {
    double v = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);

    if (v < 0) return -1; // esquerda.
    if (v > 0) return +1; // direita.
    return 0; // em frente.
}

int main() {
    int n, m, grau, adj_do_vertice;
    double x, y;

    std::cin >> n >> m;

    std::vector<Ponto> vertices(n + 1);
    std::vector<std::vector<int>> arestas(n + 1);
    std::vector<std::vector<int>> faces;

    for (int i = 1; i <= n; i++) {
        std::cin >> x >> y >> grau;

        vertices[i].x = x;
        vertices[i].y = y;

        for (int j = 0; j < grau; j++) {
            std::cin >> adj_do_vertice;
            arestas[i].push_back(adj_do_vertice);
        }
    }

    auto ordenar_adj_por_inclinacao_relativa = [&](int indice) {
        auto verificar_coeficiente_adj = [&](int a, int b) {
            Ponto p;
            p.x = vertices[a].x - vertices[indice].x;
            p.y = vertices[a].y - vertices[indice].y;

            Ponto q;
            q.x = vertices[b].x - vertices[indice].x;
            q.y = vertices[b].y - vertices[indice].y;

            double inclinacao_p = Inclinacao(p);
            double inclinacao_q = Inclinacao(q);

            if (inclinacao_p != inclinacao_q)
                return inclinacao_p < inclinacao_q;

            return TipoCurva(vertices[indice], vertices[a], vertices[b]) > 0;
        };

        std::sort(arestas[indice].begin(), arestas[indice].end(), verificar_coeficiente_adj);
    };

    auto dfs_tunada = [&]() {
        std::vector<std::vector<bool>> visitados(n + 1);

        for (int i = 1; i <= n; i++) {
            visitados[i].resize(arestas[i].size(), false);
            ordenar_adj_por_inclinacao_relativa(i);
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < arestas[i].size(); j++) {
                if (visitados[i][j]) continue;

                std::vector<int> face;
                int v = i;
                int a = j;

                while (!visitados[v][a]) {

                    visitados[v][a] = true;
                    face.push_back(v);

                    int adj = arestas[v][a];
                    int prox = std::lower_bound(arestas[adj].begin(), arestas[adj].end(), v, [&](int a, int b) {
                        Ponto p;
                        p.x = vertices[a].x - vertices[adj].x;
                        p.y = vertices[a].y - vertices[adj].y;

                        Ponto q;
                        q.x = vertices[b].x - vertices[adj].x;
                        q.y = vertices[b].y - vertices[adj].y;

                        double inclinacao_p = Inclinacao(p);
                        double inclinacao_q = Inclinacao(q);

                        if (inclinacao_p != inclinacao_q)
                            return inclinacao_p < inclinacao_q;

                        return TipoCurva(vertices[adj], vertices[a], vertices[b]) > 0;

                    }) - arestas[adj].begin() + 1;

                    if (prox == arestas[adj].size()) {
                        prox = 0;
                    }

                    v = adj;
                    a = prox;
                }
                face.push_back(face[0]);
                faces.push_back(face);
            }
        }
    };

    dfs_tunada();

    std::cout << faces.size() << std::endl;

    for (int i = 0; i < faces.size(); i++) {
        std::cout << faces[i].size() << " ";

        for (int j = 0; j < faces[i].size(); j++) {
            std::cout << faces[i][j] << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
