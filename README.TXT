1. Instalar ubuntu 16.04
2. Executar o script de inicialização ./scripts/initUbuntu.sh
3. Chamar cada um dos exemplos em ./code/run/*.sh (O script faz make). Por exemplo ./code/run/porto.sh
4. Dar três cliques de modo a copiar a query Overpass QL,  resultado da execução de um exemplo
  4.1 Ir a https://overpass-turbo.eu/
  4.2 Fazer paste da query para a caixa de texto
  4.3 Clicar em Run no menu
  4.4 Clicar na lupa de magnificação (zoom to data) no menu do mapa
  4.5 Comparar com a imagem respetiva imagem em ./code/run/*.png
5. Segunda fase do trabalho
    5.1 Escolher o destino pretendido, nome da rua.
    5.2 Selecionar o destino pretendido através do identificador - Knuth Morris Pratt
    5.3 Se pretendido copiar o output para o overpass de modo a ver o resultado
    5.4 Selecionar o destino pretendido através do identificador - Pesquisa aproximada
    5.5Se pretendido copiar o output para o overpass de modo a ver o resultado

Sistema Operativo
  Ubuntu 16.04
    ./scripts/initUbuntu.sh - script de instalação de dependências e ferramentas

Estrutura
  ./scripts/      - Scripts úteis para configurar a máquina para o desenvolvimento
  ./code/
        src             - código fonte
        makefile        - ficheiro de compilação, targets: debug, all->default
        external        - ferramentas externas ao nosso projeto
        run             - scripts de casos de uso da nossa aplicação e respetivos resultados
        data            - mapas (nodes, roads, subroads) para -mtf e ficheiros json para -qf
        bin             - onde são colocados os ficheiros binários
        buildtemp       - ficheiros temporários de compilação
        Doxyfile        - Config de geração de ficheiros de documentação
        Doxygen         - Pasta com a geração da documentação doxygen
        CAL.workspace   - Ficheiro de projeto codelite

Dependências
  Bibliotecas
    boost
    curl
    curlpp
  Compilador
    C++14

Command Line Options
  ( (--queryfile|-qf) queryfile [outputfile] ) | ( (--maptxtfiles|-mtf) in_nodesfile in_roadsfile in_waysfile in_start_node_id in_goal_node_id out_shortest_file)

  -qf     faz um pedido get a um servidor de acordo com uma localização e query. query file em json.
  -mtf    a partir de um conjunto de ficheiros de um mapa(nodes, roads, ways) encontra o caminho mais curto entre um nó e um outro(start, goal). Imprime o resultado e escreve para um ficheiro(shortest_file) ambos em formato query Overpass QL de modo a visualizar a solução.
