import matplotlib.pyplot as plt
import re

# Função para ler e extrair dados do arquivo
def read_search_data(filepath):
    # Dicionários para armazenar os valores da lista encadeada e da árvore binária
    linked_list_values = {}
    binary_tree_values = {}
    
    try:
        # Abre o arquivo e lê seu conteúdo
        with open(filepath, 'r', encoding='utf-8') as file:
            content = file.read()
            
            # Expressão regular para extrair os valores da lista encadeada
            ll_pattern = r'L\((\d+)\)\s*=\s*(\d+)'
            ll_matches = re.finditer(ll_pattern, content)
            for match in ll_matches:
                index = int(match.group(1))
                value = int(match.group(2))
                linked_list_values[index] = value
            
            # Expressão regular para extrair os valores da árvore binária
            bt_pattern = r'T\((\d+)\)\s*=\s*(\d+)'
            bt_matches = re.finditer(bt_pattern, content)
            for match in bt_matches:
                index = int(match.group(1))
                value = int(match.group(2))
                binary_tree_values[index] = value
                
        # Retorna os valores extraídos
        return linked_list_values, binary_tree_values
    
    # Trata o caso em que o arquivo não é encontrado
    except FileNotFoundError:
        print(f"Erro: O arquivo {filepath} não foi encontrado.")
        return None, None
    except Exception as e:
        print(f"Erro ao ler o arquivo: {str(e)}")
        return None, None

# Função para plotar a comparação de eficiência
def plot_efficiency_comparison(linked_list, binary_tree):
    plt.figure(figsize=(12, 6))
    
    # Ordenar os dados pelos índices
    x_ll = sorted(linked_list.keys())
    y_ll = [linked_list[k] for k in x_ll]
    
    x_bt = sorted(binary_tree.keys())
    y_bt = [binary_tree[k] for k in x_bt]
    
    # Plotar os dados reais
    plt.plot(x_ll, y_ll, 'b-o', label='Lista Encadeada', linewidth=0, markersize=3)
    plt.plot(x_bt, y_bt, 'r-s', label='Árvore de Busca Binária', linewidth=0, markersize=3)
    
    # Adicionar linha de complexidade O(n) para lista encadeada e O(log n) para árvore binária
    plt.plot(x_ll, x_ll, 'b--', label='Complexidade O(n) - Lista Encadeada')
    plt.plot(x_bt, [int(i**0.5) for i in x_bt], 'r--', label='Complexidade O(log n) - Árvore Binária')

    # Configurar o gráfico
    plt.title('Comparação de Eficiência: Lista Encadeada vs Árvore de Busca Binária', 
              fontsize=14, pad=20)
    plt.xlabel('Número de Elementos (n)', fontsize=12)
    plt.ylabel('Número de Comparações', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=10)
    
    # Ajustar os limites dos eixos
    plt.xlim(0, max(max(x_ll), max(x_bt)) + 1)
    plt.ylim(0, max(max(y_ll), max(y_bt)) + 1)
    
    # Adicionar pontos de grade nos eixos x e y
    plt.xticks(range(0, max(max(x_ll), max(x_bt)) + 10, 10))
    plt.yticks(range(0, max(max(y_ll), max(y_bt)) + 10, 10))
    
    return plt

# Função principal para execução do programa
def main():
    # Caminho do arquivo com os resultados
    filepath = r"D:\Documentos\Scripts\C\Testes\output\resultados_comparacoes.txt"
    
    # Ler os dados do arquivo
    linked_list_data, binary_tree_data = read_search_data(filepath)
    
    if linked_list_data is not None and binary_tree_data is not None:
        # Criar e mostrar o gráfico
        plt = plot_efficiency_comparison(linked_list_data, binary_tree_data)
        plt.show()
    else:
        print("Não foi possível criar o gráfico devido a erros na leitura do arquivo.")

# Executa a função principal
if __name__ == "__main__":
    main()
