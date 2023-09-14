#include <iostream>
#include <fstream> 
#include <vector>
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_set>

struct Aresta {
    int inicio, destino;
    double coeficiente_inclinacao;
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

std::vector<Aresta> set_arestas(std::vector<std::vector<int>>& adj, std::vector<Ponto> vertices_coords){
    int numVertices = adj.size();
    std::vector<Aresta> vetor_de_arestas;
    

    
    //Para cada vértice, ir em seu vetor de adj e adicionar aresta no sentido de  vertice atual -> aresta correspondente
    for (int v = 0; v < numVertices; v++){
        for (int a : adj[v]){
            Ponto vertice_atual_loop = vertices_coords[v];
            Ponto vertice_coadj = vertices_coords[a];
            double inclinacao_relativa =  InclinacaoRelativa(vertice_atual_loop, vertice_coadj);
            Aresta aresta = {v, a, inclinacao_relativa};
            vetor_de_arestas.push_back(aresta);
        }
    }
    return vetor_de_arestas;

}

void imprimirArestas(const std::vector<Aresta>& arestas) {
    for (const Aresta& aresta : arestas) {
        std::cout << "Inicio: " << aresta.inicio << ", Destino: " << aresta.destino << ", Coeficiente de Inclinacao: " << aresta.coeficiente_inclinacao << std::endl;
    }
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

bool compararPorInclinacao(const Aresta& a, const Aresta& b) {
    return a.coeficiente_inclinacao < b.coeficiente_inclinacao;
}


int encontrarPosicaoAresta(const std::vector<Aresta>& vetorArestas, const Aresta& arestaProcurada) {
    for (int i = 0; i < vetorArestas.size(); ++i) {
        if (vetorArestas[i].inicio == arestaProcurada.inicio &&
            vetorArestas[i].destino == arestaProcurada.destino) {
            return i;  // Retorna a posição da aresta no vetor
        }
    }
    
    return -1;  // Retorna -1 se a aresta não for encontrada no vetor
}

bool saoArestasIguais(const Aresta& aresta1, const Aresta& aresta2) {
    return (
        aresta1.inicio == aresta2.inicio &&
        aresta1.destino == aresta2.destino &&
        aresta1.coeficiente_inclinacao == aresta2.coeficiente_inclinacao
    );
}

int encontrarArestaComMaiorInclinacao(const std::vector<Aresta>& vetorArestas) {
    if (vetorArestas.empty()) {
        return -1;
    }

    int indiceMaiorInclinacao = 0; 
    double maiorInclinacao = vetorArestas[0].coeficiente_inclinacao;

    for (int i = 1; i < vetorArestas.size(); ++i) {
        if (vetorArestas[i].coeficiente_inclinacao > maiorInclinacao) {
            // Se encontrarmos uma aresta com inclinação maior, atualize o índice e o valor da maior inclinação
            indiceMaiorInclinacao = i;
            maiorInclinacao = vetorArestas[i].coeficiente_inclinacao;
        }
    }

    return indiceMaiorInclinacao;
}

    //std::cout  << aresta_atual.destino << std::endl;


    
void dfsPorAresta(
    const std::vector<std::vector<int>>& vetorAdj,
    const std::vector<Aresta>& vetorArestas,
    std::vector<Ponto>& vertices_pontos,
    Aresta aresta_atual,
    Aresta aresta_inicial,
    std::vector<bool>& visitadas,
    std::vector<int>& vetor_vertices_visited
) 
{   
    
    std::cout << "DE:" << aresta_atual.inicio +1  << "PRA:" <<aresta_atual.destino +1 << std::endl;
    vetor_vertices_visited.push_back(aresta_atual.destino);
    int posicao_aresta_atual = encontrarPosicaoAresta(vetorArestas, aresta_atual);
    visitadas[posicao_aresta_atual] = true;

    int num_vertice_atual = aresta_atual.destino;
    Ponto vertice_atual = vertices_pontos[num_vertice_atual];

    std::vector<Aresta> arestas_alcansaveis;
    
    if(aresta_inicial.inicio == num_vertice_atual){return;}

    
    for (int vertice : vetorAdj[num_vertice_atual]) {
        Aresta a_procurar = {num_vertice_atual, vertice, 0};
        int posicao_da_aresta_procurada = encontrarPosicaoAresta(vetorArestas, a_procurar);
        if ((vetorAdj[num_vertice_atual].size() == 1)||(!visitadas[posicao_da_aresta_procurada] && vetorArestas[posicao_da_aresta_procurada].destino != aresta_atual.inicio)) {
            arestas_alcansaveis.push_back(vetorArestas[posicao_da_aresta_procurada]);
        }
    }

    int indiceMaiorInclinacao = encontrarArestaComMaiorInclinacao(arestas_alcansaveis);
    if (saoArestasIguais(arestas_alcansaveis[indiceMaiorInclinacao], aresta_inicial)) {
        return;
    } else {
        dfsPorAresta(vetorAdj, vetorArestas, vertices_pontos, arestas_alcansaveis[indiceMaiorInclinacao], aresta_inicial, visitadas, vetor_vertices_visited);
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

    int num_total_arestas = numero_arestas*2;
    
    std::cout << numero_faces << std::endl;
    std::cout << "Itens do vetor vetor_vertices_visitados:" << std::endl;

    
    // Setar um vetor de arestas
    std::vector<Aresta> arestas = set_arestas(vetores_de_adj, vertices);
    


    //imprimirArestas(arestas);
    int vertice_inicial = 1;
    int indice_aresta_inicial = 0;
    
    //Encontrar a melhor aresta a partir de um vértice qualquer
    std::vector<Aresta> arestas_alcansaveis_vertice_inicial;

    for (int vertice : vetores_de_adj[vertice_inicial]) {
            Aresta a_procurar = {vertice_inicial, vertice, 0};
            int posicao_da_aresta_procurada = encontrarPosicaoAresta(arestas, a_procurar);
            arestas_alcansaveis_vertice_inicial.push_back(arestas[posicao_da_aresta_procurada]);
    }


    int aresta_maior = encontrarArestaComMaiorInclinacao(arestas_alcansaveis_vertice_inicial);
    indice_aresta_inicial = encontrarPosicaoAresta( arestas, arestas_alcansaveis_vertice_inicial[aresta_maior]);



    
    Aresta aresta_inicial = arestas[indice_aresta_inicial];  
    
    int numVertices = vertices.size(); 
    
    std::vector<bool> visitadas(arestas.size(), false);
    std::vector<int> vetor_vertices_visitados;
    
    vetor_vertices_visitados.push_back(aresta_inicial.inicio);
    std::cout << vetor_vertices_visitados.size();
    
    dfsPorAresta(vetores_de_adj, arestas, vertices, aresta_inicial, aresta_inicial, visitadas, vetor_vertices_visitados);
    std::cout << vetor_vertices_visitados.size();

    for (int elemento : vetor_vertices_visitados) {
    std::cout << "Elemento: " << elemento + 1 << std::endl;
}
    
    
    
    arquivo.close(); 
    return 0;
}

