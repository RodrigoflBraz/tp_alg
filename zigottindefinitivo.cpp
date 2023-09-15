#include <iostream>
#include <fstream> 
#include <vector>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>

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

void ordenar_vetores_de_adj_com_inclinacao(std::vector<std::vector<std::pair<int, double>>>& vetor) {
    for (std::vector<std::pair<int, double>>& vetor_interno : vetor) {
        std::sort(vetor_interno.begin(), vetor_interno.end(),
                  [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                      return a.second > b.second; 
                  });
    }
}


int encontrarIndicePorPrimeiroElemento(const std::vector<std::pair<int, double>>& vetorDePares, int N) {
    for (size_t i = 0; i < vetorDePares.size(); ++i) {
        if (vetorDePares[i].first == N) {
            return static_cast<int>(i);
        }
    }
    return -1;
    
}

void removerValor(std::vector<std::vector<std::pair<int, double>>>& vetores_de_adj_inclinados, int vertice_inicial, int vertice_de_adj) {
    if (vertice_inicial >= 0 && vertice_inicial < vetores_de_adj_inclinados.size()) {
        vetores_de_adj_inclinados[vertice_inicial].erase(
            std::remove_if(vetores_de_adj_inclinados[vertice_inicial].begin(), vetores_de_adj_inclinados[vertice_inicial].end(),
                [vertice_de_adj](const std::pair<int, double>& par) {
                    return par.first == vertice_de_adj;
                }),
            vetores_de_adj_inclinados[vertice_inicial].end());
    }
}


void dfs_tunada(
    const std::vector<std::vector<std::pair<int, double>>>& vetores_de_adj,
    const Aresta& aresta_inicial,
    const Aresta& aresta_atual,
    std::vector<int>& vertices_visitados)
{   
    
    // Se eu volto no vértice inicial, break
    if((vertices_visitados.size() != 1) && saoArestasIguais(aresta_inicial, aresta_atual)){return;}
    
    
    vertices_visitados.push_back(aresta_atual.destino);
    
    //Saber qual indice do meu vetor de adj do destino da aresta corrresponde ao inicio da minha aresta.
    int indice_do_Vertice_anterior_na_adj_do_vertice_atual = encontrarIndicePorPrimeiroElemento(vetores_de_adj[aresta_atual.destino], aresta_atual.inicio);
    int indice_do_proximo_elemento = indice_do_Vertice_anterior_na_adj_do_vertice_atual + 1;
    
    //Saber se esse indice pertence ao vetor de Adj do meu vertice
    if (indice_do_proximo_elemento >= 0 && indice_do_proximo_elemento < vetores_de_adj[aresta_atual.destino].size()){
        Aresta proxima_aresta = {aresta_atual.destino, vetores_de_adj[aresta_atual.destino][indice_do_proximo_elemento].first};
        dfs_tunada(vetores_de_adj, aresta_inicial, proxima_aresta, vertices_visitados);
    }
    else{
        Aresta proxima_aresta = {aresta_atual.destino, vetores_de_adj[aresta_atual.destino][0].first};
        dfs_tunada(vetores_de_adj, aresta_inicial, proxima_aresta, vertices_visitados);        
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
    
    std::vector<std::vector<std::pair<int, double>>> vetores_de_adj_com_inclinacao;

    // Computar os vetores de adj com fator inclinação
    for (int j = 0; j < numero_vertices; j++) {
        
        std::vector<std::pair<int, double>> vetor_de_adj_com_inclinacao;
        
        for (int elemento : vetores_de_adj[j]) {
            vetor_de_adj_com_inclinacao.push_back(std::make_pair(elemento, InclinacaoRelativa(vertices[j], vertices[elemento])));
        }
        
        vetores_de_adj_com_inclinacao.push_back(vetor_de_adj_com_inclinacao);
    }
    
    ordenar_vetores_de_adj_com_inclinacao(vetores_de_adj_com_inclinacao);
    
    std::vector<std::vector<int>> faces;

    

    std::cout << "Cheguei aqui 1" << std::endl;
    for(int i = 0; i < numero_faces; i++){
        
        Aresta aresta_inicial;
        

        //Definir qual o vértice vamos usar como inicial nessa iteração da BFS
        for(int j = 0; j < numero_vertices; j++){
            if(vetores_de_adj_com_inclinacao[j].size() > 0){
                Aresta aresta_inicial = {j, vetores_de_adj_com_inclinacao[j][0].first};
                faces.push_back(std::vector<int>());
                faces[i].push_back(aresta_inicial.inicio);
                break;
                }
        }
        
        std::cout << "Cheguei aqui 3" << std::endl;
        dfs_tunada(
            vetores_de_adj_com_inclinacao,
            aresta_inicial,
            aresta_inicial,
            faces[i]
        );
    
        int quantidade_de_vertices_na_face = faces[i].size();
        
        std::cout << "Cheguei aqui 4" << std::endl;
        for (int n = 0; n < quantidade_de_vertices_na_face; n++){
            int vertice_inicio = faces[i][n];
            int vertice_destino = faces[i][n+1];
            removerValor(vetores_de_adj_com_inclinacao, vertice_inicio, vertice_destino);
        }
        
    }
    
    std::cout << "Cheguei aqui 2" << std::endl;

    std::cout << numero_faces << std::endl;

    arquivo.close(); 
    
     for (const std::vector<int>& vetor : faces) {
        for (int elemento : vetor) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }   
    
    return 0;
}

