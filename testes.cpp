int vertice_inicial = 0;
    std::vector<std::vector<int>> faces;

    Aresta aresta_inicial = {vertice_inicial, vetores_de_adj_com_inclinacao[vertice_inicial][0].first};
    faces.push_back(std::vector<int>());
    faces[0].push_back(aresta_inicial.inicio);


    std:: cout << std::endl;
    
    dfs_tunada(
        vetores_de_adj_com_inclinacao,
        aresta_inicial,
        aresta_inicial,
        faces[0]
    );
    
    std:: cout << std::endl;
    
    for (int valor : faces[0]) {
        std::cout << valor << " ";
    }
    
    std:: cout << std::endl;

    int quantidade_de_vertices_na_face = faces[0].size();

    for (int i = 0; i < quantidade_de_vertices_na_face; i++){
        
        int vertice_inicio = faces[0][i];
        int vertice_destino = faces[0][i+1];
        removerValor(vetores_de_adj_com_inclinacao, vertice_inicio, vertice_destino);
    }

    std:: cout << std::endl;

    Aresta aresta_inicial2 = {1, vetores_de_adj_com_inclinacao[1][0].first};
    
    faces.push_back(std::vector<int>());
    faces[1].push_back(aresta_inicial2.inicio);

    dfs_tunada(
        vetores_de_adj_com_inclinacao,
        aresta_inicial2,
        aresta_inicial2,
        faces[1]
    );
    
    std:: cout << std::endl;
    std:: cout << std::endl;
    std:: cout << std::endl;

    for (int valor : faces[1]) {
        std::cout << valor << " ";
    }