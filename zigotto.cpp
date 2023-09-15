#include <iostream>
#include <fstream> 
#include <vector>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>

struct Aresta {
    int inicio, destino;
};

struct Ponto {
    double x, y;
};

// Distancia euclidiana de a para b.
double Distancia(Ponto a, Ponto b) {
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}
// Coeficiente da reta que passa na origem e p.
double Inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}
// Coeficiente da reta orientada de p para q.
double InclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}
// Determina se ao caminhar de a para b e depois de b para c estamos fazendo uma curva `a esquerda, `a direita, ou seguindo em frente.
int TipoCurva(Ponto a, Ponto b, Ponto c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // esquerda.
    if (v > 0) return +1; // direita.
    return 0; // em frente.
}



void imprimirVetoresDeAdj(const std::vector<std::vector<int>>& vetores_de_adj) {
    std::cout << "Vetores de adj:" << std::endl;

    for (const std::vector<int>& vetor : vetores_de_adj) {
        for (const int& elemento : vetor) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
}

void imprimirVertices(const std::vector<Ponto>& vertices) {
    std::cout << "Vertices:" << std::endl;

    for (const Ponto& ponto : vertices) {
        std::cout << "x: " << ponto.x << ", y: " << ponto.y << std::endl;
    }
}

bool saoArestasIguais(const Aresta& aresta1, const Aresta& aresta2) {
    return (
        aresta1.inicio == aresta2.inicio &&
        aresta1.destino == aresta2.destino 
    );
}

void imprimirVetoresAdjComInclinacao(const std::vector<std::vector<std::pair<int, double>>>& vetor) {
    int i = 0;

    for (const auto& vetor_interno : vetor) {
        for (const auto& par : vetor_interno) {
            std::cout << "Inicio: " << i << "Destino: " << par.first << ", Inclinacao: " << par.second << std::endl;
        }
        i++;
    }
}

void ordenarVetoresInternosDecrescente(std::vector<std::vector<std::pair<int, double>>>& vetor) {
    for (std::vector<std::pair<int, double>>& vetor_interno : vetor) {
        // Use std::sort para ordenar o vetor interno em ordem decrescente
        std::sort(vetor_interno.begin(), vetor_interno.end(),
                  [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                      return a.second > b.second; 
                  });
    }
}



int main() {
    
    std::ifstream arquivo("C:\\Users\\rodri\\Desktop\\tp_alg\\cotovelo.txt"); 

    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    int numero_vertices = 0;
    int numero_arestas = 0;

    arquivo >> numero_vertices >> numero_arestas;

    std::vector<Ponto> vertices;
    std::vector<std::vector<int>> vetores_de_adj;

    double x_vertice, y_vertice;
    int grau_vertice;
    int vertice_adj_atual;

    // Computar os vértices nas EDS
    for (int i = 0; i < numero_vertices; i++) {
        
        
        arquivo >> x_vertice >> y_vertice;
        Ponto p = {x_vertice, y_vertice};
        vertices.push_back(p);

        arquivo >> grau_vertice;

        std::vector<int> vetor_de_adj;

        for (int j = 0; j < grau_vertice; j++) {
            arquivo >> vertice_adj_atual;
            vertice_adj_atual --;
            vetor_de_adj.push_back(vertice_adj_atual);
        }

        vetores_de_adj.push_back(vetor_de_adj);
    }

    int numero_faces = 0;
    numero_faces = numero_arestas - numero_vertices + 2;
    //int num_total_arestas = numero_arestas*2;
    
    std::cout << numero_faces << std::endl;
    
    std::vector<std::vector<std::pair<int, double>>> vetores_de_adj_com_inclinacao;

    // Computar os vetores de adj com fator inclinação
    for (int j = 0; j < numero_vertices; j++) {
        
        std::vector<std::pair<int, double>> vetor_de_adj_com_inclinacao;
        
        for (int elemento : vetores_de_adj[j]) {
            vetor_de_adj_com_inclinacao.push_back(std::make_pair(elemento, InclinacaoRelativa(vertices[j], vertices[elemento])));
            //std::cout << "Inicio: " << j << "Destino: " << vetor_de_adj_com_inclinacao[elemento].first << ", Inclinacao: " << vetor_de_adj_com_inclinacao[elemento].second << std::endl;

        }
        
        vetores_de_adj_com_inclinacao.push_back(vetor_de_adj_com_inclinacao);
    }
    
    ordenarVetoresInternosDecrescente(vetores_de_adj_com_inclinacao);
    //imprimirVetoresAdjComInclinacao(vetores_de_adj_com_inclinacao);
   
    std::set<int> visitados; // Conjunto para controlar vértices visitados
    

    
    int vertice_inicial = 0;
    
    

    
    arquivo.close(); 
    return 0;
}

