# Simulador de Cache
# To-Do List

## **Setup Inicial**
1. **Estrutura do Projeto**
   - [X] Criar diretórios: `src/`, `include/`, `tests/`.
   - [X] Definir arquivos principais: `main.cpp`, `cache.cpp`, `cache.h`, `utils.cpp`, `utils.h`.
   - [X] Configurar compilação com `Makefile` ou `CMakeLists.txt`.

2. **Parsing de Argumentos de Linha de Comando**
   - [X] Pegar entrada: `nsets`, `bsize`, `assoc`, `substituição`, `flag_saida`, `arquivo_de_entrada`.
   - [X] Validar entrada: `nsets`, `bsize`, `assoc`, `substituição`, `flag_saida`, `arquivo_de_entrada`.
   - [X] Garantir que `nsets`, `bsize` e `assoc` sejam potências de 2 (exceto `assoc=1` para mapeamento direto).

---

## **Implementação do Núcleo da Cache**
3. **Cálculo de Bits (Offset, Índice, Tag)**
   - [X] Implementar `log2` para calcular bits de offset (`n_bits_offset = log2(bsize)`).
   - [X] Calcular bits de índice (`n_bits_indice = log2(nsets)`).
   - [X] Calcular bits de tag (`n_bits_tag = 32 - n_bits_offset - n_bits_indice`).

4. **Estrutura de Dados da Cache**
   - [X] Criar uma classe `Cache` com:
     - [X] Arrays para armazenar tags, bits de validade e metadados de substituição.
     - [X] Métodos para buscar/inserir blocos.
   - [X] Mapear conjuntos: `n_sets = nsets`, cada conjunto com `assoc` vias.

5. **Leitura do Arquivo Binário**
   - [X] Ler endereços de 32 bits em formato big-endian.
   - [X] Converter cada endereço para tag, índice e offset (usar bitwise operations).

---

## **Políticas de Substituição**
6. **Implementar FIFO (F)**
   - [X] Para cada conjunto, manter uma fila de ordem de inserção.
   - [X] Substituir o bloco mais antigo em caso de miss.

7. **Implementar LRU (L)**
   - [X] Rastrear o último uso de cada bloco (timestamp ou contador).
   - [X] Substituir o bloco menos recentemente usado.

8. **Implementar Random (R)**
   - [X] Gerar índice aleatório para substituição em caso de miss.

---

## **Classificação de Misses**
9. **Miss Compulsório**
   - [X] Ocorre quando o bloco é acessado pela primeira vez (bit de validade = 0).

10. **Miss de Capacidade**
    - [X] Ocorre em caches totalmente associativas (`assoc = capacidade_total`) quando todos os blocos estão ocupados.

11. **Miss de Conflito**
    - [X] Ocorre em caches mapeadas por conjunto quando há colisão de tags no mesmo conjunto.

---

## **Estatísticas e Saída**
12. **Contadores de Acesso**
    - [X] Total de acessos, hits, misses (compulsórios, capacidade, conflito).

13. **Formatação de Saída**
    - [X] Modo `flag_saida=0`: Saída legível (ex: `Taxa de hits: 95%`).
    - [X] Modo `flag_saida=1`: Saída em CSV (ex: `100000, 0.95, 0.05, 0.01, 0.02, 0.02`).

---

## **Testes e Validação**
14. **Testes Básicos**
    - [X] Testar mapeamento direto (`assoc=1`).
    - [X] Testar caches totalmente associativas (`nsets=1`).

15. **Arquivos de Exemplo**
    - [X] Verificar saída com os arquivos de teste fornecidos (ex: `bin_100.bin` deve retornar `100 0.92 0.08 1.00 0.00 0.00`).

16. **Depuração**
    - [X] Adicionar logs para rastrear hits/misses durante a execução.
    - [X] Validar cálculos de bits e mapeamento de endereços.

---

## **Extras e Otimizações**
17. **Documentação**
    - [ ] Escrever `README.md` com instruções de compilação e uso.
    - [X] Comentar código para explicar funções críticas.

18. **Tratamento de Erros**
    - [X] Validar arquivo de entrada (existência, formato binário).
    - [X] Tratar entradas inválidas (ex: `nsets=0`, `assoc > nsets`).

19. **Otimizações**
    - [X] Usar estruturas eficientes para LRU (ex: listas encadeadas).
    - [X] Evitar cálculos redundantes (ex: pré-calcular máscaras de bits).
